/***********************************
*  File: dpagemenu.cpp
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/

#include "dpage.h"
#include "ui_dpage.h"
#include <qdebug.h>

bool dpage::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->page_widget && event->type() == QEvent::ContextMenu)
    {
        label_menu->exec(QCursor::pos());

    }
    return QWidget::eventFilter(watched,event);
}

void dpage::add_click(){
    bool ck1=ui->label_ck1->isVisible();
    bool ck2=ui->label_ck2->isVisible();
    bool ck3=ui->label_ck3->isVisible();
    bool ck4=ui->label_ck4->isVisible();
    bool ck5=ui->label_ck5->isVisible();
    int x=(ui->ld_lineEdit->text().toInt())*5;
    int y=(ui->td_lineEdit->text().toInt())*5;
    int w=(ui->wd_lineEdit->text().toInt())*5;
    int h=(ui->hd_lineEdit->text().toInt())*5;
    int d=(ui->boxd_lineEdit->text().toUInt()*5);
    int pagew=(ui->W_lineEdit->text().toUInt()*5);
    int pageh=(ui->H_lineEdit->text().toUInt()*5);
    if(x+w >pagew || y+h > pageh)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("检测文本框越界，请重新设置文本框参数"));
        return;
    }
    if(ck1==false){
        ui->label_ck1->clear();
        ui->label_ck1->setGeometry(x,y+abs(4-d_member)*(h+d),w,h);
        ui->label_ck1->setStyleSheet("QLabel{border:1px dashed;background-color:rgb(192,192,192);}");
        ui->label_ck1->setVisible(true);
        d_member--;
        return;
    }
    else if(ck2==false){
        ui->label_ck2->clear();
        ui->label_ck2->setGeometry(x,y+abs(4-d_member)*(h+d),w,h);
        ui->label_ck2->setStyleSheet("QLabel{border:1px dashed;background-color:rgb(112,128,105);}");
        ui->label_ck2->setVisible(true);
        d_member--;
        return;
    }
    else if(ck3==false){
        ui->label_ck3->clear();
        ui->label_ck3->setGeometry(x,y+abs(4-d_member)*(h+d),w,h);
        ui->label_ck3->setStyleSheet("QLabel{border:1px dashed;background-color:rgb(128,128,105);}");
        ui->label_ck3->setVisible(true);
        d_member--;
        return;
    }
    else if(ck4==false){
        ui->label_ck4->clear();
        ui->label_ck4->setGeometry(x,y+abs(4-d_member)*(h+d),w,h);
        ui->label_ck4->setStyleSheet("QLabel{border:1px dashed;background-color:rgb(250,222,173);}");
        ui->label_ck4->setVisible(true);
        d_member--;
        return;
    }
    else if(ck5==false){
        ui->label_ck5->clear();
        ui->label_ck5->setGeometry(x,y+abs(4-d_member)*(h+d),w,h);
        ui->label_ck5->setStyleSheet("QLabel{border:1px dashed;background-color:rgb(255,245,238);}");
        ui->label_ck5->setVisible(true);
        d_member--;
        return;
    }
    else{
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("最多添加五个文本区域"));
        return;
    }
}

void dpage::del_click(){
    if(ck1==1){
        ui->label_ck1->setVisible(false);
        uncheckbox("label_ck1");
        ckmap["label_ck1"]=0;
        d_member++;
        return;
    }
    else if (ck1==2) {
        ui->label_ck2->setVisible(false);
        uncheckbox("label_ck2");
        ckmap["label_ck2"]=0;
        d_member++;
        return;
    }
    else if (ck1==3) {
        ui->label_ck3->setVisible(false);
        uncheckbox("label_ck3");
        ckmap["label_ck3"]=0;
        d_member++;
        return;
    }
    else if (ck1==4) {
        ui->label_ck4->setVisible(false);
        uncheckbox("label_ck4");
        ckmap["label_ck4"]=0;
        d_member++;
        return;
    }
    else if (ck1==5) {
        ui->label_ck5->setVisible(false);
        uncheckbox("label_ck5");
        ckmap["label_ck5"]=0;
        d_member++;
        return;
    }
    else if (ck1==6) {
        ui->BAR_checkBox->setCheckState(Qt::Unchecked);
        return;
    }
    else if (ck1==7) {
        ui->DATA_checkBox->setCheckState(Qt::Unchecked);
        return;
    }
    else{
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要删除的文本区域"));
        return;
    }
}


void dpage::left_click(){
    if(ck1==1){
        int y=ui->label_ck1->pos().y();
        ui->label_ck1->move(0,y);
        return;
    }
    else if(ck1==2){
        int y=ui->label_ck2->pos().y();
        ui->label_ck2->move(0,y);
        return;
    }
    else if(ck1==3){
        int y=ui->label_ck3->pos().y();
        ui->label_ck3->move(0,y);
        return;
    }
    else if(ck1==4){
        int y=ui->label_ck4->pos().y();
        ui->label_ck4->move(0,y);
        return;
    }
    else if(ck1==5){
        int y=ui->label_ck5->pos().y();
        ui->label_ck5->move(0,y);
        return;
    }
    else if(ck1==6){
        int y=ui->label_bar->pos().y();
        ui->label_bar->move(0,y);
        return;
    }
    else if(ck1==7){
        int y=ui->label_data->pos().y();
        ui->label_data->move(0,y);
        return;
    }
    else{
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择文本区域"));
        return;
    }
}

void dpage::center_click(){

    int pagew=ui->page_widget->width();
    if(ck1==1){
        int labelw=ui->label_ck1->width();
        int x=(pagew-labelw)/2;
        int y=ui->label_ck1->pos().y();
        ui->label_ck1->move(x,y);
        return;
    }
    else if(ck1==2){
        int labelw=ui->label_ck2->width();
        int x=(pagew-labelw)/2;
        int y=ui->label_ck2->pos().y();
        ui->label_ck2->move(x,y);
        return;
    }
    else if(ck1==3){
        int labelw=ui->label_ck3->width();
        int x=(pagew-labelw)/2;
        int y=ui->label_ck3->pos().y();
        ui->label_ck3->move(x,y);
        return;
    }
    else if(ck1==4){
        int labelw=ui->label_ck4->width();
        int x=(pagew-labelw)/2;
        int y=ui->label_ck4->pos().y();
        ui->label_ck4->move(x,y);
        return;
    }
    else if(ck1==5){
        int labelw=ui->label_ck5->width();
        int x=(pagew-labelw)/2;
        int y=ui->label_ck5->pos().y();
        ui->label_ck5->move(x,y);
        return;
    }
    else if(ck1==6){
        int labelw=ui->label_bar->width();
        int x=(pagew-labelw)/2;
        int y=ui->label_bar->pos().y();
        ui->label_bar->move(x,y);
        return;
    }
    else if(ck1==7){
        int labelw=ui->label_data->width();
        int x=(pagew-labelw)/2;
        int y=ui->label_data->pos().y();
        ui->label_data->move(x,y);
        return;
    }
    else{
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择文本区域"));
        return;
    }
}

void dpage::right_click(){
    int pagew=ui->page_widget->width();
    if(ck1==1){
        int labelw=ui->label_ck1->width();
        int x=pagew-labelw;
        int y=ui->label_ck1->pos().y();
        ui->label_ck1->move(x,y);
        return;
    }
    else if(ck1==2){
        int labelw=ui->label_ck2->width();
        int x=pagew-labelw;
        int y=ui->label_ck2->pos().y();
        ui->label_ck2->move(x,y);
        return;
    }
    else if(ck1==3){
        int labelw=ui->label_ck3->width();
        int x=pagew-labelw;
        int y=ui->label_ck3->pos().y();
        ui->label_ck3->move(x,y);
        return;
    }
    else if(ck1==4){
        int labelw=ui->label_ck4->width();
        int x=pagew-labelw;
        int y=ui->label_ck4->pos().y();
        ui->label_ck4->move(x,y);
        return;
    }
    else if(ck1==5){
        int labelw=ui->label_ck5->width();
        int x=pagew-labelw;
        int y=ui->label_ck5->pos().y();
        ui->label_ck5->move(x,y);
        return;
    }
    else if(ck1==6){
        int labelw=ui->label_bar->width();
        int x=pagew-labelw;
        int y=ui->label_bar->pos().y();
        ui->label_bar->move(x,y);
        return;
    }
    else if(ck1==7){
        int labelw=ui->label_data->width();
        int x=pagew-labelw;
        int y=ui->label_data->pos().y();
        ui->label_data->move(x,y);
        return;
    }
    else{
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择文本区域"));
        return;
    }
}
