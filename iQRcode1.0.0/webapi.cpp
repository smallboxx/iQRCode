/***********************************
*  File: webapi.cpp
*  Author: Mao Yu Meng
*  Time: 2022-10-30
************************************/

#include "mainwindow.h"
#include <QtNetwork>
#include "ui_mainwindow.h"
#include <QListView>
#include <QCompleter>

QJsonDocument MainWindow::set_menu(){
    QFile file("./sql.json");
    QJsonDocument document;
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(!file.isOpen()){
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("未找到菜单文件，请点击更新菜单按钮"));
        return document;
    }
    QVariant data = file.readAll();
    file.close();
    QJsonParseError jsonError;
    document=QJsonDocument::fromJson(data.toByteArray(),&jsonError);
    if(!document.isNull()&&(jsonError.error==QJsonParseError::NoError))
    {
        return document;
    }
    return document;
}

QJsonDocument MainWindow::get_webapi(QString str,QString keywords){
    get_token();
    QUrl url(str+keywords);
    qDebug()<<url;
    QJsonDocument document;
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    request.setRawHeader("Cookie",checkvalue);
    QNetworkReply* reply=WebSe->get(request);
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
    int iError = reply->error();
    if(iError == QNetworkReply::NoError){
        QVariant data = reply->readAll();
        QJsonParseError jsonError;
        document=QJsonDocument::fromJson(data.toByteArray(),&jsonError);
        return document;
    }
    return document;
    reply->deleteLater();
}

void MainWindow::get_token(){
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
    //int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //qDebug()<<"statusCode="<<statusCode;
    if (iError == QNetworkReply::NoError)
    {
        //qDebug()<<"token令牌获取成功";
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

void MainWindow::query_sl(QString text){
    QStringList list;
    QString str=query_mDimlist+text;
    get_token();
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
            QString indexstr;
            for (auto sds=dataarry.begin();sds!=dataarry.end();sds++)
            {
                QJsonObject dataobj=sds->toObject();
                sl_arry.append(dataobj);
                indexstr=dataobj["mdim8idattribname"].toString();
                webapimap["SL_comboBox"][indexstr]=dataobj["m_dim8_id"].toString();
                SLmap[indexstr]["季节"]=dataobj["mdim3idattribname"].toString();
                SLmap[indexstr]["主营品类"]=dataobj["mdim4idattribname"].toString();
                SLmap[indexstr]["大类"]=dataobj["mdim5idattribname"].toString();
                SLmap[indexstr]["中类"]=dataobj["mdim6idattribname"].toString();
                SLmap[indexstr]["小类"]=dataobj["mdim7idattribname"].toString();
                list.append(indexstr);
            }
        }
    }
    else
    {
        qDebug()<<"访问失败";
    }
    reply->deleteLater();
    ui->SL_comboBox->addItems(list);
}

void MainWindow::get_suplist(QString str){
    QStringList list;
    get_token();
    QUrl url(query_supperlist+str);
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
            for (auto sds=dataarry.begin();sds!=dataarry.end();sds++)
            {
                QJsonObject dataobj=sds->toObject();
                list.append(dataobj["name"].toString());
            }

        }
    }
    else
    {
        qDebug()<<"访问失败";
    }
    reply->deleteLater();
    ui->GYS_comboBox->clear();
    ui->GYS_comboBox->addItems(list);
}

void MainWindow::set_QComboBox(QString urlstr, QString comboboxname){
    QStringList list;
    get_token();
    QUrl url(urlstr);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Cookie",checkvalue);
    QNetworkReply* reply=WebSe->get(request);
    //开启事件循环，直到请求完成
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
    int iError = reply->error();
    if (iError == QNetworkReply::NoError)
    {
        //qDebug()<<"yes";
        QVariant data = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument document=QJsonDocument::fromJson(data.toByteArray(),&jsonError);
        if(!document.isNull()&&(jsonError.error==QJsonParseError::NoError))
        {
            QJsonValue dataval=document["data"];
            QJsonArray dataarry=dataval.toArray();
            all_json.insert(comboboxname,dataarry);
            QString indexstr;
            for (auto sds=dataarry.begin();sds!=dataarry.end();sds++) {
                QJsonObject dataobj=sds->toObject();
                if(comboboxname=="CM_comboBox" || comboboxname=="YS_comboBox" || comboboxname=="CCZ_comboBox" || comboboxname=="GYS_comboBox"){
                    indexstr=dataobj["name"].toString();
                    webapimap[comboboxname][indexstr]=dataobj["id"].toString();
                    list.append(dataobj["name"].toString());
                }
                else{
                    indexstr=dataobj["attribname"].toString();
                    webapimap[comboboxname][indexstr]=dataobj["id"].toString();
                    list.append(dataobj["attribname"].toString());
                }
            }
        }
    }
    else
    {
        qDebug()<<"访问失败";
    }
    if(comboboxname=="CKG_comboBox"){

    }
    else{
        list.append(QString::fromLocal8Bit("无"));
    }
    reply->deleteLater();
    auto name = findChild<QComboBox*>(comboboxname);
    name->clear();
    name->addItems(list);
    name->setView(new QListView);
    name->setMaxVisibleItems(5);
    name->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QCompleter* pCompleter = new QCompleter(name->model());
    pCompleter->setFilterMode(Qt::MatchContains);
    name->setCompleter(pCompleter);
}

