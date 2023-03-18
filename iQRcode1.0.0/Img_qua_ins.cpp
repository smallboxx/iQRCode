#include "mainwindow.h"
#include <math.h>

#include <iostream>
#include <ostream>
#include <random>
#include <vector>
#include <opencv2/opencv.hpp>

#include <iostream>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<MNN/Interpreter.hpp>
#include<MNN/ImageProcess.hpp>


#define IMAGE_TARGET 1024

double MainWindow::definition(Mat image){
    assert(image.empty());

    cv::Mat gray_img, smd_image_x, smd_image_y, G;
    if (image.channels() == 3){
        cv::cvtColor(image, gray_img, CV_BGR2GRAY);
    }

    double result = .0f;
    for (int i = 1; i < gray_img.rows-1; ++i){
        uchar *prev = gray_img.ptr<uchar>(i - 1);
        uchar *cur = gray_img.ptr<uchar>(i);
        uchar *next = gray_img.ptr<uchar>(i + 1);
        for (int j = 0; j < gray_img.cols; ++j){
            result += (abs(prev[j - 1] - cur[i])*0.7 + abs(prev[j] - cur[j]) + abs(prev[j + 1] - cur[j])*0.7 +
                abs(next[j - 1] - cur[j])*0.7 + abs(next[j] - cur[j]) + abs(next[j + 1] - cur[j])*0.7 +
                abs(cur[j - 1] - cur[j]) + abs(cur[j + 1] - cur[j]));
        }
    }
    double DR;
    DR=result / gray_img.total();
    String text="sharpness:"+to_string(DR);
    putText(image,text,Point(0,50),cv::FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),2,2,0);
    imshow("tee",image);
    return DR;
}

void MainWindow::img_add_ins(const Mat &img){
    vector<Mat> mv;
    cv::split(img,mv);
    Mat blur_img,maskimg;
    for (int i=0 ; i < mv.size() ; i++) {
        cv::blur(mv[i],blur_img,Size(5,5));
        maskimg=mv[i]-blur_img;
        mv[i]=mv[i]+maskimg;
    }
    Mat reslut;
    cv::merge(mv,reslut);
    //imshow("sdad",reslut);
}

void MainWindow::img_light_adjust(Mat img){
    Mat dst = Mat::zeros(img.size(), img.type());
    Mat N;
    img.convertTo(N,CV_32F);
    double alpha = 1.5;
    double beta = -100;
    double gama = 0.4;

    // 直接使用循环遍历每一个像素，应用公式
   for (int i=0;i<img.rows;i++){
       for(int j=0;j<img.cols;j++){
            if(img.channels()==3){
                //分别获取3个通道像素点的值
                float b = N.at<Vec3f>(i, j)[0];
                float g = N.at<Vec3f>(i, j)[1];
                float r = N.at<Vec3f>(i, j)[2];
                dst.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(b * alpha + beta);
                dst.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(g * alpha + beta);
                dst.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(r * alpha + beta);
            }
            else if (img.channels() == 1) {
                float v = img.at<uchar>(i, j);
                dst.at<uchar>(i, j) = saturate_cast<uchar>(v * alpha + beta);
            }
       }
   }
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gama) * 255.0);
    Mat dst3=img.clone();
    LUT(img, lookUpTable, dst3);
    imshow("org",img);
    imshow("γ correct", dst);
}

void MainWindow::mnn_test(){


}
