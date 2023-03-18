#include <fstream>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <string>
#include "mmdeploy/rotated_detector.h"


QList<RotatedRect> roitransform_extraction(QList<RotatedRect> boxes,cv::Mat img,double angle) {
    Mat M1 = img.clone();
    Point center(static_cast<float>(img.cols / 2.), static_cast<float>(img.rows / 2.));
    double scale=1.0;
    Size dst_sz(img.cols, img.rows);
    Mat M=getRotationMatrix2D(center,angle,scale);
    warpAffine(M1, M1, M, dst_sz, 1,0);
    String ang=to_string(angle);
    Mat imgclone=img.clone();
    char const* device_name = "cpu";
    char const* model_path = "roi_trans/";
    mmdeploy_rotated_detector_t detector{};
    int status{};
    status = mmdeploy_rotated_detector_create_by_path(model_path, device_name, 0, &detector);
    if (status != MMDEPLOY_SUCCESS) {
    fprintf(stderr, "failed to create rotated detector, code: %d\n", (int)status);
    return boxes;
    }

    mmdeploy_mat_t mat{
      M1.data, M1.rows, M1.cols, 3, MMDEPLOY_PIXEL_FORMAT_BGR, MMDEPLOY_DATA_TYPE_UINT8};

    mmdeploy_rotated_detection_t *rbboxes{};
    int *res_count{};
    status = mmdeploy_rotated_detector_apply(detector, &mat, 1, &rbboxes, &res_count);
    if (status != MMDEPLOY_SUCCESS) {
        fprintf(stderr, "failed to apply rotated detector, code: %d\n", (int)status);
        return boxes;
    }
    RotatedRect bestrect;
    double area=0;
    for (int i = 0; i < *res_count; ++i) {
        // skip low score
        if (rbboxes[i].score < 0.1) {
          continue;
        }
        const auto &rbbox = rbboxes[i].rbbox;
        float xc = rbbox[0];
        float yc = rbbox[1];
        float w = rbbox[2];
        float h = rbbox[3];
        float ag = rbbox[4];
        float wx = w / 2 * std::cos(ag);
        float wy = w / 2 * std::sin(ag);
        float hx = -h / 2 * std::sin(ag);
        float hy = h / 2 * std::cos(ag);
        vector <Point> points;
        cv::Point p1 = {int(xc - wx - hx), int(yc - wy - hy)};
        cv::Point p2 = {int(xc + wx - hx), int(yc + wy - hy)};
        cv::Point p3 = {int(xc + wx + hx), int(yc + wy + hy)};
        cv::Point p4 = {int(xc - wx + hx), int(yc - wy + hy)};
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
        points.push_back(p4);
        RotatedRect temp1 = minAreaRect(points);
        Point2f center=temp1.center;
        Size2f size=temp1.size;
        float ang=temp1.angle+angle;
        double temparea=size.area();
        RotatedRect box_trans=RotatedRect(center,size,ang);
        Point2f rect_center=box_trans.center;
        if (abs(rect_center.x-center.x)<center.x/2 && abs(rect_center.y-center.y)<center.y/2 && temparea>area){
            area=temparea;
            bestrect=box_trans;
        }
     }
    boxes.push_back(bestrect);
    mmdeploy_rotated_detector_release_result(rbboxes, res_count);
    mmdeploy_rotated_detector_destroy(detector);
    return boxes;
}