void MainWindow::doc_set_QcomboBox(QString comboboxname){
    QStringList list;
    QJsonObject temp;
    if(!all_doc.isNull())
    {
        QJsonValue dataval=all_doc[comboboxname];
        QJsonArray dataarry=dataval.toArray();
        QString indexstr;
        for (auto sds=dataarry.begin();sds!=dataarry.end();sds++) {
            QJsonObject dataobj=sds->toObject();
            if(comboboxname=="CM_comboBox" || comboboxname=="YS_comboBox" || comboboxname=="CCZ_comboBox" || comboboxname=="GYS_comboBox"){
                list.append(dataobj["name"].toString());
                indexstr=dataobj["name"].toString();
                webapimap[comboboxname][indexstr]=dataobj["id"].toString();
            }
            else if(comboboxname=="SL_comboBox"){
                list.append(dataobj["mdim8idattribname"].toString());
                indexstr=dataobj["mdim8idattribname"].toString();
                webapimap["SL_comboBox"][indexstr]=dataobj["m_dim8_id"].toString();
                SLmap[indexstr]["季节"]=dataobj["mdim3idattribname"].toString();
                SLmap[indexstr]["主营品类"]=dataobj["mdim4idattribname"].toString();
                SLmap[indexstr]["大类"]=dataobj["mdim5idattribname"].toString();
                SLmap[indexstr]["中类"]=dataobj["mdim6idattribname"].toString();
                SLmap[indexstr]["小类"]=dataobj["mdim7idattribname"].toString();
            }
            else{
                list.append(dataobj["attribname"].toString());
                indexstr=dataobj["attribname"].toString();
                webapimap[comboboxname][indexstr]=dataobj["id"].toString();
            }
        }
    }
    if(comboboxname=="CKG_comboBox"){

    }
    else{
        list.append(QString::fromLocal8Bit("无"));
    }
    auto name = findChild<QComboBox*>(comboboxname);
    name->clear();
    name->addItems(list);
    name->setView(new QListView);
    name->setMaxVisibleItems(5);
    name->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QCompleter* pCompleter = new QCompleter(name->model());
    pCompleter->setFilterMode(Qt::MatchContains);
    name->setCompleter(pCompleter);
}

