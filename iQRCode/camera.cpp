#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_camname_comboBox_activated(int index)
{
    camtimer->stop();
    capture->open(index,cv::CAP_DSHOW);
    camtimer->start(50);
}

void MainWindow::ReadFrame(){
    //qDebug()<<"main_thread:"<<QThread::currentThreadId();
    cv::Mat frame_mat;
    QImage frame_img;
    capture->read(frame_mat);
    if(!frame_mat.empty()){
        frame_mat = Resizeimg(frame_mat,ui->Cam_label->width(),ui->Cam_label->height());
        flip(frame_mat,frame_mat,-1);
        frame_boxes=Erode_extraction(frame_mat);
        if(frame_boxes[0].size.area()>0){
            //定时OCR
            int temp_area=frame_boxes[0].size.width*frame_boxes[0].size.height;
            if (ocr_timer>0)
                ocr_timer--;
            if (abs(ocr_area-temp_area)<1000 && ocr_timer==0 && reocr_timer == 0)//判断与前一帧检测框的面积是否大小相近
            {
                flag++;//若相近增加权重
            }
            if (flag == 15 )//每50ms更新一帧，2s中未移动标签，则将ROI提取进行文本矫正后再识别信息
            {
                Mat re_img=perspective_transform(frame_boxes[0],frame_mat);
                flag=0;
                ocr_timer=50;
                reocr_timer=10;
                ui->ocr_textBrowser->clear();
                thread->setmodel(1,re_img);
                ui->ocr_progressBar->setValue(0);
                ocr_bar();
                thread->start();
            }
            ocr_area = temp_area;
            //绘制检测框
            frame_mat=Painter_roi(frame_boxes,frame_mat);
        }
        else{
            if(reocr_timer > 0){
                reocr_timer--;
            }
        }
        frame_boxes.clear();
        frame_img = Mat2Qt(frame_mat);
        ui->Cam_label->setPixmap(QPixmap::fromImage(frame_img));
    }
}

void MainWindow::edge_result(QList<RotatedRect> boxes){
    frame_boxes.append(boxes);
}

void MainWindow::ocr_result(QList<QString> str_boxes){
    for(QString str:str_boxes){
        ui->ocr_textBrowser->append(str);
    }
}
QList<cv::RotatedRect> MainWindow::Erode_extraction(cv::Mat src){
    QList<RotatedRect> blocks;
    Mat pyr,pup,gau,gray;
    pyrDown(src, pyr, Size(src.cols/2, src.rows/2));
    pyrUp(pyr, pup, src.size());
    cv::GaussianBlur(pup, gau, cv::Size(5, 5), 0);
    cv::addWeighted(pup,5.5,gau,-4.5,0,gray);
    vector<vector<Point> > contours;
    vector<vector<Point> > squares;
    Mat rectKernel = getStructuringElement(MORPH_RECT,Size(50, 3));
    Mat sqKernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    cv::cvtColor(src,gray,COLOR_BGR2GRAY);
    erode(gray,gray,rectKernel);
    dilate(gray,gray,sqKernel);
    bitwise_not(gray,gray);
    adaptiveThreshold(gray, gray, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 25, 10);
    RotatedRect rect=FLD_extraction(gray);
    blocks.append(rect);
    return blocks;
}

