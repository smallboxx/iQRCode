/***********************************
*  File: Code.cpp
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

void MainWindow::bar_code(map<QString,QString> map)
{

}



bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->label_bar && event->type() == QEvent::Paint)
    {
        showPaint(); //响应函数
    }
    return QWidget::eventFilter(watched,event);
}

void MainWindow::showPaint()
{
    if(bar_ischeck){
        update();
        if(Code=="219025300F"){
            Code="";
        }
        Code128 bc=Code128(Code,"temp");
        bc.start();
        QImage img("temp.jpg");
        ui->label_bar->setPixmap(QPixmap::fromImage(img));

    }
    if(data_ischeck)
        ui->label_data->setText(Code);
}




void MainWindow::change_label_text(int at,QString cbbox,QString frontat)
{
    auto sed = findChild<QComboBox*>(cbbox);
    for (auto iter = atmap.begin();iter !=atmap.end();iter++)
    {
        if (at==iter->second)
        {
           auto ind = findChild<QLabel*>(iter->first);
           QString tt=frontat+sed->currentText();
           ind->setText(tt);
        }
    }
}

void MainWindow::on_PM_comboBox_currentTextChanged()
{
    QString str=ui->PM_comboBox->currentText();
    mymap[QString::fromLocal8Bit("品名")] =str;
    change_label_text(1,"PM_comboBox",QString::fromLocal8Bit("品名:"));
    PMtimer->start(1000);
}


void MainWindow::PM_query(){
    PMtimer->stop();
    QString str=ui->PM_comboBox->currentText();
    QString keywords="?keywords="+str;
    QJsonDocument doc=get_webapi(query_product,keywords);
    if(!doc.isNull()){
        QJsonValue value=doc["data"];
        QJsonArray array=value.toArray();
        if(!array.isEmpty()){

            for (auto sds=array.begin();sds!=array.end();sds++) {
                QJsonObject obj=sds->toObject();
                ui->HH_comboBox->clear();
                ui->HH_comboBox->addItem(obj["name"].toString());
                ui->PM_label->setStyleSheet("font:16px,楷体;color:red");
                ui->PM_label->setText("品名已存在");
            }
        }
        else
        {
            ui->HH_comboBox->clear();
            ui->PM_label->setStyleSheet("font-size:20px,楷体");
            ui->PM_label->setText("品名");
        }

    }
}

void MainWindow::on_ZYPL_comboBox_currentTextChanged()
{
    QString str=ui->ZYPL_comboBox->currentText();
    mymap[QString::fromLocal8Bit("主营品类")] =str;
    change_label_text(2,"ZYPL_comboBox",QString::fromLocal8Bit("主营品类:"));
    get_suplist(str);
}

void MainWindow::on_HH_comboBox_currentTextChanged()
{
    QString str=ui->HH_comboBox->currentText();
    if(str==""){
        return;
    }
    mymap[QString::fromLocal8Bit("货号")] =str;
    change_label_text(3,"HH_comboBox",QString::fromLocal8Bit("货号:"));
    QString keywords="?page=1&limit=10&searchValue="+str+"&colorid=&sexid=&ageid=&pdtStatus=&discountid=&dlid=&ppid=&seasonid=&supplierid=&yearid=&cgxzid=&specialid=&operationid=&merchantid=&versionid=&lengthid=&searchBeginTime=&searchEndTime=";
    QJsonDocument doc=get_webapi(get_pid,keywords);
    QJsonValue value=doc["data"];
    QJsonArray array=value.toArray();
    if(!array.isEmpty()){
        for (auto sds=array.begin();sds!=array.end();sds++) {

            QJsonObject obj=sds->toObject();
            //qDebug()<<obj;
            Code=obj["name"].toString();
            if(obj["mdim8idattribname"].toString()!=""){
                ui->SL_comboBox->setCurrentText(obj["mdim8idattribname"].toString());
            }
            else{
                 ui->SL_comboBox->setCurrentText("无");
            }
            if(obj["mdim4idattribname"].toString()!=""){
                ui->ZYPL_comboBox->setCurrentText(obj["mdim4idattribname"].toString());
            }
            else {
                ui->ZYPL_comboBox->setCurrentText("无");
            }

            QString JHJ=QString::number(obj["precost"].toDouble(2),'f',2);
            QString LSJ=QString::number(obj["pricelist"].toDouble(2),'f',2);
            ui->JHJ_comboBox->setCurrentText(JHJ);
            ui->LSJ_comboBox->setCurrentText(LSJ);

            if(obj["suppliername"].toString()!=""){
                ui->GYS_comboBox->setCurrentText(obj["suppliername"].toString());
            }
            else {
                ui->GYS_comboBox->setCurrentText("无");
            }
            if(obj["mdim5idattribname"].toString()!=""){
                ui->DL_comboBox->setCurrentText(obj["mdim5idattribname"].toString());
            }
            else{
                ui->DL_comboBox->setCurrentText("无");
            }
            if(obj["mdim6idattribname"].toString()!=""){
                ui->ZL_comboBox->setCurrentText(obj["mdim6idattribname"].toString());
            }
            else{
                ui->ZL_comboBox->setCurrentText("无");
            }
            if(obj["mdim7idattribname"].toString()!=""){
                ui->XL_comboBox->setCurrentText(obj["mdim7idattribname"].toString());
            }
            else{
                ui->XL_comboBox->setCurrentText("无");
            }
            if(obj["mdim11idattribname"].toString()!=""){
                ui->ZTJ_comboBox->setCurrentText(obj["mdim11idattribname"].toString());
            }
            else{
                ui->ZTJ_comboBox->setCurrentText("无");
            }
            if(obj["mdim13idattribname"].toString()!=""){
                ui->LXSH_comboBox->setCurrentText(obj["mdim13idattribname"].toString());
            }
            else{
                ui->LXSH_comboBox->setCurrentText("无");
            }
            if(obj["mdim12idattribname"].toString()!=""){
                ui->JYFS_comboBox->setCurrentText(obj["mdim12idattribname"].toString());
            }
            else{
                ui->JYFS_comboBox->setCurrentText("无");
            }
            if(obj["mdim19idattribname"].toString()!=""){
                ui->CGY_comboBox->setCurrentText(obj["mdim19idattribname"].toString());
            }
            else{
                ui->CGY_comboBox->setCurrentText("无");
            }
            if(obj["mdim18idattribname"].toString()!=""){
                ui->CGXZ_comboBox->setCurrentText(obj["mdim18idattribname"].toString());
            }
            else{
                ui->CGXZ_comboBox->setCurrentText("无");
            }
            if(obj["mdim17idattribname"].toString()!=""){
                ui->NLD_comboBox->setCurrentText(obj["mdim17idattribname"].toString());
            }
            else
            {
                ui->NLD_comboBox->setCurrentText("无");
            }
            if(obj["mdim2idattribname"].toString()!=""){
                ui->NF_comboBox->setCurrentText(obj["mdim2idattribname"].toString());
            }
            else{
                ui->NF_comboBox->setCurrentText("无");
            }
            if(obj["mdim1idattribname"].toString()!=""){
                ui->PP_comboBox->setCurrentText(obj["mdim1idattribname"].toString());
            }
            else{
                ui->PP_comboBox->setCurrentText("无");
            }
            if(obj["mdim3idattribname"].toString()!=""){
                ui->JJ_comboBox->setCurrentText(obj["mdim3idattribname"].toString());
            }
            else{
                ui->JJ_comboBox->setCurrentText("无");
            }
            if(obj["mdim20idattribname"].toString()!=""){
                ui->BX_comboBox->setCurrentText(obj["mdim20idattribname"].toString());
            }
            else{
                ui->BX_comboBox->setCurrentText("无");
            }
            if(obj["mdim23idattribname"].toString()!=""){
                ui->CKG_comboBox->setCurrentText(obj["mdim23idattribname"].toString());
            }
            else{
                ui->CKG_comboBox->setCurrentText("无");
            }
            if(obj["colorname"].toString()!=""){
                ui->YS_comboBox->setCurrentText(obj["colorname"].toString());
            }
            else{
                ui->YS_comboBox->setCurrentText("无");
            }
            if(obj["groupsizename"].toString()!=""){
                ui->CCZ_comboBox->setCurrentText(obj["groupsizename"].toString());
            }
            else{
                ui->CCZ_comboBox->setCurrentText("无");
            }
            if(obj["sizename"].toString()!=""){
                ui->CM_comboBox->setCurrentText(obj["sizename"].toString());
            }
            else{
                ui->CM_comboBox->setCurrentText("无");
            }

            if(obj["mdim16idattribname"].toString()!=""){
                ui->XB_comboBox->setCurrentText(obj["mdim16idattribname"].toString());
            }
            else{
                ui->XB_comboBox->setCurrentText("无");
            }
        }
    }
    else {
        return;
    }

}

void MainWindow::on_PP_comboBox_currentTextChanged()
{
    QString str=ui->PP_comboBox->currentText();
    mymap[QString::fromLocal8Bit("品牌")] =str;
    change_label_text(4,"PP_comboBox",QString::fromLocal8Bit("品牌:"));
}


void MainWindow::on_SL_comboBox_currentTextChanged()
{
    QString str=ui->SL_comboBox->currentText();
    mymap[QString::fromLocal8Bit("四类")] =str;
    change_label_text(5,"SL_comboBox",QString::fromLocal8Bit("四类名称:"));
    ui->JJ_comboBox->setCurrentText(SLmap[str]["季节"]);
    ui->ZYPL_comboBox->setCurrentText(SLmap[str]["主营品类"]);
    ui->DL_comboBox->setCurrentText(SLmap[str]["大类"]);
    ui->ZL_comboBox->setCurrentText(SLmap[str]["中类"]);
    ui->XL_comboBox->setCurrentText(SLmap[str]["小类"]);
}

void MainWindow::on_ZL_comboBox_currentTextChanged()
{
    QString str=ui->ZL_comboBox->currentText();
    mymap[QString::fromLocal8Bit("中类")] =str;
    change_label_text(7,"ZL_comboBox",QString::fromLocal8Bit("中类:"));
}

void MainWindow::on_XB_comboBox_currentTextChanged()
{
    QString str=ui->XB_comboBox->currentText();
    mymap[QString::fromLocal8Bit("性别")] =str;
    change_label_text(9,"XB_comboBox",QString::fromLocal8Bit("性别:"));
}

void MainWindow::on_YS_comboBox_currentTextChanged()
{
    QString str=ui->YS_comboBox->currentText();
    mymap[QString::fromLocal8Bit("颜色")] =str;
    change_label_text(10,"YS_comboBox",QString::fromLocal8Bit("颜色:"));
}

void MainWindow::on_GYS_comboBox_currentTextChanged()
{
    QString str=ui->GYS_comboBox->currentText();
    mymap[QString::fromLocal8Bit("供应商")] =str;
    change_label_text(11,"GYS_comboBox",QString::fromLocal8Bit("供应商:"));
}

void MainWindow::on_XL_comboBox_currentTextChanged()
{
    QString str=ui->XL_comboBox->currentText();
    mymap[QString::fromLocal8Bit("小类")] =str;
    change_label_text(8,"XL_comboBox",QString::fromLocal8Bit("小类:"));
}

void MainWindow::on_ZTJ_comboBox_currentTextChanged()
{
    QString str=ui->ZTJ_comboBox->currentText();
    mymap[QString::fromLocal8Bit("正特价")] =str;
    change_label_text(12,"ZTJ_comboBox",QString::fromLocal8Bit("正特价:"));
}



void MainWindow::on_JJ_comboBox_currentTextChanged()
{
    QString str=ui->JJ_comboBox->currentText();
    mymap[QString::fromLocal8Bit("季节")] =str;
    change_label_text(13,"JJ_comboBox",QString::fromLocal8Bit("季节:"));
}

void MainWindow::on_DL_comboBox_currentTextChanged()
{
    QString str=ui->DL_comboBox->currentText();
    mymap[QString::fromLocal8Bit("大类")] =str;
    change_label_text(6,"DL_comboBox",QString::fromLocal8Bit("大类:"));
}


void MainWindow::on_CGY_comboBox_currentTextChanged()
{
    QString str=ui->CGY_comboBox->currentText();
    mymap[QString::fromLocal8Bit("采购员")] =str;
    change_label_text(14,"CGY_comboBox",QString::fromLocal8Bit("采购员:"));
}

void MainWindow::on_NF_comboBox_currentTextChanged()
{
    QString str=ui->NF_comboBox->currentText();
    mymap[QString::fromLocal8Bit("年份")] =str;
    change_label_text(16,"NF_comboBox",QString::fromLocal8Bit("年份:"));
}

void MainWindow::on_CCZ_comboBox_currentTextChanged()
{
    QString str=ui->CCZ_comboBox->currentText();
    mymap[QString::fromLocal8Bit("尺寸组")] =str;
    change_label_text(17,"CCZ_comboBox",QString::fromLocal8Bit("尺寸组:"));
}

void MainWindow::on_CGXZ_comboBox_currentTextChanged()
{
    QString str=ui->CGXZ_comboBox->currentText();
    mymap[QString::fromLocal8Bit("采购性质")] =str;
    change_label_text(18,"CGXZ_comboBox",QString::fromLocal8Bit("采购性质:"));
}



void MainWindow::on_BX_comboBox_currentTextChanged()
{
    QString str=ui->BX_comboBox->currentText();
    mymap[QString::fromLocal8Bit("版型")] =str;
    change_label_text(19,"BX_comboBox",QString::fromLocal8Bit("版型:"));
}

void MainWindow::on_CKG_comboBox_currentTextChanged()
{
    QString str=ui->CKG_comboBox->currentText();
    mymap[QString::fromLocal8Bit("长宽高")] =str;
    change_label_text(20,"CKG_comboBox",QString::fromLocal8Bit("长宽高:"));
}

void MainWindow::on_NLD_comboBox_currentTextChanged()
{
    QString str=ui->NLD_comboBox->currentText();
    mymap[QString::fromLocal8Bit("年龄组")] =str;
    change_label_text(15,"NLD_comboBox",QString::fromLocal8Bit("年龄组:"));
}

void MainWindow::on_LXSH_comboBox_currentTextChanged()
{
    QString str=ui->LXSH_comboBox->currentText();
    mymap[QString::fromLocal8Bit("联系商户")] =str;
    change_label_text(21,"LXSH_comboBox",QString::fromLocal8Bit("联系商户:"));
}

void MainWindow::on_CM_comboBox_currentTextChanged()
{
    QString str=ui->CM_comboBox->currentText();
    mymap[QString::fromLocal8Bit("尺码")] =str;
    change_label_text(22,"CM_comboBox",QString::fromLocal8Bit("尺码:"));
}

void MainWindow::on_JYFS_comboBox_currentTextChanged()
{
    QString str=ui->JYFS_comboBox->currentText();
    mymap[QString::fromLocal8Bit("经营方式")] =str;
    change_label_text(23,"JYFS_comboBox",QString::fromLocal8Bit("经营方式:"));
}


