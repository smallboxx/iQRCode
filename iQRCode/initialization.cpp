#include "mainwindow.h"
#include "ui_mainwindow.h"
int iq_w;
int iq_h;


void MainWindow::initialization_all(){
    this->initScreen();
    this->initqss();
    this->initprinter();
    this->muti_thread();
    this->initcam();
    this->pushturncam();
    this->initwebapi();
    WebSe=new QNetworkAccessManager();
    connect(this,&MainWindow::rebar,this,&MainWindow::reset_barcode);
}

void MainWindow::initScreen(){
    QList<QScreen *> list_screen = QGuiApplication::screens();
    QRect rect = list_screen.at(0)->geometry();
    int desktop_width = rect.width();
    iq_w=desktop_width/2;
    iq_h=iq_w*0.44;
    QRect iq_rect(QPoint(iq_w/2,iq_h/2),QSize(iq_w,iq_h));
    this->setMinimumSize(iq_w,iq_h);
    this->setGeometry(iq_rect);
}

void MainWindow::initqss(){
    QFile qssfile(":/main_style.qss");
    if(qssfile.open(QFile::ReadOnly)){
        qDebug()<<"load qss success";
        QString style=QLatin1String(qssfile.readAll());
        this->setStyleSheet(style);
        qssfile.close();
    }
    else {
        qDebug()<<"load qss fail";
    }
}

void MainWindow::initcam(){
    //读取所有摄像头信息
    cams = QCameraInfo::availableCameras();
    if(cams.size()!=0){
        foreach(const QCameraInfo &info, cams){
            ui->camname_comboBox->addItem(info.description());
        }
        capture->open(0,cv::CAP_DSHOW);
        camtimer->start(50);
    }
    else {
        QMessageBox::information(this,QString::fromUtf8("警告"),QString::fromUtf8("未接入摄像头"));
        return;
    }
}

void MainWindow::initprinter()
{
    QStringList texts;
    texts<<QString::fromUtf8("N31BT打印机")<<QString::fromUtf8("N41BT打印机");
    ui->printer_comboBox->addItems(texts);
}

void  MainWindow::printer_run(){
    if(noprint){
        QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("请连接打印机"));
        return;
    }
    if(printer_model==0){
        int result = AddLabel(hprinter,135,tagh*9, 1);
        if (result!=0){
            QMessageBox::information(this,QString::fromUtf8("tips"),QString::fromUtf8("初始化标签失败"));
            return;
        }
        QPixmap pixmap = QPixmap::grabWidget(tagpage);
        double ratio=7.42;//50mm,picSize.h=371
        QSize picSize(tagw*ratio,tagh*ratio);
        pixmap=pixmap.scaled(picSize,Qt::KeepAspectRatio);
        pixmap.save("pt.jpg","JPG");
        result = AddImage(hprinter,0,0,15,"pt.jpg");
        result = SetDensity(hprinter, 3);
        //打印标签
        //PreFeed(hprinter,-10);
        NextLabelPos(hprinter);
        LabelPrint(hprinter);
    }
    else if(printer_model==1){
        DemClearBuffer(hprinter);
        int speed=2;
        int density=24;
        int type=0;
        int gap=0;
        int offset=0;
        DemSetup(hprinter,tagw,tagh,speed,density,type,gap,offset);
        QPixmap pixmap = QPixmap::grabWidget(tagpage);
        double ratio=7.42;//50mm,picSize.h=371
        QSize picSize(tagw*ratio,tagh*ratio);
        pixmap=pixmap.scaled(picSize,Qt::KeepAspectRatio);
        pixmap.save("pt.jpg","JPG");
        DemBitMap(hprinter,10,30,0,"pt.jpg");
        DemHome(hprinter);
        DemPrint(hprinter,1,1);
    }
}

void MainWindow::printer_close(){
    DemoPortClose(hprinter);
    DemoPrinterDestroy(hprinter);
}