RotatedRect MainWindow::FLD_extraction(cv::Mat src){
    int lengtg_threshold=10;
    float distance_threshold=1.41421356f;
    double canny_th1=50.0;
    double canny_th2=50.0;
    int canny_aperture_size=3;
    bool do_merge = false;
    Ptr<FastLineDetector> fld=createFastLineDetector(lengtg_threshold,distance_threshold,canny_th1,canny_th2,canny_aperture_size,do_merge);
    vector<Vec4f> fld_lines;
    fld->detect(src,fld_lines);
    Mat out=src.clone();
    fld->drawSegments(out,fld_lines);
    Point2f pts[2];
    double src_midw=src.cols/2;
    double src_midh=src.rows/2;
    double src_threshold_w=5*src_midw/6;
    double src_threshold_h=5*src_midh/6;
    QList<Vec4f> outputlines;
    if(fld_lines.size()>0){
        for(Vec4f index:fld_lines){
            double dif_w1=abs(index[0]-src_midw);
            double dif_w2=abs(index[2]-src_midw);
            double dif_h1=abs(index[1]-src_midh);
            double dif_h2=abs(index[3]-src_midh);
            int k=(index[3]-index[1])/(index[2]-index[0]);
            line(out,Point2f(index[0],index[1]),Point2f(index[2],index[3]),Scalar(0, 0, 255),2,LINE_AA);
            if(dif_w1<src_threshold_w && dif_w2<src_threshold_w && dif_h1<src_threshold_h && dif_h2<src_threshold_h && k==0){
                if(index[0]>index[2]){
                    float ix=index[2];
                    float iy=index[3];
                    index[2]=index[0];
                    index[3]=index[1];
                    index[0]=ix;
                    index[1]=iy;
                }
                if(outputlines.size()>0){
                    if(outputlines.size()==1){
                        int midoff=abs(outputlines[0][1]+outputlines[0][3]-index[1]-index[3])/2;
                        if(midoff<5){
                            outputlines[0]=Merge_line(outputlines[0],index);
                        }
                        else {
                            if(outputlines[0][1]<index[3])
                                outputlines.append(index);
                            else {
                                outputlines.prepend(index);
                            }
                        }
                    }
                    else if(outputlines.size()>1){
                        for(int i=0;i<outputlines.size();i++){
                            int midoff=abs(outputlines[i][1]+outputlines[i][3]-index[1]-index[3])/2;
                            if(outputlines[0][1]+outputlines[0][3]-index[1]-index[3]>10){
                                outputlines.prepend(index);
                                break;
                            }
                            if(i<outputlines.size()-1){
                                if(midoff<5){
                                    if(abs(outputlines[i+1][1]+outputlines[i+1][3]-index[1]-index[3])>10){
                                        outputlines[i]=Merge_line(outputlines[i],index);
                                    }
                                    else {
                                        if(index[0]>outputlines[i][0] && index[0]<outputlines[i+1][0]){
                                            outputlines[i][2]=outputlines[i+1][2];
                                            outputlines[i][3]=outputlines[i+1][3];
                                        }
                                    }
                                    break;
                                }
                            }
                            else {
                                if(midoff<5){
                                    outputlines[i]=Merge_line(outputlines[i],index);
                                }
                                else {
                                    if(outputlines[i][1]<index[3])
                                        outputlines.append(index);
                                    else {
                                        outputlines.insert(i,index);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                else {
                    outputlines.append(index);
                }
            }
        }
    }
    int src_centerw=src.cols/2;
    Vec4i yofflst;//最小y间距4
    Vec4i leftlist;//最左x4
    leftlist[0]=src_centerw;
    leftlist[1]=src_centerw;
    leftlist[2]=src_centerw;
    leftlist[3]=src_centerw;
    Vec4i rightlist;//最右x4
    for (int i=0;i<outputlines.size();i++) {
        int yoff;
        double x1=outputlines[i][0];
        double y1=outputlines[i][1];
        double x2=outputlines[i][2];
        double y2=outputlines[i][3];
        int len=sqrt(pow(y2-y1,2)+pow(x2-x1,2));
        int center_x=(x2-x1)/2+x1;
        int left=x1;
        int right=x2;
        if(i<outputlines.size()-1){
            yoff=y1+y2-outputlines[i+1][1]-outputlines[i+1][3];
        }
        else if(i==outputlines.size()-1){
            yoff=0;
        }
        if(len>50 && abs(center_x-src_centerw)<20){
            double k=(y2-y1)/(x2-x1);
            for (int j=0;j<3;j++){
                if(yoff<yofflst[j]){
                    yofflst[j]=yoff;
                    break;
                }
            }
            for (int m=0;m<4;m++) {
                if(left<leftlist[m]){
                    leftlist[m]=left;
                    break;
                }
            }
            for (int l=0;l<4;l++) {
                if(right>rightlist[l]){
                    rightlist[l]=right;
                    break;
                }
            }
        }
    }
    int reallyoff=abs(yofflst[1]+yofflst[2]+yofflst[3])/3;
    int reallleft=(leftlist[1]+leftlist[2]+leftlist[3])/3-20;
    int reallright=(rightlist[1]+rightlist[2]+rightlist[3])/3+20;
    vector<Point> recPoints;
    Point2f pt1;
    Point2f pt2;
    int linelen=outputlines.size();
    for (int i=0;i<linelen-1;i++) {
        pt1.x=outputlines[i][0];
        pt1.y=outputlines[i][1];
        pt2.x=outputlines[i][2];
        pt2.y=outputlines[i][3];
        int midoff=abs(outputlines[i][1]-outputlines[i+1][1]);

        if(midoff < reallyoff && outputlines[i][0]>=reallleft && outputlines[i][2]<=reallright){
            recPoints.push_back(pt1);
            recPoints.push_back(pt2);
        }
    }
    if(tempPoints.size()>recPoints.size()&&abs(flagleft-reallleft)<50 && recPoints.size()>0){
        recPoints.clear();
        recPoints=tempPoints;
    }
    tempPoints.clear();
    tempPoints=recPoints;
    flagleft=reallleft;
    RotatedRect outrect;
    if(recPoints.size()>10){
        RotatedRect rect=minAreaRect(recPoints);
        outrect.center.x=rect.center.x;
        outrect.center.y=rect.center.y;
        outrect.size.width=rect.size.width+20;
        outrect.size.height=rect.size.height+20;
        outrect.angle=rect.angle;
    }
    return outrect;
}

Vec4f MainWindow::Merge_line(Vec4f line1, Vec4f line2){
    Vec4f outline;
    if(line1[0]<line2[0]){
        outline[0]=line1[0];
        outline[1]=line1[1];
        outline[2]=line2[2];
        outline[3]=line2[3];
    }
    else if(line1[0]>=line2[0]){
        outline[0]=line2[0];
        outline[1]=line2[1];
        outline[2]=line1[2];
        outline[3]=line1[3];
    }
    return outline;
}

Mat MainWindow::perspective_transform(RotatedRect M,const Mat &img){
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
    cv::warpPerspective(img,resultImg, M_2,Size(width-1, height-1),INTER_CUBIC);
    return resultImg;
}
