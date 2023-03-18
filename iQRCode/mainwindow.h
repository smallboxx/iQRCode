#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QFontDatabase>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ximgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <QCameraInfo>
#include <QMessageBox>
#include <QTimer>
#include "mutithread.h"
#include "mmdeploy/rotated_detector.h"
#include <QLibrary>
#include "printer.h"
#include <QScreen>
#include <QGuiApplication>
#include <QList>
#include <QLabel>
#include "customlabel.h"
#include "Zint/code128.h"
#include <QPushButton>
#include <QLineEdit>
#include <QTextCursor>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>


using namespace cv;
using namespace std;
using namespace cv::ximgproc;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initialization_all();//_mainwindow
    void initScreen();//初始化系统分辨率_initialization
    void initqss();//初始化控件_initialization
    void initcam();//初始化摄像头_initialization
    void initwebapi();//初始化调用API参数_initialization
    void initprinter();//初始化打印机_initialization
    void printer_run();//打印机打印操作_initialization
    void printer_close();//断开打印机连接 _initialization
    QImage Mat2Qt(const cv::Mat &src);//MAT图像转QIMage格式_initialization
    cv::Mat Qt2Mat(const QImage &src);//QIMage格式转MAT图像_initialization
    cv::Mat Resizeimg(cv::Mat src,int w,int h);//等比缩放原图像_initialization
    float Get_box_iou(RotatedRect box1, RotatedRect box2);//评价两个矩形框相似度_initialization
    RotatedRect RotatedRect_trans(RotatedRect box,double angel);//按角度旋转矩形框参数_initialization
    void muti_thread();//多线程初始化_initialization
    Mat Painter_roi(QList<RotatedRect> box,Mat src);//绘制矩形框_initalization
    void set_maintag();//设置标签区域_initialization
    void resizeEvent(QResizeEvent *event)override;//控制标签中label跟随缩放_initialization
    void reset_barcode();//ID变换触发该函数，标签显示对应条形码_initialization
    void pushturncam();//初始化按钮返回_initialization
    void get_apitoken();//获取请求APItoken令牌_webapi
    void webapi_listbywbqk();//web_api查询对应商品ID_webapi
    QList<cv::RotatedRect> Erode_extraction(cv::Mat src);//自定义腐蚀检测提取_camera
    RotatedRect FLD_extraction(cv::Mat src);//基于FLD直线检测提取矩形框_camera
    Vec4f Merge_line(Vec4f line1,Vec4f line2);
    Mat perspective_transform(RotatedRect M,const Mat &img);//透视变换矫正文本区域_camera

private slots:
    void on_camname_comboBox_activated(int index);//切换摄像头_camera

    void ReadFrame();//处理每一帧图像_camera

    void on_uploadimg_pushButton_clicked();//处理上传图片_uploadimg

    void ocr_result(QList<QString> ocr_boxes);//返回ocr结果_camera

    void edge_result(QList<RotatedRect> boxes);//返回边缘检测结果_camera

    void at_set(QString strwbpm,QString strwbkh,QString strys,QString strcm,QString strgender,QString strzypl,QString strgys,QString strlxhh,QString strpid);

    void on_wx_pushButton_clicked();//连接微信小程序

    void on_printer_comboBox_currentIndexChanged(int index);

    void on_cnprinter_pushButton_clicked();

    void ocr_bar();//进度条显示_initalization

    void on_designtag_pushButton_clicked();

    void on_print_pushButton_clicked();

    void on_wbpm_lineEdit_textChanged(const QString &arg1);

    void on_wbkh_lineEdit_textChanged(const QString &arg1);

    void on_color_lineEdit_textChanged(const QString &arg1);

    void on_size_lineEdit_textChanged(const QString &arg1);

    void on_gender_lineEdit_textChanged(const QString &arg1);

    void on_zypl_lineEdit_textChanged(const QString &arg1);

    void on_gys_lineEdit_textChanged(const QString &arg1);

    void on_lxhh_lineEdit_textChanged(const QString &arg1);

    void on_pid_lineEdit_textChanged(const QString &arg1);

    void uploadimg(QString filename);

    void turnCam();

    void changeuseapi();
signals:
    void rebar();

private:
    Ui::MainWindow *ui;
    QList<QCameraInfo> cams;
    cv::VideoCapture *capture;
    QTimer *camtimer;//捕获计时器
    QTimer *labeltimer;
    QTimer *attimer;
    ROIQThread *mtest;
    ROIQThread *thread;//多线程边缘检测
    QList<RotatedRect> frame_boxes;//每一帧的ROI
    QList<RotatedRect> default_boxes;//预设的ROI
    int flag=0;
    int reocr_timer=0;
    int ocr_area=0;
    int ocr_timer=0;
    vector<Point> tempPoints;
    int flagleft;
    QString Code;//barcode
    QPushButton *turn;
    QString str1;
    QString str2;
    QString str3;
    QString str4;
//    API参数
    QNetworkAccessManager *WebSe;
    QString mainpath;
    QString username;
    QString password;
    QString token_api;
    QString listeqkey;
    QByteArray checkvalue;
//    打印机函数
    QLabel* tagpage;//标签对象
    double tagw;
    double tagh;
    bool noprint=true;
    void* hprinter;
    int printer_model;
    QFont  Rlabelfont;
    PrinterCreator                               DemoPrinterCreator;
    PrinterCreatorS                              DemoPrinterCreatorS;
    PrinterDestroy                               DemoPrinterDestroy;
    PortOpen                                     DemoPortOpen;
    PortClose                                    DemoPortClose;
    DirectIO                                     DemoDirectIO;
    CPCL_AddLabel                                AddLabel;
    CPCL_SetAlign                                SetAlign;
    CPCL_AddText                                 AddText;
    CPCL_AddBarCode                              AddBarCode;
    CPCL_AddBarCodeText                          AddBarCodeText;
    CPCL_AddQRCode                               AddQRCode;
    CPCL_AddPDF417                               AddPDF417;
    CPCL_AddBox                                  AddBox;
    CPCL_AddLine                                 AddLine;
    CPCL_SetFontSize                             SetFontSize;
    CPCL_SetDensity                              SetDensity;
    CPCL_SetSpeed                                SetSpeed;
    CPCL_SetTextSpacing                          SetTextSpacing;
    CPCL_SetTextBold                             SetTextBold;
    CPCL_SetTextUnderline                        SetTextUnderline;
    CPCL_Print                                   LabelPrint;
    CPCL_NextLabelPos                            NextLabelPos;
    CPCL_PreFeed                                 PreFeed;
    CPCL_PostFeed                                PostFeed;
    CPCL_AddImage                                AddImage;

    TSPL_GetPrinterStatus                        DemStatus;
    TSPL_Setup                                   DemSetup;
    TSPL_ClearBuffer                             DemClearBuffer;
    TSPL_Print                                   DemPrint;
    TSPL_Direction                               DemDirection;
    TSPL_BitMap                                  DemBitMap;
    TSPL_Home                                    DemHome;
};
#endif // MAINWINDOW_H
