#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "websocket.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camtimer=new QTimer(this);
    labeltimer=new QTimer(this);
    capture=new cv::VideoCapture();
    tagpage=new QLabel(ui->tag_label);
    connect(camtimer,SIGNAL(timeout()),this,SLOT(ReadFrame()));
    this->initialization_all();
    labeltimer->singleShot(100,this,&MainWindow::set_maintag);
    attimer=new QTimer(this);
    attimer->setSingleShot(true);
    connect(attimer,&QTimer::timeout,this,&MainWindow::changeuseapi);
}

MainWindow::~MainWindow()
{
    capture->release();
    thread->deleteLater();
    delete ui;
}


void MainWindow::on_wx_pushButton_clicked()
{
    websocket *webserver=new websocket;
    webserver->show();
    ui->wx_pushButton->setDisabled(true);
    connect(webserver,&websocket::close,this,[=](){
        ui->wx_pushButton->setDisabled(false);
    });
    connect(webserver,&websocket::upload,this,&MainWindow::uploadimg);
}

void MainWindow::on_designtag_pushButton_clicked()
{
    printer *printer_page=new printer;
    printer_page->show();
    ui->designtag_pushButton->setDisabled(true);
    connect(printer_page,&printer::close,this,[=](){
        ui->designtag_pushButton->setDisabled(false);
        camtimer->start(50);
        delete printer_page;
    });
    connect(printer_page,&printer::send_mainwindow,this,&MainWindow::set_maintag);
}


void MainWindow::on_printer_comboBox_currentIndexChanged(int index)
{
    ui->cnprinter_pushButton->setText(QString::fromUtf8("连接")+ui->printer_comboBox->currentText());
    printer_model=index;
}

void MainWindow::on_uploadimg_pushButton_clicked()
{
    if(ui->uploadimg_pushButton->hasFocus()){
        QString filename;
        filename=QFileDialog::getOpenFileName(this,tr("Select Image"),"",tr("Image(*.png *.bmp *.tif *.gif *.jpeg *jpg);;AllFiles(*.*)"));
        uploadimg(filename);
    }
}

void MainWindow::on_cnprinter_pushButton_clicked()
{
    if(noprint){
        if(printer_model==0){
            QLibrary m_hDll("./CPCL_SDK.dll");
            m_hDll.load();
            DemoPrinterCreator = (PrinterCreator)m_hDll.resolve("PrinterCreator");
            DemoPrinterCreatorS = (PrinterCreatorS)m_hDll.resolve("PrinterCreatorS");
            DemoPrinterDestroy = (PrinterDestroy)m_hDll.resolve("PrinterDestroy");
            DemoPortOpen = (PortOpen)m_hDll.resolve("PortOpen");
            DemoPortClose = (PortClose)m_hDll.resolve("PortClose");
            DemoDirectIO = (DirectIO)m_hDll.resolve("DirectIO");
            AddLabel = (CPCL_AddLabel)m_hDll.resolve("CPCL_AddLabel");
            SetAlign = (CPCL_SetAlign)m_hDll.resolve("CPCL_SetAlign");
            AddText = (CPCL_AddText)m_hDll.resolve("CPCL_AddText");
            AddBarCode = (CPCL_AddBarCode)m_hDll.resolve("CPCL_AddBarCode");
            AddBarCodeText = (CPCL_AddBarCodeText)m_hDll.resolve("CPCL_AddBarCodeText");
            AddQRCode = (CPCL_AddQRCode)m_hDll.resolve("CPCL_AddQRCode");
            AddPDF417 = (CPCL_AddPDF417)m_hDll.resolve("CPCL_AddPDF417");
            AddBox = (CPCL_AddBox)m_hDll.resolve("CPCL_AddBox");
            AddLine = (CPCL_AddLine)m_hDll.resolve("CPCL_AddLine");
            SetFontSize = (CPCL_SetFontSize)m_hDll.resolve("CPCL_SetFontSize");
            SetDensity = (CPCL_SetDensity)m_hDll.resolve("CPCL_SetDensity");
            SetSpeed = (CPCL_SetSpeed)m_hDll.resolve("CPCL_SetSpeed");
            SetTextSpacing = (CPCL_SetTextSpacing)m_hDll.resolve("CPCL_SetTextSpacing");
            SetTextBold = (CPCL_SetTextBold)m_hDll.resolve("CPCL_SetTextBold");
            SetTextUnderline = (CPCL_SetTextUnderline)m_hDll.resolve("CPCL_SetTextUnderline");
            LabelPrint = (CPCL_Print)m_hDll.resolve("CPCL_Print");
            NextLabelPos = (CPCL_NextLabelPos)m_hDll.resolve("CPCL_NextLabelPos");
            PreFeed = (CPCL_PreFeed)m_hDll.resolve("CPCL_PreFeed");
            PostFeed = (CPCL_PostFeed)m_hDll.resolve("CPCL_PostFeed");
            AddImage = (CPCL_AddImage)m_hDll.resolve("CPCL_AddImage");
            const char * name = "HM-A300";
            const char * port = "USB";
            int result = -1;
            result = DemoPrinterCreator(&hprinter,name);
            result = DemoPortOpen(hprinter,port);
            if (result==0)
            {
                QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("汉印N31BT打印机成功连接"));
            }
            else{
                QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("汉印N31BT打印机连接失败"));
                return;
            }
            ui->cnprinter_pushButton->setText(QString::fromUtf8("断开N31BT"));
        }
        else if(printer_model==1){
            QLibrary m_hDll("./TSPL_SDK.dll");
            m_hDll.load();
            DemoPrinterCreator = (PrinterCreator)m_hDll.resolve("PrinterCreator");
            DemoPrinterDestroy = (PrinterDestroy)m_hDll.resolve("PrinterDestroy");
            DemoPortOpen = (PortOpen)m_hDll.resolve("PortOpen");
            DemoPortClose = (PortClose)m_hDll.resolve("PortClose");
            DemStatus =(TSPL_GetPrinterStatus)m_hDll.resolve("TSPL_GetPrinterStatus");
            DemSetup = (TSPL_Setup)m_hDll.resolve("TSPL_Setup");
            DemClearBuffer = (TSPL_ClearBuffer)m_hDll.resolve("TSPL_ClearBuffer");
            DemPrint = (TSPL_Print)m_hDll.resolve("TSPL_Print");
            DemDirection = (TSPL_Direction)m_hDll.resolve("TSPL_Direction");
            DemBitMap = (TSPL_BitMap)m_hDll.resolve("TSPL_BitMap");
            DemHome = (TSPL_Home)m_hDll.resolve("TSPL_Home");
            const char * name = "HT130";
            const char * port = "USB";
            int result = -1;
            result = DemoPrinterCreator(&hprinter,name);
            result = DemoPortOpen(hprinter,port);
            if (result==0)
            {
                QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("汉印N41BT打印机成功连接"));
            }
            else{
                QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("汉印N41BT打印机连接失败"));
                return;
            }
            DemDirection(hprinter,1);

            ui->cnprinter_pushButton->setText(QString::fromUtf8("断开N41BT"));
        }
        noprint=false;
    }
    else{
        printer_close();
        ui->cnprinter_pushButton->setText(QString::fromUtf8("连接打印机"));
        noprint=true;
    }
}