QImage MainWindow::Mat2Qt(const cv::Mat &src){
    QImage img;
    if(src.channels() == 1){
        //Gray
        img = QImage((const unsigned char*)src.data, src.cols, src.rows, src.cols, QImage::Format_Grayscale8);
    }else{
        //BGR
        cvtColor(src,src,cv::COLOR_BGR2RGB);
        img = QImage((uchar*)src.data, src.cols, src.rows, src.step, QImage::Format_RGB888);

    }
    return img;
}

cv::Mat MainWindow::Qt2Mat(const QImage &src){
    switch (src.format() )
    {
    // 8-bit, 4 channel
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat  mat( src.height(), src.width(),
                      CV_8UC4,
                      const_cast<uchar*>(src.bits()),
                      static_cast<size_t>(src.bytesPerLine())
                      );

        return  mat.clone();
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB32:
    {

        cv::Mat  mat( src.height(), src.width(),
                  CV_8UC4,
                  const_cast<uchar*>(src.bits()),
                  static_cast<size_t>(src.bytesPerLine())
                  );

        cv::Mat  matNoAlpha;

        cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

        return matNoAlpha;
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
        QImage   swapped = src.rgbSwapped();

        return cv::Mat( swapped.height(), swapped.width(),
                    CV_8UC3,
                    const_cast<uchar*>(swapped.bits()),
                    static_cast<size_t>(swapped.bytesPerLine())
                    ).clone();
    }

        // 8-bit, 1 channel
    case QImage::Format_Grayscale8:
    case QImage::Format_Indexed8:
    {
        cv::Mat  mat( src.height(), src.width(),
                  CV_8UC1,
                  const_cast<uchar*>(src.bits()),
                  static_cast<size_t>(src.bytesPerLine())
                  );

        return mat.clone() ;
    }
    default:
        qDebug() << "没有专门所属类型，请完善！！！" << src.format()<<endl;
        break;
    }

    return cv::Mat();
}

cv::Mat MainWindow::Resizeimg(cv::Mat src,int w,int h){
    cv::Mat resize_img;
    double img_w=src.cols;//列数_宽
    double img_h=src.rows;//行数_高
    double ratio=img_h/img_w;

    double new_w;
    double new_h;

    if(ratio>=0){
        new_h=int(h);
        new_w=int(new_h/ratio);
    }
    else {
        new_w=int(w);
        new_h=int(new_w*ratio);

    }
    try {
        cv::resize(src,resize_img,cv::Size(new_w,new_h));
    } catch (...) {
        return resize_img;
    }
    return resize_img;
}

RotatedRect MainWindow::RotatedRect_trans(RotatedRect box,double angel){
    Point2f center=box.center;
    Size2f size=box.size;
    float ang=box.angle+angel;
    RotatedRect box_trans=RotatedRect(center,size,ang);
    return box_trans;
}

Mat MainWindow::Painter_roi(QList<RotatedRect> box,Mat src){
    int color=0;
    if(box.size()>0){
        for(RotatedRect block:box)
        {
            if(color>255){
                color=0;
            }
            Point2f pts[4];
            block.points(pts);
            for (int i = 0; i < 4; i++)
            {
                if (i == 3)
                {
                        line(src, pts[i], pts[0], Scalar(255, 0, 0),3,LINE_AA);
                        break;
                }
                line(src,pts[i], pts[i + 1], Scalar(255, 0, 0),3,LINE_AA);
            }
            color=color+50;
        }
    }
    return src;
}

