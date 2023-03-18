#include "mutithread.h"
#include <QDebug>
#include <QTextCursor>
#include "mainwindow.h"


ROIQThread::ROIQThread(QObject* par) : QThread(par)
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
    initwebapi();
}

ROIQThread::~ROIQThread()
{
}

void ROIQThread::stopImmediately(){
    QMutexLocker locker(&m_lock);
    m_isCanrun = false;
    this->exit(0);
}

void ROIQThread::setmodel(int model_index,Mat src){
    if(model_index==1){
        model=1;
        img_1=src.clone();
    }
    else if(model_index==0){
        model=0;
        img_0=src.clone();
    }
}

void ROIQThread::setmodel2(int model_index,QString str1,QString str2,QString str3,QString str4){
    if(model_index==2){
        model=2;
        str_wbpm=str1;
        str_wbkh=str2;
        str_ys=str3;
        str_cm=str4;
    }
}

void ROIQThread::run()
{
    //qDebug()<<"second_thread:"<<QThread::currentThreadId();
    if(model==0){
        //qDebug()<<"model:"<<model;
        medianBlur(img_0, img_0, 9);
        cvtColor(img_0,img_0,COLOR_RGB2GRAY);
        Point center(static_cast<float>(img_0.cols / 2.), static_cast<float>(img_0.rows / 2.));
        Mat canny_out;
        Canny(img_0, canny_out, 45, 127, 3, false);
        dilate(canny_out, canny_out, Mat(), Point(-1, -1));
        vector<vector<Point> > contours;
        vector<Vec4i> hierachy;
        findContours(canny_out, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));
        vector<vector<Point>> contours_ploy(contours.size());
        vector<Rect> rects_ploy(contours.size());
        vector<RotatedRect> RotatedRect_ploy;

        int w = img_0.rows;
        int h = img_0.cols;
        int frame_area = w*h;
        for (size_t i = 0; i < contours.size(); i++)
            {

                approxPolyDP(contours[i], contours_ploy[i], 5, true);
                rects_ploy[i] = boundingRect(contours_ploy[i]);

                if (contours_ploy[i].size() > 5)
                {

                    RotatedRect temp1 = minAreaRect(contours_ploy[i]);
                    Point2f rect_center=temp1.center;
                    if (abs(rect_center.x-center.x)<w/2 && abs(rect_center.y-center.y)<h/2)
                        RotatedRect_ploy.push_back(temp1);
                }
            }

        QList<RotatedRect> blocks;

        for (auto &fliter : RotatedRect_ploy)
        {
            int width = fliter.boundingRect().width;
            int height = fliter.boundingRect().height;
            int area_1 = width*height;
            if (area_1 > frame_area/10 && area_1 < frame_area/1.2)
            {
            blocks.append(fliter);
            }

        }
        //qDebug()<<"edge:"<<blocks.size();
        emit edge_message(blocks);
    }
    else if(model==1){
        WebSe =new QNetworkAccessManager(this);
        WebSe->moveToThread(this);
        qDebug()<<"start_ocr";
        copyMakeBorder(img_1, img_1, 100, 100, 100, 100, BORDER_CONSTANT, Scalar(255, 255, 255));
        vector<vector<vector<int>>> boxes;
        m_det->Run(img_1,boxes);
        //根据中位线融合boxe框
        vector<vector<int>> temporary_boxes(4);
        for (int i=0;i<4;i++) {
            temporary_boxes[i].resize(2);
        }
        double midline=0;
        double front_midline=0;
        vector<vector<vector<int>>> re_boxes;
        if(boxes.size()>0){
            for(vector<vector<int>> iter_box:boxes){
                midline=iter_box[0][1]+abs(iter_box[0][1]-iter_box[2][1])/2;
                if(abs(midline-front_midline)<8){
                    int x_min=min(iter_box[0][0],re_boxes.back()[0][0]);
                    int x_max=max(iter_box[1][0],re_boxes.back()[1][0]);
                    int y_min=min(iter_box[0][1],re_boxes.back()[0][1]);
                    int y_max=max(iter_box[2][1],re_boxes.back()[2][1]);
                    temporary_boxes[0][0]=x_min;
                    temporary_boxes[0][1]=y_min;
                    temporary_boxes[1][0]=x_max;
                    temporary_boxes[1][1]=y_min;
                    temporary_boxes[2][0]=x_max;
                    temporary_boxes[2][1]=y_max;
                    temporary_boxes[3][0]=x_min;
                    temporary_boxes[3][1]=y_max;
                    re_boxes.pop_back();
                    re_boxes.push_back(temporary_boxes);
                }
                else {
                    re_boxes.push_back(iter_box);
                }
                front_midline=midline;
            }
            vector<string> str_res=m_rec->Run(re_boxes, img_1,m_cls);
            QList<QString> ocrlist;
            for(auto index:str_res){
                QString tempstr=index.data();
                ocrlist.append(tempstr);
            }
            emit ocr_message(ocrlist);
            fuzzy_reasoning(ocrlist);
            get_apitoken();
            webapi_listbywbqk();
        }
        emit ocr_bar();
    }
}

