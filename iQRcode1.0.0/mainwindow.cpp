/***********************************
*  File: mainwindow.cpp
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include "mmdeploy/rotated_detector.h"

QString filepath="./temporary.ini";
MainWindow* mywin;
int barindex=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mywin=this;
    codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    ui->CN_pushButton->setStyleSheet("background:red");
    textfont.setPointSize(7);
    ui->textBrowser->setFont(textfont);
    ui->textBrowser->setStyleSheet("border:4px solid gray ");
    ui->Cam_label->setStyleSheet("border:4px solid gray ");
    isCam=true;
    timer = new QTimer(this);
    PMtimer = new QTimer(this);
    noprint=true;
    this->initCamera();
    this->initPrinter();
    this->set_page_widget();
    ui->label_bar->installEventFilter(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ReadFrame()));
    connect(PMtimer,SIGNAL(timeout()),this,SLOT(PM_query()));
    WebSe=new QNetworkAccessManager(this);
    QLabel *label_permanent = new QLabel(this);
    label_permanent->setOpenExternalLinks(true);
    label_permanent->setFrameStyle(QFrame::Box|QFrame::Sunken);
    label_permanent->setText("<a style='color: blue;' href=\"https://faculty.xidian.edu.cn/LJ32/zh_CN/index.htm\">Powered by Xidian Univ.iMIA Group(Jun Li).2022");
    ui->statusbar->addPermanentWidget(label_permanent);
    ui->statusbar->setStyleSheet("border:0px;");
    initiation_api();
    all_doc=set_menu();
    //21项属性
    doc_set_QcomboBox("BX_comboBox");
    doc_set_QcomboBox("CCZ_comboBox");
    doc_set_QcomboBox("CGXZ_comboBox");
    doc_set_QcomboBox("CGY_comboBox");
    doc_set_QcomboBox("CKG_comboBox");
    doc_set_QcomboBox("CM_comboBox");
    doc_set_QcomboBox("DL_comboBox");
    doc_set_QcomboBox("GYS_comboBox");
    doc_set_QcomboBox("JJ_comboBox");
    doc_set_QcomboBox("JYFS_comboBox");
    doc_set_QcomboBox("LXSH_comboBox");
    doc_set_QcomboBox("NLD_comboBox");
    doc_set_QcomboBox("PP_comboBox");
    doc_set_QcomboBox("SL_comboBox");
    doc_set_QcomboBox("XB_comboBox");
    doc_set_QcomboBox("XL_comboBox");
    doc_set_QcomboBox("YS_comboBox");
    doc_set_QcomboBox("ZL_comboBox");
    doc_set_QcomboBox("ZTJ_comboBox");
    doc_set_QcomboBox("ZYPL_comboBox");
    doc_set_QcomboBox("NF_comboBox");
    QJsonValue dataval=all_doc["update"];
    QString title_str=dataval.toString();
    ui->title_label->setText("智能编码系统---属性菜单更新时间:"+title_str);
}

MainWindow::~MainWindow()
{
    delete ui;
}

const char * MainWindow::QString2char(QString str)
{
    QByteArray ba = str.toLocal8Bit();
    const char * data = ba.data();
    return data;
}

void MainWindow::on_CN_pushButton_clicked()
{
    noprint=false;
    const char * name = "HT130";
    const char * port = "USB";
    int result = -1;
    int status = -1;
    result = DemPrinterCreator(&hprinter,name);
    result = DemPortOpen(hprinter,port);
    result = DemStatus(hprinter,&status);
    if (status==0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("汉印N41打印机成功连接"));
        ui->CN_pushButton->setText(QString::fromLocal8Bit("已连接"));
        ui->CN_pushButton->setStyleSheet("");
    }
    else
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("汉印N41打印机连接失败"));
    DemDirection(hprinter,1);
}

void MainWindow::on_PR_pushButton_clicked()
{
    if(noprint)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请连接打印机"));
        return;
    }
    DemClearBuffer(hprinter);
    int speed=2;
    int density=14;
    int type=0;
    int gap=0;
    int offset=0;
    DemSetup(hprinter,printmap["page_w"],printmap["page_h"],speed,density,type,gap,offset);
    QPixmap pixmap = QPixmap::grabWidget(ui->page_widget);
    QSize picSize(ui->page_widget->width()*1.5,ui->page_widget->height()*1.5);
    pixmap=pixmap.scaled(picSize,Qt::KeepAspectRatio);
    pixmap.save("pt.jpg","JPG");
    DemBitMap(hprinter,0,0,0,"pt.jpg");
    DemPrint(hprinter,1,1);

}

void MainWindow::on_ST_pushButton_clicked()
{
    if(noprint)
    {
        return;
    }
    DemPortClose(hprinter);
    DemPrinterDestroy(hprinter);
    QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("打印机成功断开连接"));
    ui->CN_pushButton->setText(QString::fromLocal8Bit("请连接打印机"));
    ui->CN_pushButton->setStyleSheet("background:red");
    noprint=true;
}


void MainWindow::on_dpage_pushButton_clicked()
{
    design_page=new dpage;
    design_page->show();
}

void MainWindow::on_Cam_comboBox_activated(int index)
{
    timer->stop();//停止计时器
    index = ui->Cam_comboBox->currentIndex();//获取当前摄像头对应ID
    //qDebug()<<"Index"<< index <<": "<< ui->Cam_comboBox->currentText();
    capture.release();//释放初始摄像头
    capture.open(index);//打开选择的摄像头
    capture.set(CV_CAP_PROP_FRAME_WIDTH,421);;//设置分辨率适配当前展示框
    timer->start(50);//计时器开始
}

QString MainWindow::get_font(int index)
{
    QString font;
    if(index==0){
        font="SimSun";
        return font;
    }
    else if (index==1) {
        font="SimHei";
        return font;
    }
    else if (index==2) {
        font="KaiTi";
        return font;
    }
    else
        font="SimSun";
        return font;
}

void MainWindow::label_set(int at, int font, int size,QString label)
{

    auto label_ind=findChild<QLabel*>(label);
    atmap[label]=at;
    QFont setfont;
    QString check_font;
    check_font=get_font(font);
    setfont.setFamily(check_font);
    setfont.setPixelSize(size);
    label_ind->setFont(setfont);
    if(at==1)
    {
        QString text=QString::fromLocal8Bit("品名:")+ui->PM_comboBox->currentText();
        label_ind->setText(text);
    }
    if (at==2)
    {
        QString text=QString::fromLocal8Bit("主营品类:")+ui->ZYPL_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==3)
    {
        QString text=QString::fromLocal8Bit("货号:")+ui->HH_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==4)
    {
        QString text=QString::fromLocal8Bit("品牌:")+ui->PP_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==5)
    {
        QString text=QString::fromLocal8Bit("四类名称:")+ui->SL_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==6)
    {
        QString text=QString::fromLocal8Bit("大类：")+ui->DL_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==7)
    {
        QString text=QString::fromLocal8Bit("中类:")+ui->ZL_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==8)
    {
        QString text=QString::fromLocal8Bit("小类:")+ui->XL_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==9)
    {
        QString text=QString::fromLocal8Bit("性别:")+ui->XB_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==10)
    {
        QString text=QString::fromLocal8Bit("颜色:")+ui->YS_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==11)
    {
        QString text=QString::fromLocal8Bit("供应商:")+ui->GYS_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==12)
    {
        QString text=QString::fromLocal8Bit("正特价:")+ui->ZTJ_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==13)
    {
        QString text=QString::fromLocal8Bit("季节:")+ui->JJ_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==14)
    {
        QString text=QString::fromLocal8Bit("采购员:")+ui->CGY_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==15)
    {
        QString text=QString::fromLocal8Bit("年龄段:")+ui->NLD_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==16)
    {
        QString text=QString::fromLocal8Bit("年份:")+ui->NF_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==17)
    {
        QString text=QString::fromLocal8Bit("尺寸组:")+ui->CCZ_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==18)
    {
        QString text=QString::fromLocal8Bit("采购性质:")+ui->CGXZ_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==19)
    {
        QString text=QString::fromLocal8Bit("版型:")+ui->BX_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==20)
    {
        QString text=QString::fromLocal8Bit("长宽高:")+ui->CKG_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==21)
    {
        QString text=QString::fromLocal8Bit("联系商户:")+ui->LXSH_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==22)
    {
        QString text=QString::fromLocal8Bit("尺码:")+ui->CM_comboBox->currentText();
        label_ind->setText(text);
    }

    if (at==23)
    {
        QString text=QString::fromLocal8Bit("经营方式:")+ui->JYFS_comboBox->currentText();
        label_ind->setText(text);
    }


}

void MainWindow::label_align(int index,QString label){
    auto ind = findChild<QLabel*>(label);
    if(index==0)
    {
        ind->setAlignment(Qt::AlignLeft);
    }
    else if(index==1)
    {
        ind->setAlignment(Qt::AlignCenter);
    }
    else if(index==2)
    {
        ind->setAlignment(Qt::AlignRight);
    }
}

void MainWindow::set_page_widget()
{
    qDebug()<<QString::fromLocal8Bit("当前使用标签路径:")<<filepath;
    QSettings setting(filepath,QSettings::IniFormat);
    //设置页面大小
    setting.beginGroup("page");
    int w=setting.value("page_w").toInt();
    int h=setting.value("page_h").toInt();
    //将标签设置参数传入printmap字典中
    printmap["page_w"]=w;
    printmap["page_h"]=h;
    int x_offset=(291-w*5)/2;
    int y_offset=(341-h*5)/2;
    ui->page_widget->setGeometry(x_offset,y_offset,w*5,h*5);
    ui->page_widget->setStyleSheet("background:white");
    setting.endGroup();

    setting.beginGroup("label_ck1");
    //获取相关参数
    QPoint point_ck1=setting.value("start_point").toPoint();
    printmap["label_ck1_x"]=point_ck1.x();
    printmap["label_ck1_y"]=point_ck1.y();
    printmap["label_ck1_w"]=setting.value("label_w").toInt();
    printmap["label_ck1_h"]=setting.value("label_h").toInt();
    printmap["label_ck1_at"]=setting.value("attribute").toInt();
    printmap["label_ck1_font"]=setting.value("font").toInt();
    printmap["label_ck1_size"]=setting.value("font_size").toInt();
    printmap["label_ck1_align"]=setting.value("align").toInt();
    setting.endGroup();

    setting.beginGroup("label_ck2");
    //获取相关参数
    QPoint point_ck2=setting.value("start_point").toPoint();
    printmap["label_ck2_x"]=point_ck2.x();
    printmap["label_ck2_y"]=point_ck2.y();
    printmap["label_ck2_w"]=setting.value("label_w").toInt();
    printmap["label_ck2_h"]=setting.value("label_h").toInt();
    printmap["label_ck2_at"]=setting.value("attribute").toInt();
    printmap["label_ck2_font"]=setting.value("font").toInt();
    printmap["label_ck2_size"]=setting.value("font_size").toInt();
    printmap["label_ck2_align"]=setting.value("align").toInt();
    setting.endGroup();

    setting.beginGroup("label_ck3");
    //获取相关参数
    QPoint point_ck3=setting.value("start_point").toPoint();
    printmap["label_ck3_x"]=point_ck3.x();
    printmap["label_ck3_y"]=point_ck3.y();
    printmap["label_ck3_w"]=setting.value("label_w").toInt();
    printmap["label_ck3_h"]=setting.value("label_h").toInt();
    printmap["label_ck3_at"]=setting.value("attribute").toInt();
    printmap["label_ck3_font"]=setting.value("font").toInt();
    printmap["label_ck3_size"]=setting.value("font_size").toInt();
    printmap["label_ck3_align"]=setting.value("align").toInt();
    setting.endGroup();

    setting.beginGroup("label_ck4");
    //获取相关参数
    QPoint point_ck4=setting.value("start_point").toPoint();
    printmap["label_ck4_x"]=point_ck4.x();
    printmap["label_ck4_y"]=point_ck4.y();
    printmap["label_ck4_w"]=setting.value("label_w").toInt();
    printmap["label_ck4_h"]=setting.value("label_h").toInt();
    printmap["label_ck4_at"]=setting.value("attribute").toInt();
    printmap["label_ck4_font"]=setting.value("font").toInt();
    printmap["label_ck4_size"]=setting.value("font_size").toInt();
    printmap["label_ck4_align"]=setting.value("align").toInt();
    setting.endGroup();

    setting.beginGroup("label_ck5");
    //获取相关参数
    QPoint point_ck5=setting.value("start_point").toPoint();
    printmap["label_ck5_x"]=point_ck5.x();
    printmap["label_ck5_y"]=point_ck5.y();
    printmap["label_ck5_w"]=setting.value("label_w").toInt();
    printmap["label_ck5_h"]=setting.value("label_h").toInt();
    printmap["label_ck5_at"]=setting.value("attribute").toInt();
    printmap["label_ck5_font"]=setting.value("font").toInt();
    printmap["label_ck5_size"]=setting.value("font_size").toInt();
    printmap["label_ck5_align"]=setting.value("align").toInt();
    setting.endGroup();

    setting.beginGroup("label_bar");
    printmap["label_bar_ischeck"]=setting.value("ischeck").toInt();
    QPoint point_bar=setting.value("start_point").toPoint();
    printmap["label_bar_x"]=point_bar.x();
    printmap["label_bar_y"]=point_bar.y();
    printmap["label_bar_w"]=setting.value("label_w").toInt();
    printmap["label_bar_h"]=setting.value("label_h").toInt();
    printmap["label_bar_align"]=setting.value("align").toInt();
    setting.endGroup();

    setting.beginGroup("label_data");
    printmap["label_data_ischeck"]=setting.value("ischeck").toInt();
    QPoint point_data=setting.value("start_point").toPoint();
    printmap["label_data_x"]=point_data.x();
    printmap["label_data_y"]=point_data.y();
    printmap["label_data_w"]=setting.value("label_w").toInt();
    printmap["label_data_h"]=setting.value("label_h").toInt();
    printmap["label_data_align"]=setting.value("align").toInt();
    setting.endGroup();

    if(printmap["label_ck1_at"]!=0)
    {
        ui->label_ck1->setGeometry(printmap["label_ck1_x"],printmap["label_ck1_y"],printmap["label_ck1_w"],printmap["label_ck1_h"]);
        label_align(printmap["label_ck1_align"],"label_ck1");
        label_set(printmap["label_ck1_at"],printmap["label_ck1_font"],printmap["label_ck1_size"],"label_ck1");

    }
    else if (printmap["label_ck1_at"]==0) {
        ui->label_ck1->clear();
        atmap["label_ck1"]=0;
    }

    if(printmap["label_ck2_at"]!=0)
    {
        ui->label_ck2->setGeometry(printmap["label_ck2_x"],printmap["label_ck2_y"],printmap["label_ck2_w"],printmap["label_ck2_h"]);
        label_align(printmap["label_ck2_align"],"label_ck2");
        label_set(printmap["label_ck2_at"],printmap["label_ck2_font"],printmap["label_ck2_size"],"label_ck2");
    }
    else if (printmap["label_ck2_at"]==0) {
        ui->label_ck2->clear();
        atmap["label_ck2"]=0;
    }

    if(printmap["label_ck3_at"]!=0)
    {
        ui->label_ck3->setGeometry(printmap["label_ck3_x"],printmap["label_ck3_y"],printmap["label_ck3_w"],printmap["label_ck3_h"]);
        label_align(printmap["label_ck3_align"],"label_ck3");
        label_set(printmap["label_ck3_at"],printmap["label_ck3_font"],printmap["label_ck3_size"],"label_ck3");
    }
    else if (printmap["label_ck3_at"]==0) {
        ui->label_ck3->clear();
        atmap["label_ck3"]=0;
    }

    if(printmap["label_ck4_at"]!=0)
    {
        ui->label_ck4->setGeometry(printmap["label_ck4_x"],printmap["label_ck4_y"],printmap["label_ck4_w"],printmap["label_ck4_h"]);
        label_align(printmap["label_ck4_align"],"label_ck4");
        label_set(printmap["label_ck4_at"],printmap["label_ck4_font"],printmap["label_ck4_size"],"label_ck4");
    }
    else if (printmap["label_ck4_at"]==0) {
        ui->label_ck4->clear();
        atmap["label_ck4"]=0;
    }

    if(printmap["label_ck5_at"]!=0)
    {
        ui->label_ck5->setGeometry(printmap["label_ck5_x"],printmap["label_ck5_y"],printmap["label_ck5_w"],printmap["label_ck5_h"]);
        label_align(printmap["label_ck5_align"],"label_ck5");
        label_set(printmap["label_ck5_at"],printmap["label_ck5_font"],printmap["label_ck5_size"],"label_ck5");
    }
    else if (printmap["label_ck5_at"]==0) {
        ui->label_ck5->clear();
        atmap["label_ck5"]=0;
    }

    if(printmap["label_bar_ischeck"]!=0)
    {
        bar_ischeck=true;
        ui->label_bar->setGeometry(printmap["label_bar_x"],printmap["label_bar_y"],printmap["label_bar_w"],printmap["label_bar_h"]);
        label_align(printmap["label_bar_align"],"label_bar");
    }
    else if (printmap["label_bar_ischeck"]==0) {
        bar_ischeck=false;
        ui->label_bar->clear();
    }

    if(printmap["label_data_ischeck"]!=0)
    {
        data_ischeck=true;
        ui->label_data->setGeometry(printmap["label_data_x"],printmap["label_data_y"],printmap["label_data_w"],printmap["label_data_h"]);
        label_align(printmap["label_data_align"],"label_data");
    }
    else if (printmap["label_data_ischeck"]==0) {
        data_ischeck=false;
        ui->label_data->clear();
    }
}




void MainWindow::on_test_pushButton_clicked()
{

    QFile file("./sql.json");
    file.open(QIODevice::WriteOnly);
    int sl_size = sl_arry.size();
    for(int i=0; i<sl_size; i++)
    {
         sl_arry.removeFirst();
    }
    //get_token();
    set_QComboBox(pplist, "PP_comboBox");//品牌
    set_QComboBox(sexlist, "XB_comboBox");//性别
    set_QComboBox(yearlist, "NF_comboBox");//年份
    set_QComboBox(supplier_list, "GYS_comboBox");//供应商
    set_QComboBox(seasonlist, "JJ_comboBox");//季节
    set_QComboBox(mDimlist, "ZYPL_comboBox");//主营品类
    set_QComboBox(dllist, "DL_comboBox");//大类
    set_QComboBox(mediumlist, "ZL_comboBox");//中类
    set_QComboBox(smalllist, "XL_comboBox");//小类
    set_QComboBox(speciallist, "ZTJ_comboBox");//正特价
    set_QComboBox(operationlist, "JYFS_comboBox");//经营方式
    set_QComboBox(merchantlist, "LXSH_comboBox");//联系商户
    set_QComboBox(agelist, "NLD_comboBox");//年龄段
    set_QComboBox(cgxzlist, "CGXZ_comboBox");//采购性质
    set_QComboBox(buyerlist, "CGY_comboBox");//采购员
    set_QComboBox(versionlist, "BX_comboBox");//版型
    set_QComboBox(lengthlist, "CKG_comboBox");//长宽高
    set_QComboBox(sizelist, "CM_comboBox");//尺码
    set_QComboBox(colorlist, "YS_comboBox");//颜色
    set_QComboBox(cczlist, "CCZ_comboBox");//尺寸组
    query_sl("春");
    query_sl("夏");
    query_sl("秋");
    query_sl("冬");
    query_sl("四季");
    QDateTime timeDate = QDateTime::currentDateTime();
    QString timeStr = timeDate.toString("[yy-MM-dd]");
    all_json.insert("update",timeStr);
    all_json.insert("SL_comboBox",sl_arry);
    QJsonDocument temp_doc;
    temp_doc.setObject(all_json);
    QByteArray temp_data = temp_doc.toJson();
    file.write(temp_data);
    file.close();
    QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("菜单更新完成"));
    ui->title_label->setText("智能编码系统---属性菜单更新时间:"+timeStr);
}


void MainWindow::on_id_pushButton_clicked()
{
    get_id();
    PM_query();
}

void MainWindow::upload_img_test(QString fileName,MainWindow* mywindow){
    mywin->ui->rec_progressBar->setValue(0);
    QImage img;
    if(!(img.load(fileName))) //加载图像
    {
        QMessageBox::information(this, tr("打开图像失败"),tr("打开图像失败!"));
        return;
    }
    string path=fileName.toStdString();
    Mat temp_img=imread(path);
    Mat img_test=imread(path);
    double w_g=temp_img.cols;
    double h_g=temp_img.rows;

    double w=mywindow->ui->Cam_label->width();
    double h=mywindow->ui->Cam_label->height();

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

    QImage dst1=Mat2Qt(temp_img);
    mywindow->ui->Cam_label->setPixmap(QPixmap::fromImage(dst1));
    set_recbar_value();//1
    mywindow->timer->stop();

    mywindow->isCam=false;
    mywindow->ui->img_pushButton->setText("切换至摄像头");
    set_recbar_value();//2
    QList<RotatedRect> bboxes;
    bboxes=edge_extraction(temp_img);
    set_recbar_value();//3
    bboxes=roi_main(bboxes,temp_img,0);
    set_recbar_value();//4
    bboxes=roi_main(bboxes,temp_img,15);
    set_recbar_value();//5
    bboxes=roi_main(bboxes,temp_img,-15);
    set_recbar_value();//6
    bboxes=frame_pro(temp_img,bboxes,0);

    bboxes=frame_pro(temp_img,bboxes,15);

    bboxes=frame_pro(temp_img,bboxes,-15);
    set_recbar_value();//7

    if(bboxes.size()>0){
        if(bboxes.size()>1){
            QList<float> iou;
            for (int i=0;i<bboxes.size();i++) {
                int j=0;
                float temp_iou=0.00;
                for (;j<bboxes.size();j++) {
                    float temp=get_box_iou(bboxes[i],bboxes[j]);//计算两个box的iou值
                    temp_iou+=temp;
                }
                iou.push_back(temp_iou);
            }
            float max=iou.at(0);
            int max_idx=0;
            for (int idx=0;idx<iou.size();++idx) {
                if(max<iou.at(idx)){
                    max=iou.at(idx);
                    max_idx=idx;
                }
            }

            float def;
            int sec_idx;
            if(max_idx>0){
                def=abs(max-iou.at(0));
                sec_idx=0;
            }

            else {
                def=abs(max-iou.at(1));
                sec_idx=1;
            }

            for (int idx=0;idx<iou.size();++idx) {

                if(abs(max-iou.at(idx))>0 && abs(max-iou.at(idx))<def){
                    def=abs(max-iou.at(idx));
                    sec_idx=idx;
                }
            }
            set_recbar_value();//8
            qDebug()<<max_idx<<sec_idx;
            RotatedRect box_maxiou=bboxes.at(max_idx);
            RotatedRect box_seciou=bboxes.at(sec_idx);
            float bx=box_maxiou.center.x;
            float by=box_maxiou.center.y;
            float bw=0.7*box_maxiou.size.width+0.3*box_seciou.size.width;
            float bh=0.7*box_maxiou.size.height+0.3*box_seciou.size.height;
            float ba=0.8*box_maxiou.angle+0.2*box_seciou.angle;
            if(abs(ba)>15.00 && abs(ba)<75.00){
                double df_15=abs(ba)-15.00;
                double df_75=75-abs(ba);
                if(ba>0 && df_15<df_75){
                    ba=15.00;
                }
                else if(ba<0 && df_15<df_75){
                    ba=-15.00;
                }
                else if(ba>0 && df_15>df_75){
                    ba=75.00;
                }
                else if (ba<0 && df_15>df_75) {
                    ba=-75.00;
                }
            }
            RotatedRect result=RotatedRect(Point2f(bx, by), Size2f(bw, bh), ba);
            qDebug()<<box_maxiou.center.x<<box_maxiou.center.y<<box_maxiou.size.width<<box_maxiou.size.height<<box_maxiou.angle<<"\n"<<"----------"<<"\n"<<box_seciou.center.x<<box_seciou.center.y<<box_seciou.size.width<<box_seciou.size.height<<box_seciou.angle<<"\n"<<"----------"<<"\n"<<result.center.x<<result.center.y<<result.size.width<<result.size.height<<result.angle;
            set_recbar_value();//9
            Point2f pts[4];
            result.points(pts);
            for (int i = 0; i < 4; i++)
            {
                if (i == 3)
                {
                        line(temp_img, pts[i], pts[0], Scalar(0, 0, 255), 2, 8, 0);
                        break;
                }
                line(temp_img,pts[i], pts[i + 1], Scalar(0, 0, 255), 2, 8, 0);
            }
            QImage dst=Mat2Qt(temp_img);

            mywindow->ui->Cam_label->setPixmap(QPixmap::fromImage(dst));
            //imshow("result",temp_img);
            Mat re_img=perspective_transform(result,temp_img);
            ocr(re_img);
            set_recbar_value();//10
            return;

        }
        else {
            Point2f pts[4];
            bboxes[0].points(pts);
            for (int i = 0; i < 4; i++)
            {
                if (i == 3)
                {
                        line(temp_img, pts[i], pts[0], Scalar(0, 0, 255), 2, 8, 0);
                        break;
                }
                line(temp_img,pts[i], pts[i + 1], Scalar(0, 0, 255), 2, 8, 0);
            }
            set_recbar_value();//8
            QImage dst=Mat2Qt(temp_img);
            mywindow->ui->Cam_label->setPixmap(QPixmap::fromImage(dst));
            //imshow("result",temp_img);
            Mat re_img=perspective_transform(bboxes[0],temp_img);
            set_recbar_value();//9
            ocr(re_img);
            //ocr(temp_img);//直接进行文本识别
            set_recbar_value();//10
            return;
        }
    }
    else {
        QImage dst=Mat2Qt(temp_img);

        mywindow->ui->Cam_label->setPixmap(QPixmap::fromImage(dst));
        ocr(temp_img);
        set_recbar_value();//8
        set_recbar_value();//9
        set_recbar_value();//10

        return;
    }
}



void MainWindow::on_img_pushButton_clicked()
{
    if(isCam)
    {
        QString fileName;
        fileName = QFileDialog::getOpenFileName(this,tr("Select Image"),"",tr("Images(*.png *.bmp *.jpg *.tif *.gif *.jpeg);;AllFiles(*.*)"));
        if(fileName.isEmpty())
        {
             return;
        }
        else
        {
            upload_img_test(fileName,mywin);
        }
    }
    else {
        isCam=true;
        ui->img_pushButton->setText("上传吊牌图片");
        timer->start(50);
    }
}

void MainWindow::set_recbar_value(){

    int index = mywin->ui->rec_progressBar->value();
    if (index >= 100)
    {
        return;
    }
    else {
        for (int i=1;i<11;i++) {
            mywin->ui->rec_progressBar->setValue(i+index);
        }
    }
}

void MainWindow::on_tcp_pushButton_clicked()
{
    webserver=new WebSocketServer;
    webserver->show();
    mywin->ui->tcp_pushButton->setStyleSheet("background:gray");
}
void MainWindow::close_tcp(){
    mywin->ui->tcp_pushButton->setStyleSheet("");
}