float MainWindow::Get_box_iou(RotatedRect box1, RotatedRect box2){
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

void MainWindow::muti_thread(){
    thread=new ROIQThread(this);
    connect(thread,SIGNAL(edge_message(QList<RotatedRect>)),this,SLOT(edge_result(QList<RotatedRect>)),Qt::DirectConnection);
    connect(thread,SIGNAL(ocr_message(QList<QString>)),this,SLOT(ocr_result(QList<QString>)),Qt::DirectConnection);
    connect(thread,&ROIQThread::ocr_bar,this,&MainWindow::ocr_bar);
    connect(thread,&ROIQThread::at_message,this,&MainWindow::at_set);
}

void MainWindow::ocr_bar(){
    int value=ui->ocr_progressBar->value();
    for (int i=0;i<51;i=i+10) {
        ui->ocr_progressBar->setValue(value+i);
    }
    if(value==0){
        ui->label_ocrbar->setText("开始识别");
        ui->label_ocrbar->setStyleSheet("background-color:#8E2323;border: 3px solid #8E2323;");
    }
    if(value==50){
        ui->label_ocrbar->setText("识别完毕");
        ui->label_ocrbar->setStyleSheet("background-color:#70a19f;border: 3px solid #70a19f;");
    }
}

void MainWindow::set_maintag(){
    QList<RLabel*> labellist=tagpage->findChildren<RLabel*>();
    for(RLabel* index:labellist){
        delete index;
    }
    QSettings config_page("temp.ini",QSettings::IniFormat);
    config_page.beginGroup("page");
    tagw=config_page.value("page_w").toDouble();
    tagh=config_page.value("page_h").toDouble();
    config_page.endGroup();
    double ratiow=tagw/80;
    double ratioh=tagh/80;
    double tagwidget_w=ui->tag_label->width();
    double tagwidget_h=ui->tag_label->height();
    double new_w=tagwidget_w*ratiow;
    double new_h=tagwidget_w*ratioh;
    QRect rect(QPoint((tagwidget_w-new_w)/2,(tagwidget_h-new_h)/2),QSize(new_w,new_h));
    tagpage->setGeometry(rect);
    tagpage->setStyleSheet("border:0 px;background:white");
    tagpage->show();
    for(QString groupname:config_page.childGroups()){
        if(groupname=="page"){
            continue;
        }
        RLabel* text=new RLabel(tagpage);
        text->setObjectName(groupname);
        config_page.beginGroup(groupname);
        Rlabelfont.setFamily(config_page.value("font").toString());
        Rlabelfont.setPointSize(config_page.value("fontsize").toInt());
        double rx=config_page.value("rx").toDouble();
        double ry=config_page.value("ry").toDouble();
        double rw=config_page.value("rw").toDouble();
        double rh=config_page.value("rh").toDouble();
        int x=tagpage->width()*rx;
        int y=tagpage->height()*ry;
        int w=tagpage->width()*rw;
        int h=tagpage->height()*rh;
        QRect rect(QPoint(x,y),QSize(w,h));
        text->setGeometry(rect);
        config_page.endGroup();
        text->setText(groupname);
        text->show();
        text->compute_r_geometry(new_w,new_h);
        text->setFont(Rlabelfont);
    }
}

void MainWindow::reset_barcode(){
    RLabel* text=tagpage->findChild<RLabel*>("barcode");
    Code=ui->pid_lineEdit->text();
    //pos=index.indexOf(QRegExp("[:：]"));
    QByteArray ba = Code.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if((*s>='A' && *s<='Z') || (*s>='a' && *s<='z') || (*s>='0' && *s<='9'))
        {

        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    if(!bret){
        text->clear();
        return;
    }
    if(Code=="219025300F"){
        Code="";
    }
    if(text){
        update();
        Code128 bc=Code128(Code,"tempbarcode");
        bc.start();
        QImage *img;
        img=new QImage;
        img->load("tempbarcode.jpg");
        img->scaled(text->size(),Qt::KeepAspectRatio);
        text->setScaledContents(true);
        text->setPixmap(QPixmap::fromImage(*img));
    }
}

void MainWindow::resizeEvent(QResizeEvent *event){
    double ratio_w=tagw/80;
    double ratio_h=tagh/80;
    double tagwidget_w=ui->tag_label->width();
    double tagwidget_h=ui->tag_label->height();
    double new_w=tagwidget_w*ratio_w;
    double new_h=tagwidget_w*ratio_h;
    QRect tag_rect(QPoint((tagwidget_w-new_w)/2,(tagwidget_h-new_h)/2),QSize(new_w,new_h));
    tagpage->setGeometry(tag_rect);
    for(RLabel* index:tagpage->findChildren<RLabel*>()){
        double x=(index->rx)*new_w;
        double y=(index->ry)*new_h;
        double w=(index->rw)*new_w;
        double h=(index->rh)*new_h;
        QRect label_rect(QPoint(x,y),QSize(w,h));
        index->setGeometry(label_rect);
    }
}

void MainWindow::uploadimg(QString filename){
    camtimer->stop();
    turn->show();
    if(!filename.isEmpty())
    {
        QImage img;
        if(!(img.load(filename))) //加载图像
        {
            QMessageBox::information(this, tr("提示"),tr("打开图像失败!"));
            return;
        }
        string path=filename.toStdString();
        Mat temp_img=imread(path);
        double w_g=temp_img.cols;
        double h_g=temp_img.rows;

        double w=ui->Cam_label->width();
        double h=ui->Cam_label->height();

        double ratio1=w/h;
        double ratio2=w_g/h_g;

        try{
            if(ratio2 < ratio1){
                cv::resize(temp_img,temp_img,Size(ratio2*h,h));
            }
            else
            {
                cv::resize(temp_img,temp_img,Size(w,w/ratio2));
            }
        }
        catch(exception e){
            QMessageBox::information(this, tr("tips"),tr("图片文件名称不能存在中文，请修改!"));
            return;
        }
        Mat displayimg=temp_img.clone();
        QImage dst1=Mat2Qt(temp_img);
        ui->Cam_label->setPixmap(QPixmap::fromImage(dst1));
        QList<RotatedRect> bboxes;
        bboxes=roitransform_extraction(bboxes,temp_img,0);//roitransform识别
        //bboxes=Erode_extraction(temp_img);
        Mat re_img=perspective_transform(bboxes[0],temp_img);
        displayimg=Painter_roi(bboxes,displayimg);
        QImage dst2=Mat2Qt(displayimg);
        ui->Cam_label->setPixmap(QPixmap::fromImage(dst2));
        ui->ocr_textBrowser->clear();
        thread->setmodel(1,re_img);
        ui->ocr_progressBar->setValue(0);
        ocr_bar();
        thread->start();
    }
    else {
        return;
    }
}

void MainWindow::turnCam(){
    camtimer->start(50);
    turn->hide();
}

void MainWindow::pushturncam(){
    turn=new QPushButton(ui->Cam_label);
    QRect rect(QPoint(0,0),QSize(10,20));
    turn->setGeometry(rect);
    turn->setObjectName("返回相机模式");
    turn->setText("turnCam");
    turn->adjustSize();
    connect(turn,&QPushButton::clicked,this,&MainWindow::turnCam);
    QMetaObject::connectSlotsByName(this);
    turn->hide();
}

void MainWindow::at_set(QString strwbpm,QString strwbkh,QString strys,QString strcm,QString strgender,QString strzypl,QString strgys,QString strlxhh,QString strpid){
    ui->ocr_textBrowser->update();
    if(strpid==QString::fromUtf8("无")){
        ui->wbpm_lineEdit->setText(strwbpm);
        ui->wbkh_lineEdit->setText(strwbkh);
        ui->color_lineEdit->setText(strys);
        ui->size_lineEdit->setText(strcm);
        ui->gender_lineEdit->setText(QString::fromUtf8("无"));
        ui->zypl_lineEdit->setText(QString::fromUtf8("无"));
        ui->gys_lineEdit->setText(QString::fromUtf8("无"));
        ui->lxhh_lineEdit->setText(QString::fromUtf8("无"));
        ui->pid_lineEdit->setText(QString::fromUtf8("无"));
        QMessageBox::information(this,"tips",QString::fromUtf8("未查询到该商品"));
        return;
    }
    ui->wbpm_lineEdit->setText(strwbpm);

    ui->wbkh_lineEdit->setText(strwbkh);

    ui->color_lineEdit->setText(strys);

    ui->size_lineEdit->setText(strcm);

    ui->gender_lineEdit->setText(strgender);

    ui->zypl_lineEdit->setText(strzypl);

    ui->gys_lineEdit->setText(strgys);

    ui->lxhh_lineEdit->setText(strlxhh);

    ui->pid_lineEdit->setText(strpid);

}

void MainWindow::changeuseapi(){
    qDebug()<<"changeuseapi";
    ui->gender_lineEdit->clear();
    ui->zypl_lineEdit->clear();
    ui->gys_lineEdit->clear();
    ui->lxhh_lineEdit->clear();
    ui->pid_lineEdit->clear();
    get_apitoken();
    webapi_listbywbqk();
}

void MainWindow::initwebapi(){
    QSettings setting("./webAPI_config.ini",QSettings::IniFormat);
    //设置api参数
    setting.beginGroup("mainpath");
    mainpath=setting.value("mainpath").toString();
    setting.endGroup();

    setting.beginGroup("token");
    username=setting.value("username").toString();
    password=setting.value("password").toString();
    token_api=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("Listeqkeybywb");
    listeqkey=setting.value("api").toString();
    setting.endGroup();
}

void MainWindow::get_apitoken(){
    QJsonObject body;
    body.insert("account",username);
    body.insert("password",password);
    QJsonDocument doc;
    doc.setObject(body);
    QByteArray arr = doc.toJson(QJsonDocument::Compact);
    QNetworkRequest request;
    QUrl url;
    url.setUrl(token_api);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkReply* reply=WebSe->post(request,arr);
    //开启事件循环，直到请求完成
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
    int iError = reply->error();
    if (iError == QNetworkReply::NoError)
    {
        QVariant data = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument document=QJsonDocument::fromJson(data.toByteArray(),&jsonError);
        if(!document.isNull()&&(jsonError.error==QJsonParseError::NoError))
        {
            QString data=document["data"].toString();
            checkvalue="Authorization="+data.toUtf8();
        }
    }
    else
    {
        qDebug()<<"get_token_false";
    }
    reply->deleteLater();
}

void MainWindow::webapi_listbywbqk(){
    QString pid="无";
    QString gender="无";
    QString zypl="无";
    QString gys="无";
    QString lxhh="无";
    QTextCodec *codec = QTextCodec::codecForName("gbk");
    QByteArray byte = codec->fromUnicode(str2);
    byte = byte.toPercentEncoding();
    QString str=mainpath+listeqkey+"?wpname="+byte+"&wpvalue="+str1;
    QUrl url(str);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    request.setRawHeader("Cookie",checkvalue);
    QNetworkReply* reply=WebSe->get(request);
    //开启事件循环，直到请求完成
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
    int iError = reply->error();
    if (iError == QNetworkReply::NoError)
    {
        QVariant data = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument document=QJsonDocument::fromJson(data.toByteArray(),&jsonError);
        if(!document.isNull()&&(jsonError.error==QJsonParseError::NoError))
        {
            QJsonValue dataval=document["data"];   
            QJsonArray dataarry=dataval.toArray();
            if(dataarry.size()>0){
                for (auto sds=dataarry.begin();sds!=dataarry.end();sds++)
                {
                    QJsonObject dataobj=sds->toObject();
                    pid=dataobj["id"].toString();
                    gender=dataobj["mdim16idattribname"].toString();
                    zypl=dataobj["mdim4idattribname"].toString();
                    gys=dataobj["suppliername"].toString();
                    lxhh=dataobj["name"].toString();
                    ui->gender_lineEdit->setText(gender);
                    ui->zypl_lineEdit->setText(zypl);
                    ui->gys_lineEdit->setText(gys);
                    ui->lxhh_lineEdit->setText(lxhh);
                    ui->pid_lineEdit->setText(pid);
                    reply->deleteLater();
                    WebSe->clearAccessCache();
                    WebSe->clearConnectionCache();
                    return;
                }
            }
            else {
                ui->gender_lineEdit->setText(gender);
                ui->zypl_lineEdit->setText(zypl);
                ui->gys_lineEdit->setText(gys);
                ui->lxhh_lineEdit->setText(lxhh);
                ui->pid_lineEdit->setText(pid);
                QMessageBox::information(this,"tips",QString::fromUtf8("未查询到该商品！"));
            }
        }
    }
    else
    {
        qDebug()<<"访问失败";
    }
    reply->deleteLater();
    WebSe->clearAccessCache();
    WebSe->clearConnectionCache();
}