void MainWindow::on_print_pushButton_clicked()
{
    printer_run();
}



void MainWindow::on_wbpm_lineEdit_textChanged(const QString &arg1)
{
    RLabel* index=findChild<RLabel*>("wbpm");
    if(index){
        index->setText(QString::fromUtf8("品名:")+arg1);
        index->adjustSize();
    }
    if(ui->wbpm_lineEdit->hasFocus()){
        str1=ui->wbpm_lineEdit->text();
        str2=ui->wbkh_lineEdit->text();
        str3=ui->color_lineEdit->text();
        str4=ui->size_lineEdit->text();
        attimer->start(1000);
    }
}

void MainWindow::on_wbkh_lineEdit_textChanged(const QString &arg1)
{
    RLabel* index=findChild<RLabel*>("wbkh");
    if(index){
        index->setText(QString::fromUtf8("款号：")+arg1);
        index->adjustSize();
    }
    if(ui->wbkh_lineEdit->hasFocus()){
        str1=ui->wbpm_lineEdit->text();
        str2=ui->wbkh_lineEdit->text();
        str3=ui->color_lineEdit->text();
        str4=ui->size_lineEdit->text();
        attimer->start(1000);
    }
}

void MainWindow::on_color_lineEdit_textChanged(const QString &arg1)
{
    RLabel* index=findChild<RLabel*>("ys");
    if(index){
        index->setText(QString::fromUtf8("颜色：")+arg1);
        index->adjustSize();
    }
    if(ui->color_lineEdit->hasFocus()){
        str1=ui->wbpm_lineEdit->text();
        str2=ui->wbkh_lineEdit->text();
        str3=ui->color_lineEdit->text();
        str4=ui->size_lineEdit->text();
        attimer->start(1000);
    }
}

void MainWindow::on_size_lineEdit_textChanged(const QString &arg1)
{
    RLabel* index=findChild<RLabel*>("cm");
    if(index){
        index->setText(QString::fromUtf8("尺码：")+arg1);
        index->adjustSize();
    }
    if(ui->size_lineEdit->hasFocus()){
        str1=ui->wbpm_lineEdit->text();
        str2=ui->wbkh_lineEdit->text();
        str3=ui->color_lineEdit->text();
        str4=ui->size_lineEdit->text();
        attimer->start(1000);
    }
}

void MainWindow::on_gender_lineEdit_textChanged(const QString &arg1)
{
    RLabel* index=findChild<RLabel*>("xb");
    if(index){
        index->setText(QString::fromUtf8("性别：")+arg1);
        index->adjustSize();
    }
}

void MainWindow::on_zypl_lineEdit_textChanged(const QString &arg1)
{
    RLabel* index=findChild<RLabel*>("zypl");
    if(index){
        index->setText(QString::fromUtf8("主营品类：")+arg1);
        index->adjustSize();
    }
}

void MainWindow::on_gys_lineEdit_textChanged(const QString &arg1)
{
    RLabel* index=findChild<RLabel*>("gys");
    if(index){
        index->setText(QString::fromUtf8("供应商：")+arg1);
        index->adjustSize();
    }
}

void MainWindow::on_lxhh_lineEdit_textChanged(const QString &arg1)
{
    RLabel* index=findChild<RLabel*>("lxhh");
    if(index){
        index->setText(QString::fromUtf8("乐销货号")+arg1);
    }
}

void MainWindow::on_pid_lineEdit_textChanged(const QString &arg1)
{
    RLabel* index=findChild<RLabel*>("id");
    if(index){
        index->setText(arg1);
        index->adjustSize();
    }
    emit rebar();
}