void MainWindow::get_id(){
    get_token();
    QJsonObject body;
    if(ui->PM_comboBox->currentText()==""){
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请输入品名"));
            return;
    }
    else{
        body.insert("value",ui->PM_comboBox->currentText());//品名
    }


    if(ui->JHJ_comboBox->currentText()==""){
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请输入进货价"));
            return;
    }
    else{
        body.insert("precost",ui->JHJ_comboBox->currentText());//进货价
    }


    if(ui->LSJ_comboBox->currentText()==""){
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请输入零售价"));
            return;
    }
    else{
        body.insert("pricelist",ui->LSJ_comboBox->currentText());//零售价
    }


    if(ui->PP_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim1_id",webapimap["PP_comboBox"][ui->PP_comboBox->currentText()]);//品牌
    }

    if(ui->NF_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim2_id",webapimap["NF_comboBox"][ui->NF_comboBox->currentText()]);//年份
    }
    if(ui->JJ_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim3_id",webapimap["JJ_comboBox"][ui->JJ_comboBox->currentText()]);
    }

    if(ui->ZYPL_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim4_id",webapimap["ZYPL_comboBox"][ui->ZYPL_comboBox->currentText()]);//主营品类
    }

    if(ui->DL_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim5_id",webapimap["DL_comboBox"][ui->DL_comboBox->currentText()]);//大类
    }

    if(ui->ZL_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim6_id",webapimap["ZL_comboBox"][ui->ZL_comboBox->currentText()]);//中类
    }

    if(ui->XL_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim7_id",webapimap["XL_comboBox"][ui->XL_comboBox->currentText()]);
    }

    if(ui->GYS_comboBox->currentText()=="无"){

    }
    else{
        body.insert("c_supplier_id",webapimap["GYS_comboBox"][ui->GYS_comboBox->currentText()]);
    }

    if(ui->CCZ_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_sizegroup_id",webapimap["CCZ_comboBox"][ui->CCZ_comboBox->currentText()]);
    }

    if(ui->CM_comboBox->currentText()=="无"){

    }
    else{
        body.insert("sizes",webapimap["CM_comboBox"][ui->CM_comboBox->currentText()]);
    }

    if(ui->YS_comboBox->currentText()=="无"){

    }
    else{
        body.insert("colors",webapimap["YS_comboBox"][ui->YS_comboBox->currentText()]);
    }

    if(ui->ZTJ_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim11_id",webapimap["ZTJ_comboBox"][ui->ZTJ_comboBox->currentText()]);
    }

    if(ui->JYFS_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim12_id",webapimap["JYFS_comboBox"][ui->JYFS_comboBox->currentText()]);
    }

    if(ui->LXSH_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim13_id",webapimap["LXSH_comboBox"][ui->LXSH_comboBox->currentText()]);
    }

    if(ui->XB_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim16_id",webapimap["XB_comboBox"][ui->XB_comboBox->currentText()]);

    }

    if(ui->NLD_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim17_id",webapimap["NLD_comboBox"][ui->NLD_comboBox->currentText()]);
    }

    if(ui->CGXZ_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim18_id",webapimap["CGXZ_comboBox"][ui->CGXZ_comboBox->currentText()]);
    }

    if(ui->CGY_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim19_id",webapimap["CGY_comboBox"][ui->CGY_comboBox->currentText()]);

    }

    if(ui->BX_comboBox->currentText()=="无"){

    }
    else{
        body.insert("m_dim20_id",webapimap["BX_comboBox"][ui->BX_comboBox->currentText()]);
    }

    if(ui->CKG_comboBox->currentText()==""){

    }
    else{
        body.insert("m_dim23_id",webapimap["CKG_comboBox"][ui->CKG_comboBox->currentText()]);
    }

    if(ui->SL_comboBox->currentText()==""){

    }
    else{
        body.insert("hidmdim8value",webapimap["SL_comboBox"][ui->SL_comboBox->currentText()]);
    }

    QJsonDocument doc;
    doc.setObject(body);
    //qDebug()<<doc;
    QByteArray param = doc.toJson(QJsonDocument::Compact);
    QUrl url(addreturnid);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Cookie",checkvalue);
    QNetworkReply* reply=WebSe->post(request,param);
    //开启事件循环，直到请求完成
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
    int iError = reply->error();
    if (iError == QNetworkReply::NoError)
    {
        /*
        ui->HH_comboBox->clear();
        QByteArray data = reply->readAll();
        QString str;
        str.prepend(data);
        str.replace("\"","");
        ui->HH_comboBox->addItem(str);
        //qDebug()<<str;
        */

    }
    else
    {
        qDebug()<<"访问失败";
    }
    reply->deleteLater();
    PMtimer->start(1000);
}




void MainWindow::initiation_api(){
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

    setting.beginGroup("query_mDimlist");
    query_mDimlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("query_supplier");
    query_supperlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("addreturnid");
    addreturnid=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("query_product");
    query_product=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("get_pid");
    get_pid=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("pplist");
    pplist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("sexlist");
    sexlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("yearlist");
    yearlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("supplier_list");
    supplier_list=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("seasonlist");
    seasonlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("mDimlsit");
    mDimlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("dllist");
    dllist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("mediumlist");
    mediumlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("smalllist");
    smalllist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("speciallist");
    speciallist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("operationlist");
    operationlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("merchantlist");
    merchantlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("agelist");
    agelist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("cgxzlist");
    cgxzlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("buyerlist");
    buyerlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("versionlist");
    versionlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("lengthlist");
    lengthlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("sizelist");
    sizelist=mainpath+setting.value("api").toString();
    setting.endGroup();


    setting.beginGroup("colorlist");
    colorlist=mainpath+setting.value("api").toString();
    setting.endGroup();

    setting.beginGroup("cczlist");
    cczlist=mainpath+setting.value("api").toString();
    setting.endGroup();

}
