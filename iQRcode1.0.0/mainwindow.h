/***********************************
*  File: mainwindow.h
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include "opencv2/opencv.hpp"
#include <vector>
#include <QImage>
using namespace cv;
#include <QString>
#include <QPainter>
#include <QPen>
#include "paddle/config.h"
#include "paddle/ocr_det.h"
#include "paddle/ocr_rec.h"
#include <QChar>
#include "printer.h"
#include "QLibrary"
#include <QMessageBox>
#include <QTextCodec>
#include <cstring>
#include <QDebug>
#include "dpage.h"
#include <QCameraInfo>
#include <QScreen>
#include <iostream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <Zint/code128.h>
#include "websocketserver.h"
#include <QThread>
#include <QtConcurrent>
#include <QFuture>



#define WIN32COMMON
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage Mat2Qt(const Mat &src);
    Mat resize_frame(Mat &frame);
    QList<RotatedRect> frame_pro(Mat img,QList<RotatedRect> blocks_out,double angle);//dilate提取roi
    QList<RotatedRect> edge_extraction(Mat img);//Canny提取roi
    void nms();
    float get_box_iou(RotatedRect box1, RotatedRect box2);
    void Mtcnn();
    RotatedRect RotatedRect_trans(RotatedRect box,double angel);
    double angle(Point pt1, Point pt2, Point pt0);
    Mat locate_label(const Mat &gray,const Mat &orimg,bool model);
    Mat perspective_transform(RotatedRect M,const Mat &img);
    void ocr(Mat &reslutimgh);
    bool eventFilter(QObject *watched, QEvent *event);
    void showPaint();
    void bar_code(map<QString,QString> map);
    void* hprinter;
    PrinterCreator DemPrinterCreator;
    PortOpen DemPortOpen;
    TSPL_GetPrinterStatus DemStatus;
    PortClose DemPortClose;
    PrinterDestroy DemPrinterDestroy;
    TSPL_Setup DemSetup;
    TSPL_ClearBuffer DemClearBuffer;
    TSPL_BarCode DemBarCode;
    TSPL_Print DemPrint;
    TSPL_Text DemText;
    TSPL_Direction DemDirection;
    TSPL_FormFeed DemFormFeed;
    TSPL_Home DemHome;
    TSPL_SetCodePag DemCodePag;
    TSPL_BitMap DemBitMap;
    const char * QString2char(QString str);
    void label_set(int at,int font,int size,QString label);
    QString get_font(int index);
    void change_label_text(int at,QString cbbox,QString frontat);
    vector<vector<vector<int>>> revise_boxes(vector<vector<vector<int>>> boxes);
    Mat mask_img(const Mat &erode);
    void label_align(int index,QString label);
    void get_token();
    void get_suplist(QString str);
    void set_QComboBox(QString urlstr,QString comboboxname);
    void doc_set_QcomboBox(QString comboboxname);
    void get_id();
    void initiation_api();
    double definition(Mat image);
    void img_add_ins(const Mat &img);
    void img_light_adjust(Mat img);
    void mnn_test();
    void upload_img_test(QString fileName,MainWindow* mywindow);
    void close_tcp();
    Mat compute_flag(QList<RotatedRect> boxes,Mat img,bool model);
    void set_recbar_value();

private slots:
    void ReadFrame();

    void on_PM_comboBox_currentTextChanged();

    void on_ZYPL_comboBox_currentTextChanged();

    void on_HH_comboBox_currentTextChanged();

    void on_PP_comboBox_currentTextChanged();

    void on_SL_comboBox_currentTextChanged();

    void on_ZL_comboBox_currentTextChanged();

    void on_XB_comboBox_currentTextChanged();

    void on_YS_comboBox_currentTextChanged();

    void on_GYS_comboBox_currentTextChanged();

    void on_XL_comboBox_currentTextChanged();

    void on_ZTJ_comboBox_currentTextChanged();

    void on_DL_comboBox_currentTextChanged();

    void on_CGY_comboBox_currentTextChanged();

    void on_NF_comboBox_currentTextChanged();

    void on_CCZ_comboBox_currentTextChanged();

    void on_CGXZ_comboBox_currentTextChanged();

    void on_BX_comboBox_currentTextChanged();

    void on_CKG_comboBox_currentTextChanged();

    void on_NLD_comboBox_currentTextChanged();

    void on_LXSH_comboBox_currentTextChanged();

    void on_CM_comboBox_currentTextChanged();

    void on_JYFS_comboBox_currentTextChanged();

    void on_JJ_comboBox_currentTextChanged();

    void on_CN_pushButton_clicked();

    void on_PR_pushButton_clicked();

    void on_ST_pushButton_clicked();


    void on_dpage_pushButton_clicked();

    void on_Cam_comboBox_activated(int index);

    void set_page_widget();

    void on_test_pushButton_clicked();

    void on_id_pushButton_clicked();

    void query_sl(QString text);

    QJsonDocument set_menu();

    QJsonDocument get_webapi(QString str,QString keywords);

    void PM_query();

    void on_img_pushButton_clicked();


    void on_tcp_pushButton_clicked();

public:
    Ui::MainWindow *ui;
    QImage *image;
    QTimer *timer;
    QTimer *Reload_timer;
    QTimer *PMtimer;
    VideoCapture capture;
    Mat myimg;
    Mat frame;
    Mat frame2;
    int flag_1=0;
    int flag_2=0;
    int flag_area=0;
    int ten_timer=0;
    //int A[9][9];
    void initCamera();
    void initPrinter();
    PaddleOCR::DBDetector* m_det;
    PaddleOCR::CRNNRecognizer* m_rec;
    PaddleOCR::Classifier *m_cls;
    std::map<QString,QString> mymap;
    std::map<QString,QString> indexmap;
    QString Code;
    QPainter painter;
    QTextCodec *codec;
    QFont textfont;
    dpage *design_page;
    WebSocketServer *webserver;

    QList<QCameraInfo> infos;
    bool noprint;

    std::map<QString,int> printmap;
    bool bar_ischeck;
    bool data_ischeck;

    std::map<QString,int> atmap;
    QNetworkAccessManager* WebSe;
    QByteArray checkvalue;
    std::map<QString,map<QString,QString>> webapimap;
    std::map<QString,map<QString,QString>> SLmap;

    QJsonObject all_json;
    QJsonArray sl_arry;
    QJsonDocument all_doc;

    bool isCam;//是否调用摄像头
    //WEBAPI_PARAMS
    QString mainpath;
    QString username;
    QString password;
    QString token_api;
    QString query_mDimlist;
    QString query_supperlist;
    QString addreturnid;
    QString query_product;
    QString get_pid;
    QString pplist;
    QString sexlist;
    QString yearlist;
    QString supplier_list;
    QString seasonlist;
    QString mDimlist;
    QString dllist;
    QString mediumlist;
    QString smalllist;
    QString speciallist;
    QString operationlist;
    QString merchantlist;
    QString agelist;
    QString cgxzlist;
    QString buyerlist;
    QString versionlist;
    QString lengthlist;
    QString sizelist;
    QString colorlist;
    QString cczlist;

};

#endif // MAINWINDOW_H
