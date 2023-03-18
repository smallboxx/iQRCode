/***********************************
*  File: Cam.cpp
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <float.h>
#include <algorithm>
#include <QDebug>
#include "mmdeploy/rotated_detector.h"
using namespace std;
extern MainWindow* mywin;
void MainWindow::initCamera()
{
    PaddleOCR::OCRConfig config("./config.txt");
    m_det=new PaddleOCR::DBDetector(config.det_model_dir, config.use_gpu, config.gpu_id,
                     config.gpu_mem, config.cpu_math_library_num_threads,
                     config.use_mkldnn, config.max_side_len, config.det_db_thresh,
                     config.det_db_box_thresh, config.det_db_unclip_ratio,
                     config.visualize, config.use_tensorrt, config.use_fp16);

    m_rec=new PaddleOCR::CRNNRecognizer(config.rec_model_dir, config.use_gpu, config.gpu_id,
                       config.gpu_mem, config.cpu_math_library_num_threads,
                       config.use_mkldnn, config.char_list_file,
                       config.use_tensorrt, config.use_fp16);

    m_cls=nullptr;
    //读取所有摄像头信息
    infos = QCameraInfo::availableCameras();
    foreach(QCameraInfo info, infos){
        //qDebug()<<info.description()<<info.deviceName();
        ui->Cam_comboBox->addItem(info.description());
    }
    capture.open(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,421);//设置分辨率适配当前展示框
    //capture.set(CV_CAP_PROP_FRAME_HEIGHT,ui->Cam_label->height());
    timer->start(50);
    //indexmap[QString::fromLocal8Bit("品名")]="PM_comboBox";

    indexmap[QString::fromLocal8Bit("主营品类")]="ZYPL_comboBox";
    indexmap[QString::fromLocal8Bit("货号")]="PM_comboBox";
    indexmap[QString::fromLocal8Bit("款号")]="PM_comboBox";
    indexmap[QString::fromLocal8Bit("品牌")]="PP_comboBox";
    indexmap[QString::fromLocal8Bit("四类")]="SL_comboBox";
    indexmap[QString::fromLocal8Bit("中类")]="ZL_comboBox";
    indexmap[QString::fromLocal8Bit("性别")]="XB_comboBox";
    indexmap[QString::fromLocal8Bit("颜色")]="YS_comboBox";
    indexmap[QString::fromLocal8Bit("色号")]="YS_comboBox";
    indexmap[QString::fromLocal8Bit("供应商")]="GYS_comboBox";
    indexmap[QString::fromLocal8Bit("小类")]="XL_comboBox";
    indexmap[QString::fromLocal8Bit("正特价")]="ZTJ_comboBox";
    indexmap[QString::fromLocal8Bit("季节")]="JJ_comboBox";
    indexmap[QString::fromLocal8Bit("大类")]="DL_comboBox";
    indexmap[QString::fromLocal8Bit("采购员")]="CGY_comboBox";
    indexmap[QString::fromLocal8Bit("年份")]="NF_comboBox";
    indexmap[QString::fromLocal8Bit("尺寸组")]="CCZ_comboBox";
    indexmap[QString::fromLocal8Bit("采购性质")]="CGXZ_comboBox";
    indexmap[QString::fromLocal8Bit("版型")]="BX_comboBox";
    indexmap[QString::fromLocal8Bit("长宽高")]="CKG_comboBox";
    indexmap[QString::fromLocal8Bit("年龄段")]="NLD_comboBox";
    indexmap[QString::fromLocal8Bit("经营方式")]="JYFS_comboBox";
    indexmap[QString::fromLocal8Bit("尺码")]="CM_comboBox";
    indexmap[QString::fromLocal8Bit("规格")]="CM_comboBox";
    indexmap[QString::fromLocal8Bit("码号")]="CM_comboBox";
    indexmap[QString::fromLocal8Bit("联系商户")]="LXSH_comboBox";
}



//透视变换
Mat MainWindow::perspective_transform(RotatedRect M,const Mat &img)
{
    mywin->flag_1=0;
    mywin->ten_timer=50;
    //透视变换前的四个点所在坐标,调整顺序为左上、右上、右下、左下
    Point2f  corner[4];
    M.points(corner);//获取四点坐标
    float H[4];//左上、右下
    H[0] = corner[0].x+corner[0].y;
    H[1] = corner[1].x+corner[1].y;
    H[2] = corner[2].x+corner[2].y;
    H[3] = corner[3].x+corner[3].y;
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    for (int i = 0; i<4;i++)
    {
        if(H[i]==(*min_element(H,H+4)))
            a = i;
        if(H[i]==(*max_element(H,H+4)))
            c = i;
    }
    float K[4];//右上、左下
    K[0] = corner[0].y-corner[0].x;
    K[1] = corner[1].y-corner[1].x;
    K[2] = corner[2].y-corner[2].x;
    K[3] = corner[3].y-corner[3].x;
    for (int j = 0; j<4;j++)
    {
        if(K[j]==(*min_element(K,K+4)))
            b = j;
        if(K[j]==(*max_element(K,K+4)))
            d = j;
    }
    //左上
    Point p1;
    p1.x=corner[a].x;
    p1.y=corner[a].y;
    //右上
    Point p2;
    p2.x=corner[b].x;
    p2.y=corner[b].y;
    //右下
    Point p3;
    p3.x=corner[c].x;
    p3.y=corner[c].y;
    //左下
    Point p4;
    p4.x=corner[d].x;
    p4.y=corner[d].y;

    //计算变换后宽高
    int widthA= sqrt(pow((p3.x-p4.x),2)+pow((p3.y-p4.y),2));
    int widthB= sqrt(pow((p2.x-p1.x),2)+pow((p2.y-p1.y),2));
    int width=max(widthA,widthB);
    int heightA= sqrt(pow((p2.x-p3.x),2)+pow((p2.y-p3.y),2));
    int heightB= sqrt(pow((p1.x-p4.x),2)+pow((p1.y-p4.y),2));
    int height=max(heightA,heightB);
    //src_corner
    vector<Point2f> src_corner(4);
    src_corner[0] = p1;
    src_corner[1] = p2;
    src_corner[2] = p3;
    src_corner[3] = p4;

    vector<Point2f> dst_corner(4);
    dst_corner[0] = Point(0,0);
    dst_corner[1] = Point(width-1,0);
    dst_corner[2] = Point(width-1,height-1);
    dst_corner[3] = Point(0,height-1);
    Mat resultImg;
    Mat M_2 = getPerspectiveTransform(src_corner, dst_corner);
    cv::warpPerspective(img,resultImg, M_2,Size(width-1, height-1),CV_INTER_CUBIC);
    return resultImg;
}




Mat MainWindow::mask_img(const Mat &erode)
{

    int w = (mywin->ui->Cam_label->width())/2;
    int h = (mywin->ui->Cam_label->height())/2;
    cv::circle(erode,cvPoint(h,w),20,Scalar(255,255,255),-1);
    return erode;
}

vector<vector<vector<int>>> MainWindow::revise_boxes(vector<vector<vector<int>>> boxes)
{
    if(boxes.size()==0)
        return boxes;
    vector<int> mid;
    for(auto iter:boxes)
    {
        int midline=iter[0][1]+abs(iter[0][1]-iter[2][1])/2;
        mid.push_back(midline);
    }
    //qDebug()<<"box_size"<<boxes.size();
    vector<int> ind;
    for(int i=0;i < mid.size();i++)
    {
        if(abs(mid[i-1]-mid[i])<5)
            ind.push_back(i-1);
    }
    //qDebug()<<"ind"<<ind;
    vector<vector<vector<int>>> re_boxes;
    vector<vector<int>> temporary_boxes(4);
    for (int i=0;i<4;i++) {
        temporary_boxes[i].resize(2);
    }
    if(ind.size() != 0)
    {
        int check=0;
        int j=0;
        while(check < ind.size() && j < boxes.size())
        {
            if(ind[check]==j)
            {
                int x_min=min(boxes[j][0][0],boxes[j+1][0][0]);
                int x_max=max(boxes[j][1][0],boxes[j+1][1][0]);
                int y_min=min(boxes[j][0][1],boxes[j+1][0][1]);
                int y_max=max(boxes[j][2][1],boxes[j+1][2][1]);

                temporary_boxes[0][0]=x_min;
                temporary_boxes[0][1]=y_min;
                temporary_boxes[1][0]=x_max;
                temporary_boxes[1][1]=y_min;
                temporary_boxes[2][0]=x_max;
                temporary_boxes[2][1]=y_max;
                temporary_boxes[3][0]=x_min;
                temporary_boxes[3][1]=y_max;
                re_boxes.push_back(temporary_boxes);
                j=j+1;
                check=check+1;
            }
            else
            {
                re_boxes.push_back(boxes[j]);
            }
            j++;
        }
        while(j<boxes.size()){
            re_boxes.push_back(boxes[j]);
            j++;
        }
        //qDebug()<<"re_boxes"<<re_boxes;
        //qDebug()<<"re_boxes_size"<<re_boxes.size();
        return re_boxes;
    }
    return boxes;

}



void MainWindow::ocr(Mat &resultimg)
{
    qDebug()<<"start_ocr";
     copyMakeBorder(resultimg, resultimg, 100, 100, 100, 100, BORDER_CONSTANT, Scalar(255, 255, 255));
     vector<vector<vector<int>>> boxes;
     mywin->m_det->Run(resultimg,boxes);
     vector<vector<vector<int>>> re_boxes=revise_boxes(boxes);
     vector<string> str_res=mywin->m_rec->Run(re_boxes, resultimg,mywin->m_cls);
     mywin->ui->textBrowser->clear();
     QString check_1 = mywin->codec->toUnicode("：");
     QString check_2 = mywin->codec->toUnicode(":");
     for(auto s:str_res)
     {
         mywin->ui->textBrowser->append(s.data());
         QString text = s.data();
         int i = text.indexOf(check_1);
         int j = text.indexOf(check_2);
         if (i>-1||j>-1)
         {
             QString key = text.mid(0,i);
             QString value = text.mid(i+1);
             //qDebug()<<"Text:"<< key;
             //qDebug()<<"Text:"<< value;
             for (auto iter = mywin->indexmap.begin();iter !=mywin->indexmap.end();iter++)
             {
                 //qDebug()<<iter->first<<endl<<key;
                 if (key.endsWith(iter->first))
                 {
                    auto ind = findChild<QComboBox*>(iter->second);
                    ind->setCurrentText(value);
                 }
             }

         }
         else {
             continue;
         }

     }

}


Mat MainWindow::locate_label(const Mat &gray,const Mat &orimg,bool model)
{
    vector<vector<Point>> contours;
    cv::findContours(gray,contours,cv::RETR_TREE,cv::CHAIN_APPROX_NONE);
    QList<RotatedRect> blocks_1;
    for (auto &cnt : contours)
    {
        if (cnt.size()>500)
        {
            RotatedRect rect = cv::minAreaRect(cnt);
            blocks_1.append(rect);
        }
    }
    QList<RotatedRect> blocks;
    int w = orimg.rows;
    int h = orimg.cols;
    int frame_area = w*h;
    for (auto &fliter : blocks_1)
    {
        int width = fliter.boundingRect().width;
        int height = fliter.boundingRect().height;
        int area_1 = width*height;
        if (area_1 > frame_area/6.5 && area_1 < frame_area/1.5)
        {
            blocks.append(fliter);
        }

    }
    if (blocks.size()>0)
    {
        QList<double> region;
        vector<vector<Point2i>> crop;
        vector<Point2i> points;
        for (auto &check : blocks)
        {
            int width = check.boundingRect().width;
            int height = check.boundingRect().height;
            int area = width*height;
            Point2f vertices[4];
            check.points(vertices);
            points.push_back(vertices[0]);
            points.push_back(vertices[1]);
            points.push_back(vertices[2]);
            points.push_back(vertices[3]);
            crop.push_back(points);
            Mat mask = Mat::zeros(gray.size(),CV_8UC1);
            Mat dst;
            cv::drawContours(mask,crop,0,Scalar::all(255),-1);
            gray.copyTo(dst,mask);
            int w=0;
            for (int k=0;k<dst.rows;k++)
            {
                uchar* src_rows_ptr = &(dst.at<uchar>(k, 0));
                for (int j = 0; j < dst.cols; j++)
                {
                    //将(i,0)数据的地址下的内容看成是一维数组，(i,0)数据的地址是一维数组的名字
                    if (src_rows_ptr[j]>150)
                        w = w+1;
                }
            }
            double k = w/area;
            region.append(k);
            crop.clear();
        }
        int maxindex=0;
        for (int index=0;index<region.size();index++)
        {
            if (index>0)
            {
                if (region[index]>region[index-1])
                    maxindex = index;
            }

        }
        RotatedRect Ma_trix;
        Ma_trix=(blocks[maxindex]);
        int width = Ma_trix.boundingRect().width;
        int height = Ma_trix.boundingRect().height;
        int s= width*height;
        if (ten_timer>0)
            ten_timer--;
        if (abs(flag_area-s)<1000 && ten_timer==0)//判断与前一帧检测框的面积是否大小相近
        {
            flag_1++;//若相近增加权重
        }
        if (flag_1 ==20 || model==true)//每50ms更新一帧，2s中未移动标签，则将ROI提取进行文本矫正后再识别信息
        {
            Mat re_img=perspective_transform(Ma_trix,orimg);
            ocr(re_img);
            //QImage pre_image = Mat2Qt(re_img);
            //ui->pre_label->setPixmap(QPixmap::fromImage(pre_image));//透视变换
        }
        flag_area = s;
        Point2f pts[4];
        blocks[maxindex].points(pts);
        for (int i = 0; i < 4; i++)
        {
            if (i == 3)
            {
                    line(orimg, pts[i], pts[0], Scalar(0, 0, 255), 2, 8, 0);
                    break;
            }
            line(orimg,pts[i], pts[i + 1], Scalar(0, 0, 255), 2, 8, 0);
        }
        return orimg;
    }
    else {
        return orimg;
    }
}

Mat MainWindow::resize_frame(Mat &frame)
{
    Mat resize_img;
    Point center(frame.cols/2,frame.rows/2); //旋转中心
    int Cam_w=ui->Cam_label->width();
    int Cam_h=ui->Cam_label->height();
    double angle = 90.0;  //角度
    //double scale = Cam_w/307;  //缩放系数
    Mat rotMat = getRotationMatrix2D(center,angle,1.0);
    warpAffine(frame,resize_img,rotMat,Size(frame.rows,frame.cols));
    //imshow("23",resize_img);
    Mat esize_img = resize_img(Range(50,475),Range(140,447));
    Mat dst;
    cv::resize(esize_img,dst,Size(Cam_w,Cam_h));
    return dst;
}



QList<RotatedRect> MainWindow::edge_extraction(Mat img){
    Mat image=img.clone();
    Mat imgclone=img.clone();
    medianBlur(image, image, 9);
    cvtColor(image,image,CV_RGB2GRAY);
    Point center(static_cast<float>(image.cols / 2.), static_cast<float>(image.rows / 2.));
    Mat canny_out;
    Canny(image, canny_out, 45, 127, 3, false);
    dilate(canny_out, canny_out, Mat(), Point(-1, -1));
    vector<vector<Point> > contours;
    vector<Vec4i> hierachy;
    findContours(canny_out, contours, hierachy, RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(-1, -1));

    vector<vector<Point>> contours_ploy(contours.size());
    vector<Rect> rects_ploy(contours.size());
    vector<RotatedRect> RotatedRect_ploy;

    for (size_t i = 0; i < contours.size(); i++)
        {

            approxPolyDP(contours[i], contours_ploy[i], 5, true);
            rects_ploy[i] = boundingRect(contours_ploy[i]);

            if (contours_ploy[i].size() > 5)
            {

                RotatedRect temp1 = minAreaRect(contours_ploy[i]);
                Point2f rect_center=temp1.center;
                //qDebug()<<abs(rect_center.x-center.x)<<","<<abs(rect_center.y-center.y);
                if (abs(rect_center.x-center.x)<50 && abs(rect_center.y-center.y)<50)
                    RotatedRect_ploy.push_back(temp1);
            }
        }

    QList<RotatedRect> blocks;
    int w = image.rows;
    int h = image.cols;
    int frame_area = w*h;
    for (auto &fliter : RotatedRect_ploy)
    {
        int width = fliter.boundingRect().width;
        int height = fliter.boundingRect().height;
        int area_1 = width*height;
        if (area_1 > frame_area/8 && area_1 < frame_area/1.2)
        {
            blocks.append(fliter);
        }

    }

    for (int i = 0; i < blocks.size(); i++)
        {
            Point2f pts[4];
            blocks[i].points(pts);
            for (int i = 0; i < 4; i++)
            {
                if (i == 3)
                {
                        line(imgclone, pts[i], pts[0], Scalar(0, 0, 255), 2, 8, 0);
                        break;
                }
                line(imgclone,pts[i], pts[i + 1], Scalar(0, 0, 255), 2, 8, 0);
            }
        }
    //imshow("Canny",imgclone);

    return blocks;

}

RotatedRect MainWindow::RotatedRect_trans(RotatedRect box,double angel){
    Point2f center=box.center;
    Size2f size=box.size;
    float ang=box.angle+angel;
    RotatedRect box_trans=RotatedRect(center,size,ang);
    return box_trans;

}

QList<RotatedRect> MainWindow::frame_pro(Mat img,QList<RotatedRect> blocks_out,double angle){
    Mat M1 = img.clone();
    Mat imgclone = img.clone();
    Point center(static_cast<float>(img.cols / 2.), static_cast<float>(img.rows / 2.));
    double scale=1.0;
    Size dst_sz(img.cols, img.rows);
    Mat M=getRotationMatrix2D(center,angle,scale);
    warpAffine(M1, M1, M, dst_sz, 1,0);
    String ang=to_string(angle);
    Mat M2;
    Mat M3;
    cv::GaussianBlur(M1, M2, cv::Size(5, 5), 0);
    cv::addWeighted(M1,5.5,M2,-4.5,0,M3);
    int A[9][9];
    memset(A,0,sizeof(A));
    for (int i=0;i<9;i++) {
        A[i][2]=1;
        A[i][3]=1;
        A[i][5]=1;
        A[i][6]=1;
    }
    Mat kernel=Mat(9,9,CV_8U,A);
    cv::erode(M3,M3,kernel);
    cv::erode(M3,M3,kernel);
    cv::bitwise_not(M3,M3);
    M3 = mask_img(M3);
    //myimg = mask_img(M3);
    cv::cvtColor(M3,M3,CV_BGR2GRAY);
    cv::adaptiveThreshold(M3, M3, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 41, 20);

    vector<vector<Point>> contours;
    cv::findContours(M3,contours,cv::RETR_TREE,cv::CHAIN_APPROX_NONE);
    QList<RotatedRect> blocks_1;
    for (auto &cnt : contours)
    {
        if (cnt.size()>500)
        {
            RotatedRect rect = cv::minAreaRect(cnt);
            Point2f rect_center=rect.center;
            //qDebug()<<abs(rect_center.x-center.x)<<","<<abs(rect_center.y-center.y);
            if (abs(rect_center.x-center.x)<50 && abs(rect_center.y-center.y)<50)
                blocks_1.append(rect);

        }
    }
    QList<RotatedRect> blocks;
    int w = M1.rows;
    int h = M1.cols;
    int frame_area = w*h;
    for (auto &fliter : blocks_1)
    {
        int width = fliter.boundingRect().width;
        int height = fliter.boundingRect().height;
        int area_1 = width*height;
        if (area_1 > frame_area/10 && area_1 < frame_area)
        {
            blocks.append(fliter);
        }

    }
    if (blocks.size()>0)
    {
        QList<double> region;
        vector<vector<Point2i>> crop;
        vector<Point2i> points;
        for (auto &check : blocks)
        {
            int width = check.boundingRect().width;
            int height = check.boundingRect().height;
            int area = width*height;
            Point2f vertices[4];
            check.points(vertices);
            points.push_back(vertices[0]);
            points.push_back(vertices[1]);
            points.push_back(vertices[2]);
            points.push_back(vertices[3]);
            crop.push_back(points);
            Mat mask = Mat::zeros(M3.size(),CV_8UC1);
            Mat dst;
            cv::drawContours(mask,crop,0,Scalar::all(255),-1);
            M3.copyTo(dst,mask);
            int w=0;
            for (int k=0;k<dst.rows;k++)
            {
                uchar* src_rows_ptr = &(dst.at<uchar>(k, 0));
                for (int j = 0; j < dst.cols; j++)
                {
                    //将(i,0)数据的地址下的内容看成是一维数组，(i,0)数据的地址是一维数组的名字
                    if (src_rows_ptr[j]>150)
                        w = w+1;
                }
            }
            double k = w/area;
            region.append(k);
            crop.clear();
        }
        int maxindex=0;
        for (int index=0;index<region.size();index++)
        {
            if (index>0)
            {
                if (region[index]>region[index-1])
                    maxindex = index;
            }

        }
        Point2f pts[4];
        blocks[maxindex].points(pts);

        Mat draw_rect(img.size(), img.type(), Scalar::all(0));
        for (int i = 0; i < 4; i++)
        {
            if (i == 3)
            {
                    line(draw_rect, pts[i], pts[0], Scalar(0, 0, 255), 2, 8, 0);
                    break;
            }
            line(draw_rect,pts[i], pts[i + 1], Scalar(0, 0, 255), 2, 8, 0);
        }
        //imshow("dilate",draw_rect);
        Point2f pts1[4];
        RotatedRect booo=RotatedRect_trans(blocks[maxindex],angle);
        booo.points(pts1);
        for (int i = 0; i < 4; i++)
        {
            if (i == 3)
            {
                    line(imgclone, pts1[i], pts1[0], Scalar(0, 0, 255), 2, 8, 0);
                    break;
            }
            line(imgclone,pts1[i], pts1[i + 1], Scalar(0, 0, 255), 2, 8, 0);
        }
        //imshow("dilate_tarns"+ang,imgclone);
        blocks_out.append(booo);
    }
    return blocks_out;
}


float MainWindow::get_box_iou(RotatedRect box1, RotatedRect box2){
    Point pt1_tl;
    Point pt1_rb;
    Point pt2_tl;
    Point pt2_rb;

    pt1_tl.x=box1.center.x-box1.size.width;
    pt1_tl.y=box1.center.y-box1.size.height;
    pt1_rb.x=box1.center.x+box1.size.width;
    pt1_rb.y=box1.center.y+box1.size.height;

    pt2_tl.x=box2.center.x-box2.size.width;
    pt2_tl.y=box2.center.y-box2.size.height;
    pt2_rb.x=box2.center.x+box2.size.width;
    pt2_rb.y=box2.center.y+box2.size.height;

    Rect rect1=Rect(pt1_tl,pt1_rb);
    Rect rect2=Rect(pt2_tl,pt2_rb);
    int xx1, yy1, xx2, yy2;

    xx1 = max(rect1.x, rect2.x);
    yy1 = max(rect1.y, rect2.y);
    xx2 = min(rect1.x + rect1.width - 1, rect2.x + rect2.width - 1);
    yy2 = min(rect1.y + rect1.height - 1, rect2.y + rect2.height - 1);

    int insection_width, insection_height;
    insection_width = max(0, xx2 - xx1 + 1);
    insection_height = max(0, yy2 - yy1 + 1);

    float insection_area, union_area, iou;
    insection_area = float(insection_width) * insection_height;
    union_area = float(rect1.width*rect1.height + rect2.width*rect2.height - insection_area);
    iou = insection_area / union_area;
    //qDebug()<<iou;
    return iou;

}

void MainWindow::Mtcnn(){

}
void MainWindow::ReadFrame()
{
    if (!capture.isOpened()){
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请连接摄像头"));
        timer->stop();
        return;
    }
    capture.read(frame);
    frame = resize_frame(frame);
    Mat M1 = frame.clone();
//    Mat M2;
//    Mat M3;
//    cv::GaussianBlur(M1, M2, cv::Size(5, 5), 0);
//    cv::addWeighted(M1,5.5,M2,-4.5,0,M3);
//    int A[9][9];
//    memset(A,0,sizeof(A));
//    for (int i=0;i<9;i++) {
//        A[i][2]=1;
//        A[i][3]=1;
//        A[i][5]=1;
//        A[i][6]=1;
//    }
//    Mat kernel=Mat(9,9,CV_8U,A);
//    cv::erode(M3,M3,kernel);
//    cv::erode(M3,M3,kernel);
//    cv::bitwise_not(M3,M3);
//    M3 = mask_img(M3);
//    //myimg = mask_img(M3);
//    cv::cvtColor(M3,M3,CV_BGR2GRAY);
//    cv::adaptiveThreshold(M3, M3, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 41, 20);
//    qDebug()<<"readframe4";
//    Mat imgin = locate_label(M3,M1,false);
//    qDebug()<<"readframe4";
    QList<RotatedRect> boxes;
    boxes=edge_extraction(frame);
    Mat newimg=compute_flag(boxes,frame,false);
    QImage showimage = Mat2Qt(newimg);
    ui->Cam_label->setPixmap(QPixmap::fromImage(showimage));
    bar_code(mymap);
}
Mat MainWindow::compute_flag(QList<RotatedRect> boxes,Mat img,bool model){
    if(boxes.size()>0){
        int width = boxes[0].size.width;
        int height = boxes[0].size.height;
        int s= width*height;
        if (ten_timer>0)
            ten_timer--;
        if (abs(flag_area-s)<1000 && ten_timer==0)//判断与前一帧检测框的面积是否大小相近
        {
            flag_1++;//若相近增加权重
        }
        if (flag_1 ==20 || model==true)//每50ms更新一帧，2s中未移动标签，则将ROI提取进行文本矫正后再识别信息
        {
            mywin->ui->rec_progressBar->setValue(0);
            set_recbar_value();//1
            set_recbar_value();//2
            Mat re_img=perspective_transform(boxes[0],img);
            set_recbar_value();//3
            set_recbar_value();//4
            set_recbar_value();//5
            set_recbar_value();//6
            set_recbar_value();//7
            set_recbar_value();//8
            set_recbar_value();//9
            ocr(re_img);
            set_recbar_value();//10
            //QImage pre_image = Mat2Qt(re_img);
            //ui->pre_label->setPixmap(QPixmap::fromImage(pre_image));//透视变换
        }
        flag_area = s;
        Point2f pts[4];
        boxes[0].points(pts);
        for (int i = 0; i < 4; i++)
        {
            if (i == 3)
            {
                    line(img, pts[i], pts[0], Scalar(0, 0, 255), 2, 8, 0);
                    break;
            }
            line(img,pts[i], pts[i + 1], Scalar(0, 0, 255), 2, 8, 0);
        }
        return img;
        }
    else {
        return img;
    }
}

QImage MainWindow::Mat2Qt(const Mat &src)
{
       if(src.type() == CV_8UC3)
       {
           //得到图像的的首地址
           const uchar *pSrc = (const uchar*)src.data;
           //以src构造图片
           QImage qImage(pSrc,src.cols,src.rows,src.step,QImage::Format_RGB888);
           //在不改变实际图像数据的条件下，交换红蓝通道
           return qImage.rgbSwapped();
       }
       //四通道图片，带Alpha通道的RGB彩色图像
       else if(src.type() == CV_8UC4)
       {
           const uchar *pSrc = (const uchar*)src.data;
           QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
           //返回图像的子区域作为一个新图像
           return qImage.copy();
       }
       else
       {
           return QImage();
       }
}