void ROIQThread::fuzzy_reasoning(QList<QString> ocr_boxes){
    bool pmflag=false;
    bool khflag=false;
    bool ysflag=false;
    bool cmflag=false;
//Y1:存在品名、Y2:存在款号、Y3：存在颜色、Y4：存在尺码、Y5：字符串size相对品名、Y6：字符串location相对品名、Y7:存在“名”、Y8：存在“号”、Y9：存在“色”、Y10：存在“尺”
    vector<vector<double>> yu_m{
        {1.00,0.00,0.00,0.00},
        {0.00,1.00,0.00,0.00},
        {0.00,0.00,1.00,0.00},
        {0.00,0.00,0.00,1.00},
        {0.40,0.40,0.10,0.10},
        {0.50,0.30,0.10,0.10},
        {0.70,0.10,0.10,0.10},
        {0.10,0.70,0.10,0.10},
        {0.10,0.10,0.70,0.10},
        {0.10,0.10,0.10,0.70},
    };
    str_wbpm="NULL";
    str_wbkh="NULL";
    str_ys="均色";
    str_cm="均码";
    if(ocr_boxes[0]==QString::fromUtf8("合格证")){
        ocr_boxes.removeFirst();
    }
    int boxsize=ocr_boxes.size();
    vector<vector<double>> xy_m(boxsize);
    for(int i=0;i<boxsize;i++){
        xy_m[i].resize(10);
    }

    for(int i=0;i<ocr_boxes.size();i++){
        QString index=ocr_boxes[i];
        //xy_str_size
        double str_size=index.size();
        if(str_size>10){
            xy_m[i][4]=0.1;
        }
        if(str_size<=10){
            xy_m[i][4]=abs(str_size-5.00)/5.00;
        }
        double location=ocr_boxes.size()-i;
        double str_num=ocr_boxes.size();
        double location_ind=location/str_num;
        xy_m[i][5]=location_ind;
        if(index.indexOf(QRegExp(QString::fromUtf8("[名]")))!=-1)
            xy_m[i][6]=1;
        if(index.indexOf(QRegExp(QString::fromUtf8("[号]")))!=-1)
            xy_m[i][7]=1;
        if(index.indexOf(QRegExp(QString::fromUtf8("[色红橙黄绿青蓝紫白黑粉]")))!=-1)
            xy_m[i][8]=1;
        if(index.indexOf(QRegExp(QString::fromUtf8("[尺规码]")))!=-1)
            xy_m[i][9]=1;
        int pos=index.indexOf(QRegExp("[:：]"));
        if(pos!=-1){
            QString key=index.mid(0,pos);
            QString value=index.mid(pos+1);
            if(key==QString::fromUtf8("品名")){
                xy_m[i][0]=1;
                str_wbpm=value;
                pmflag=true;
                continue;
            }
            if(key==QString::fromUtf8("款号") || key==QString::fromUtf8("货号")){
                xy_m[i][1]=1;
                str_wbkh=value;
                khflag=true;
                continue;
            }
            if(key==QString::fromUtf8("颜色")){
                xy_m[i][2]=1;
                str_ys=value;
                ysflag=true;
                continue;
            }
            if(key==QString::fromUtf8("尺码") || key==QString::fromUtf8("尺寸")){
                xy_m[i][3]=1;
                str_cm=value;
                cmflag=true;
                continue;
            }
        }
        xy_m[i][0]=0.25;
        xy_m[i][1]=0.25;
        xy_m[i][2]=0.25;
        xy_m[i][3]=0.25;
    }
    vector<vector<double>> out_m;
    out_m=mutil(xy_m,yu_m);
    qDebug()<<"mutilend";
    for(int i=0;i < out_m.size();i++){
        for(int j=0;j < out_m[0].size();j++){
            cout<<out_m[i][j]<<" ";
        }
        cout<<endl;
    }
    if(pmflag && khflag && ysflag && cmflag){
       return;
    }
    Vec4i indexocr;
    indexocr[0]=-1;
    indexocr[1]=-1;
    indexocr[2]=-1;
    indexocr[3]=-1;
    for(int i=0;i<4;i++){
        double temp=0.00;
        for(int j=0;j<boxsize;j++){
            if(out_m[j][i]>temp && out_m[j][i]>0.3){
                temp=out_m[j][i];
                indexocr[i]=j;
            }
        }
        temp=0.00;
    }
    if(!pmflag&&indexocr[0]>-1){
        QString str=ocr_boxes[indexocr[0]];
        int pos=str.indexOf(QRegExp("[:：]"));
        if(pos!=-1){
            str_wbpm=str.mid(pos+1);
        }
        else {
            str_wbpm=str;
        }
    }
    if(!khflag&&indexocr[1]>-1){
        QString str=ocr_boxes[indexocr[1]];
        int pos=str.indexOf(QRegExp("[:：]"));
        if(pos!=-1){
            str_wbkh=str.mid(pos+1);
        }
        else {
            str_wbkh=str;
        }
    }
    if(!ysflag && indexocr[2]>-1){
        QString str=ocr_boxes[indexocr[2]];
        int pos=str.indexOf(QRegExp("[:：]"));
        if(pos>-1){
            QString regstr=str.mid(pos+1);
            int pos2=regstr.indexOf(QRegExp("[红橙黄绿青蓝紫白黑粉]"));
            QString erstr=regstr.mid(pos2);
            int pos3=erstr.indexOf(QRegExp("[^色红橙黄绿青蓝紫白黑粉]"));
            if(pos3>-1)
                str_ys=erstr.mid(0,pos3);
            if(pos3==-1)
                str_ys=erstr;
        }
        if(pos==-1){
            str_ys=str;
        }
    }
    if(!cmflag && indexocr[3]>-1){
        QString str=ocr_boxes[indexocr[3]];
        int pos=str.indexOf(QRegExp("[:：]"));
        if(pos>-1){
            QString regstr=str.mid(pos+1);
            int pos2=regstr.indexOf(QRegExp("[A-Za-z0-9]"));
            QString erstr=regstr.mid(pos2);
            int pos3=erstr.indexOf(QRegExp("^[A-Za-z0-9]"));
            if(pos3>-1)
                str_cm=erstr.mid(0,pos3-1);
            if(pos3==-1)
                str_cm=erstr;
        }
        if(pos==-1){
            str_cm=str;
        }
    }
}

vector<vector<double>> ROIQThread::mutil(vector<vector<double>> m1, vector<vector<double>> m2){
    //两矩阵相乘
    size_t m = m1.size();
    size_t n = m1[0].size();
    size_t p = m2[0].size();
    vector<vector<double>> array;
    vector<double> temparay;
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < p; j++) {
            double sum = 0;
            for (size_t k = 0; k < n; k++) {
                    sum =MAX(sum,m1[i][k]*m2[k][j]);
            }
            temparay.push_back(sum);
        }
        array.push_back(temparay);
        temparay.erase(temparay.begin(), temparay.end());
    }
    return array;
}
