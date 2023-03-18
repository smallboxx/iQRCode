#ifndef MUTITHREAD_H
#define MUTITHREAD_H
#include <QThread>
#include <QMutex>
#include "opencv2/opencv.hpp"
#include "paddle/config.h"
#include "paddle/ocr_det.h"
#include "paddle/ocr_rec.h"
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <QMessageBox>

using namespace cv;
using namespace std;

class ROIQThread : public QThread
{
    Q_OBJECT
signals:
    void edge_message(QList<RotatedRect>);
    void ocr_message(QList<QString>);
    void ocr_bar();
    void at_message(QString,QString,QString,QString,QString,QString,QString,QString,QString);//外部品名，外部款号，颜色，尺码，性别，主营品类，供应商，乐销货号，商品ID
public:
    ROIQThread(QObject* par);
    ~ROIQThread();
    void run();
    void get_apitoken();//获取请求APItoken令牌_webapi
    void webapi_listbywbqk();//web_api查询对应商品ID_webapi
    void initwebapi();//初始化调用API参数_initialization
    vector<vector<double>> mutil(vector<vector<double>> m1, vector<vector<double>> m2);
    double normal_distribution(int u,int x);
    void fuzzy_reasoning(QList<QString> ocr_boxes);//对OCR结果进行模糊推理_mutithread

public slots:
    void stopImmediately();//强行停止多线程
    void setmodel(int model_index,Mat src);//设置run函数运模式_mutithread
    void setmodel2(int model_index,QString str1,QString str2,QString str3,QString str4);//设置run函数运模式_mutithread

private:
    QMutex m_lock;
    bool m_isCanrun;
    Mat img_0;//存储main线程传来图像
    Mat img_1;
    int model;//model=0——ROItransform检测，model=1——OCR识别
    PaddleOCR::DBDetector *m_det;
    PaddleOCR::CRNNRecognizer *m_rec;
    PaddleOCR::Classifier *m_cls;

//    API参数
    QNetworkAccessManager *WebSe;
    QString mainpath;
    QString username;
    QString password;
    QString token_api;
    QString listeqkey;
    QByteArray checkvalue;

    QString str_wbpm;
    QString str_wbkh;
    QString str_ys;
    QString str_cm;
};

#endif // MUTITHREAD_H
