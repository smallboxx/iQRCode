#include "mutithread.h"

void ROIQThread::initwebapi(){
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

void ROIQThread::get_apitoken(){
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

void ROIQThread::webapi_listbywbqk(){
    QString pid="无";
    QString gender="无";
    QString zypl="无";
    QString gys="无";
    QString lxhh="无";
    QTextCodec *codec = QTextCodec::codecForName("gbk");
    QByteArray byte = codec->fromUnicode(str_wbkh);
    byte = byte.toPercentEncoding();
    QString str=mainpath+listeqkey+"?wpname="+byte+"&wpvalue="+str_wbpm.toUtf8();
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
                }
            }
        }
    }
    else
    {
        qDebug()<<"访问失败";
    }
    emit at_message(str_wbpm,str_wbkh,str_ys,str_cm,gender,zypl,gys,lxhh,pid);
    reply->deleteLater();
    WebSe->clearAccessCache();
    WebSe->clearConnectionCache();
}
