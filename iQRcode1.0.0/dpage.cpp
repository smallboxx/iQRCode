/***********************************
*  File: dpage.cpp
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/

#include "dpage.h"
#include "ui_dpage.h"
#include <QtDebug>
#include "customlabel.h"
#include <QDebug>
QString pagewidth;
QString pageheight;
extern QString filepath;
#include "mainwindow.h"
#include <QAction>
#include <QWidgetAction>
extern MainWindow* mywin;
dpage::dpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dpage)
{
    ui->setupUi(this);
    maindpagePtr=(void*)parent;
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    d_member=-1;
    label_menu = new QMenu;
    label_menu->setWindowFlags(label_menu->windowFlags() | Qt::FramelessWindowHint);
    QWidgetAction* pWdtAction1 = new QWidgetAction(label_menu);
    pWdtAction1->setDefaultWidget(new QLabel(QString::fromLocal8Bit("文本框设置:"), label_menu));
    QAction* m_add = new QAction(QString::fromLocal8Bit("添加"));
    QAction* m_del = new QAction(QString::fromLocal8Bit("删除"));
    QAction* m_left = new QAction(QString::fromLocal8Bit("左对齐"));
    QAction* m_center = new QAction(QString::fromLocal8Bit("居中"));
    QAction* m_right = new QAction(QString::fromLocal8Bit("右对齐"));
    //右键菜单添加属性
    label_menu->addAction(pWdtAction1);
    label_menu->addAction(m_add);
    label_menu->addAction(m_del);
    label_menu->addAction(m_left);
    label_menu->addAction(m_center);
    label_menu->addAction(m_right);
    connect(m_add,SIGNAL(triggered(bool)),this,SLOT(add_click()));
    connect(m_del,SIGNAL(triggered(bool)),this,SLOT(del_click()));
    connect(m_left,SIGNAL(triggered(bool)),this,SLOT(left_click()));
    connect(m_center,SIGNAL(triggered(bool)),this,SLOT(center_click()));
    connect(m_right,SIGNAL(triggered(bool)),this,SLOT(right_click()));
    ui->page_widget->installEventFilter(this);
    connect(this,SIGNAL(reload()),mywin,SLOT(set_page_widget()));

    setWindowTitle(QString::fromLocal8Bit("标签设计"));
    design=true;
    MR=false;
    boundingflag=true;
    //代表当前文本框显示的属性
    ckmap["label_ck1"]=0;
    ckmap["label_ck2"]=0;
    ckmap["label_ck3"]=0;
    ckmap["label_ck4"]=0;
    ckmap["label_ck5"]=0;
    //当前文本字体属性
    fontmap["label_ck1"]=0;
    fontmap["label_ck2"]=0;
    fontmap["label_ck3"]=0;
    fontmap["label_ck4"]=0;
    fontmap["label_ck5"]=0;
    //当前文本字号属性
    sizemap["label_ck1"]="16";
    sizemap["label_ck2"]="16";
    sizemap["label_ck3"]="16";
    sizemap["label_ck4"]="16";
    sizemap["label_ck5"]="16";
    //当前文本框布局属性
    alignmap["label_ck1"]=0;
    alignmap["label_ck1"]=0;
    alignmap["label_ck1"]=0;
    alignmap["label_ck1"]=0;
    alignmap["label_ck1"]=0;
    //设置文本框样式
    ui->label_ck2->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
    ui->label_ck3->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
    ui->label_ck4->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
    ui->label_ck5->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
    ui->label_bar->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
    ui->label_data->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
    ui->label_ck1->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
    //标签大小
    ui->page_widget->setStyleSheet("background:white");
    load(MR);
    //响应文本框选中
    connect(ui->label_ck1,&CustomLabel::clicked,this,[=](){

        ck1=1;
        ui->label_ck2->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck3->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck4->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck5->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_data->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_bar->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck1->setStyleSheet("QLabel{border:1px dashed;background-color:#CCCCCC}");
        ui->label_ck1->raise();
        ui->font_comboBox->setCurrentIndex(fontmap["label_ck1"]);
        ui->size_comboBox->setCurrentText(sizemap["label_ck1"]);
        ui->Align_comboBox->setCurrentIndex(alignmap["label_ck1"]);

    });

    connect(ui->label_ck2,&CustomLabel::clicked,this,[=](){
        ck1=2;
        ui->label_ck1->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck3->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck4->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck5->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_data->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_bar->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck2->setStyleSheet("QLabel{border:1px dashed;background-color:#CCCCCC}");
        ui->label_ck2->raise();
        ui->font_comboBox->setCurrentIndex(fontmap["label_ck2"]);
        ui->size_comboBox->setCurrentText(sizemap["label_ck2"]);
        ui->Align_comboBox->setCurrentIndex(alignmap["label_ck2"]);
    });

    connect(ui->label_ck3,&CustomLabel::clicked,this,[=](){
        ck1=3;
        ui->label_ck2->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck1->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck4->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck5->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_data->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_bar->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck3->setStyleSheet("QLabel{border:1px dashed;background-color:#CCCCCC}");
        ui->label_ck3->raise();
        ui->font_comboBox->setCurrentIndex(fontmap["label_ck3"]);
        ui->size_comboBox->setCurrentText(sizemap["label_ck3"]);
        ui->Align_comboBox->setCurrentIndex(alignmap["label_ck3"]);
    });

    connect(ui->label_ck4,&CustomLabel::clicked,this,[=](){
        ck1=4;
        ui->label_ck2->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck3->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck1->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck5->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_data->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_bar->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck4->setStyleSheet("QLabel{border:1px dashed;background-color:#CCCCCC}");
        ui->label_ck4->raise();
        ui->font_comboBox->setCurrentIndex(fontmap["label_ck4"]);
        ui->size_comboBox->setCurrentText(sizemap["label_ck4"]);
        ui->Align_comboBox->setCurrentIndex(alignmap["label_ck4"]);
    });

    connect(ui->label_ck5,&CustomLabel::clicked,this,[=](){
        ck1=5;
        ui->label_ck2->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck3->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck4->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck1->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_data->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_bar->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck5->setStyleSheet("QLabel{border:1px dashed;background-color:#CCCCCC}");
        ui->label_ck5->raise();
        ui->font_comboBox->setCurrentIndex(fontmap["label_ck5"]);
        ui->size_comboBox->setCurrentText(sizemap["label_ck5"]);
        ui->Align_comboBox->setCurrentIndex(alignmap["label_ck5"]);
    });
    connect(ui->label_bar,&CustomLabel::clicked,this,[=](){
        ck1=6;
        ui->label_ck2->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck3->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck4->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck1->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck5->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_data->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_bar->setStyleSheet("QLabel{border:1px dashed;background-color:#CCCCCC}");
        ui->label_bar->raise();
        ui->Align_comboBox->setCurrentIndex(alignmap["label_bar"]);
    });
    connect(ui->label_data,&CustomLabel::clicked,this,[=](){
        ck1=7;
        ui->label_ck2->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck3->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck4->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck1->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_ck5->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_bar->setStyleSheet("QLabel{border:1px dashed;background:transparent}");
        ui->label_data->setStyleSheet("QLabel{border:1px dashed;background-color:#CCCCCC}");
        ui->label_data->raise();
        ui->Align_comboBox->setCurrentIndex(alignmap["label_data"]);
    });
}

dpage::~dpage()
{
    delete ui;
}


void dpage::on_W_lineEdit_editingFinished()
{
    if(boundingflag){
        int check=ui->W_lineEdit->text().toInt();
        int check2=ui->H_lineEdit->text().toInt();
        if(check > 55 || check < 19)
        {
            boundingflag=false;
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请输入允许的标签宽高:宽20~54,高20~70"));
            ui->W_lineEdit->setText( pagewidth);
            return;
        }
        int pagew=check*5;
        int pageh=check2*5;
        int sw=pagew/1.5;
        int sh=pageh/10;
        QPoint tt(0,0);
        QPoint rm(pagew,pageh);
        QRect rect(tt,rm);
        QPoint lack1(ui->label_ck1->width()+ui->label_ck1->x(),ui->label_ck1->height()+ui->label_ck1->y());
        QPoint lack2(ui->label_ck2->width()+ui->label_ck2->x(),ui->label_ck2->height()+ui->label_ck2->y());
        QPoint lack3(ui->label_ck3->width()+ui->label_ck3->x(),ui->label_ck3->height()+ui->label_ck3->y());
        QPoint lack4(ui->label_ck4->width()+ui->label_ck4->x(),ui->label_ck4->height()+ui->label_ck4->y());
        QPoint lack5(ui->label_ck5->width()+ui->label_ck5->x(),ui->label_ck5->height()+ui->label_ck5->y());
        QPoint lackbar(ui->label_bar->width()+ui->label_bar->x(),ui->label_bar->height()+ui->label_bar->y());
        QPoint lackdata(ui->label_data->width()+ui->label_data->x(),ui->label_data->height()+ui->label_data->y());
        if(rect.contains(ui->label_ck1->pos())==false || rect.contains(lack1)==false || rect.contains(ui->label_ck2->pos())==false || rect.contains(lack2)==false
                || rect.contains(ui->label_ck3->pos())==false || rect.contains(lack3)==false || rect.contains(ui->label_ck4->pos())==false || rect.contains(lack4)==false
                || rect.contains(ui->label_ck5->pos())==false || rect.contains(lack5)==false || rect.contains(ui->label_bar->pos())==false || rect.contains(lackbar)==false
                || rect.contains(ui->label_data->pos())==false || rect.contains(lackdata)==false )
        {
            boundingflag=false;
            QMessageBox::StandardButton box;
            box = QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("允许的标签宽高范围:宽20~54,高20~70。检测到您将修改的宽高数据可能会使文本框越界，继续则会重置越界文本框位置，取消则恢复？"), QMessageBox::Yes|QMessageBox::No);
            if(box==QMessageBox::No)
            {
                ui->W_lineEdit->setText( pagewidth);
                return;
            }
        }
        pagewidth=ui->W_lineEdit->text();
        ui->page_widget->resize(pagewidth.toInt()*5,pageheight.toInt()*5);
        if(rect.contains(ui->label_ck1->pos())==false || rect.contains(lack1)==false){
            ui->label_ck1->setGeometry(0,0,sw,sh);
        }
        if(rect.contains(ui->label_ck2->pos())==false || rect.contains(lack2)==false){
            ui->label_ck2->setGeometry(0,sh*2,sw,sh);
        }
        if(rect.contains(ui->label_ck3->pos())==false || rect.contains(lack3)==false){
            ui->label_ck3->setGeometry(0,sh*3,sw,sh);
        }
        if(rect.contains(ui->label_ck4->pos())==false || rect.contains(lack4)==false){
            ui->label_ck4->setGeometry(0,sh*4,sw,sh);
        }
        if(rect.contains(ui->label_ck5->pos())==false || rect.contains(lack5)==false){
            ui->label_ck5->setGeometry(0,sh*5,sw,sh);
        }
        if(rect.contains(ui->label_bar->pos())==false || rect.contains(lackbar)==false){
            ui->label_bar->setGeometry(0,sh*6,sw,sh*2);
        }
        if(rect.contains(ui->label_data->pos())==false || rect.contains(lackdata)==false){
            ui->label_data->setGeometry(0,sh*8,sw,sh);
        }
    }
    else{
        boundingflag=true;
        return;
    }
}

void dpage::on_H_lineEdit_editingFinished()
{
    if(boundingflag){
        int check=ui->H_lineEdit->text().toInt();
        int check2=ui->W_lineEdit->text().toInt();
        if(check > 71 || check < 19)
        {
            boundingflag=false;
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请输入允许的标签宽高:宽20~54,高20~70"));
            ui->H_lineEdit->setText( pageheight);
            return;
        }
        int pagew=check2*5;
        int pageh=check*5;
        int sw=pagew/1.5;
        int sh=pageh/10;
        QPoint tt(0,0);
        QPoint rm(pagew,pageh);
        QRect rect(tt,rm);
        QPoint lack1(ui->label_ck1->width()+ui->label_ck1->x(),ui->label_ck1->height()+ui->label_ck1->y());
        QPoint lack2(ui->label_ck2->width()+ui->label_ck2->x(),ui->label_ck2->height()+ui->label_ck2->y());
        QPoint lack3(ui->label_ck3->width()+ui->label_ck3->x(),ui->label_ck3->height()+ui->label_ck3->y());
        QPoint lack4(ui->label_ck4->width()+ui->label_ck4->x(),ui->label_ck4->height()+ui->label_ck4->y());
        QPoint lack5(ui->label_ck5->width()+ui->label_ck5->x(),ui->label_ck5->height()+ui->label_ck5->y());
        QPoint lackbar(ui->label_bar->width()+ui->label_bar->x(),ui->label_bar->height()+ui->label_bar->y());
        QPoint lackdata(ui->label_data->width()+ui->label_data->x(),ui->label_data->height()+ui->label_data->y());
        if(rect.contains(ui->label_ck1->pos())==false || rect.contains(lack1)==false || rect.contains(ui->label_ck2->pos())==false || rect.contains(lack2)==false
                || rect.contains(ui->label_ck3->pos())==false || rect.contains(lack3)==false || rect.contains(ui->label_ck4->pos())==false || rect.contains(lack4)==false
                || rect.contains(ui->label_ck5->pos())==false || rect.contains(lack5)==false || rect.contains(ui->label_bar->pos())==false || rect.contains(lackbar)==false
                || rect.contains(ui->label_data->pos())==false || rect.contains(lackdata)==false )
        {
            boundingflag=false;
            QMessageBox::StandardButton box;
            box = QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("允许的标签宽高范围:宽20~54,高20~70。检测到您将修改的宽高数据可能会使文本框越界，继续则会重置越界文本框位置，取消则恢复？"), QMessageBox::Yes|QMessageBox::No);
            if(box==QMessageBox::No)
            {
                ui->H_lineEdit->setText( pageheight);
                return;
            }
        }
        pageheight=ui->H_lineEdit->text();
        ui->page_widget->resize(pagewidth.toInt()*5,pageheight.toInt()*5);
        if(rect.contains(ui->label_ck1->pos())==false || rect.contains(lack1)==false){
            ui->label_ck1->setGeometry(0,0,sw,sh);
        }
        if(rect.contains(ui->label_ck2->pos())==false || rect.contains(lack2)==false){
            ui->label_ck2->setGeometry(0,sh*2,sw,sh);
        }
        if(rect.contains(ui->label_ck3->pos())==false || rect.contains(lack3)==false){
            ui->label_ck3->setGeometry(0,sh*3,sw,sh);
        }
        if(rect.contains(ui->label_ck4->pos())==false || rect.contains(lack4)==false){
            ui->label_ck4->setGeometry(0,sh*4,sw,sh);
        }
        if(rect.contains(ui->label_ck5->pos())==false || rect.contains(lack5)==false){
            ui->label_ck5->setGeometry(0,sh*5,sw,sh);
        }
        if(rect.contains(ui->label_bar->pos())==false || rect.contains(lackbar)==false){
            ui->label_bar->setGeometry(0,sh*6,sw,sh*2);
        }
        if(rect.contains(ui->label_data->pos())==false || rect.contains(lackdata)==false){
            ui->label_data->setGeometry(0,sh*8,sw,sh);
        }
    }
    else{
        boundingflag=true;
        return;
    }
}

void dpage::uncheckbox(QString label_str)
{
    if (ckmap[label_str]==1)
    {
        ui->PM_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==2)
    {
        ui->ZYPL_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==3)
    {
        ui->HH_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==4)
    {
        ui->PP_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==5)
    {
        ui->SLMC_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==6)
    {
        ui->DL_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==7)
    {
        ui->ZL_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==8)
    {
        ui->XL_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==9)
    {
        ui->XB_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==10)
    {
        ui->YS_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==11)
    {
        ui->GYS_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==12)
    {
        ui->ZTJ_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==13)
    {
        ui->JJ_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==14)
    {
        ui->CGY_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==15)
    {
        ui->NLD_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==16)
    {
        ui->NF_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==17)
    {
        ui->CCZ_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==18)
    {
        ui->CGXZ_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==19)
    {
        ui->BX_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==20)
    {
        ui->CKG_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==21)
    {
        ui->LXSH_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==22)
    {
        ui->CM_checkBox->setCheckState(Qt::Unchecked);
    }

    if (ckmap[label_str]==23)
    {
        ui->JYFS_checkBox->setCheckState(Qt::Unchecked);
    }
}

void dpage::add_items(bool status,QString str,int ck,QString label_str){
    //勾选属性
    if (status==true)
    {
        if (ck1==1)//第一个文本框
        {
            uncheckbox(label_str);//取消勾选该文本框之前显示的属性
            ui->label_ck1->setText(str);//将现在所勾选的属性进行显示
            ckmap["label_ck1"]=ck;//记录该文本区域所存的属性是什么
        }
        if (ck1==2)
        {
            uncheckbox(label_str);
            ui->label_ck2->setText(str);
            ckmap["label_ck2"]=ck;
        }

        if (ck1==3)
        {
            uncheckbox(label_str);
            ui->label_ck3->setText(str);
            ckmap["label_ck3"]=ck;
        }

        if (ck1==4)
        {
            uncheckbox(label_str);
            ui->label_ck4->setText(str);
            ckmap["label_ck4"]=ck;
        }

        if (ck1==5)
        {
            uncheckbox(label_str);
            ui->label_ck5->setText(str);
            ckmap["label_ck5"]=ck;
        }
    }
    //实现同一文本框，取消勾选该属性，文本清楚
    else {
        for (auto iter = ckmap.begin();iter !=ckmap.end();iter++)
        {
            if (ck==iter->second)
            {
               auto ind = findChild<CustomLabel*>(iter->first);
               ind->clear();
               iter->second=0;
            }
        }

    }
}


QString dpage::get_label(int ck1)
{
    QString str;
    if (ck1==1)
    {
        str="label_ck1";
        return str;
    }
    if (ck1==2)
    {
        str="label_ck2";
        return str;
    }
    if (ck1==3)
    {
        str="label_ck3";
        return str;
    }
    if (ck1==4)
    {
        str="label_ck4";
        return str;
    }
    if (ck1==5)
    {
        str="label_ck5";
        return str;
    }
    else
    {
        str="";
        QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择正确的文本区域"));
        return str;
    }
}

void dpage::on_PM_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->PM_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->PM_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("品名：休闲短裤");
        QString label_str=get_label(ck1);
        add_items(status,str,1,label_str);
    }
}

void dpage::on_ZYPL_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->ZYPL_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->ZYPL_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("主营品类:鞋");
        QString label_str=get_label(ck1);
        add_items(status,str,2,label_str);
    }

}

void dpage::on_HH_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->HH_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->HH_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("货号：22-213");
        QString label_str=get_label(ck1);
        add_items(status,str,3,label_str);
    }
}

void dpage::on_PP_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->PP_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->PP_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("品牌：向阳木");
        QString label_str=get_label(ck1);
        add_items(status,str,4,label_str);
    }
}

void dpage::on_SLMC_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->SLMC_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->SLMC_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("四类名称:xxxx");
        QString label_str=get_label(ck1);
        add_items(status,str,5,label_str);
    }
}

void dpage::on_DL_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->DL_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->DL_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("大类:上衣");
        QString label_str=get_label(ck1);
        add_items(status,str,6,label_str);
    }
}

void dpage::on_ZL_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->ZL_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->ZL_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("中类：牛仔裤");
        QString label_str=get_label(ck1);
        add_items(status,str,7,label_str);
    }
}

void dpage::on_XL_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->XL_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->XL_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("小类:直筒牛仔裤");
        QString label_str=get_label(ck1);
        add_items(status,str,8,label_str);
    }
}

void dpage::on_XB_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->XB_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->XB_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("性别：女");
        QString label_str=get_label(ck1);
        add_items(status,str,9,label_str);
    }
}

void dpage::on_YS_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->YS_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->YS_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("颜色：黑色");
        QString label_str=get_label(ck1);
        add_items(status,str,10,label_str);
    }
}

void dpage::on_GYS_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->GYS_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->GYS_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("供应商：浙商产业园");
        QString label_str=get_label(ck1);
        add_items(status,str,11,label_str);
    }
}

void dpage::on_ZTJ_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->ZTJ_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->ZTJ_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("正特价：特价");
        QString label_str=get_label(ck1);
        add_items(status,str,12,label_str);
    }
}

void dpage::on_JJ_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->JJ_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->JJ_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("季节：四季");
        QString label_str=get_label(ck1);
        add_items(status,str,13,label_str);
    }
}

void dpage::on_CGY_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->CGY_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->CGY_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("采购员：小王");
        QString label_str=get_label(ck1);
        add_items(status,str,14,label_str);
    }
}

void dpage::on_NLD_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->NLD_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->NLD_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("年龄段：12~16");
        QString label_str=get_label(ck1);
        add_items(status,str,15,label_str);
    }
}

void dpage::on_NF_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->NF_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->NF_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("年份：2022");
        QString label_str=get_label(ck1);
        add_items(status,str,16,label_str);
    }
}

void dpage::on_CCZ_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->CCZ_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->CCZ_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("尺寸组：L");
        QString label_str=get_label(ck1);
        add_items(status,str,17,label_str);
    }
}

void dpage::on_CGXZ_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->CGXZ_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->CGXZ_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("采购性质：内采");
        QString label_str=get_label(ck1);
        add_items(status,str,18,label_str);
    }
}

void dpage::on_BX_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->BX_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->BX_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("版型：宽松");
        QString label_str=get_label(ck1);
        add_items(status,str,19,label_str);
    }
}

void dpage::on_CKG_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->CKG_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->CKG_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("长宽高：30");
        QString label_str=get_label(ck1);
        add_items(status,str,20,label_str);
    }
}

void dpage::on_LXSH_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->LXSH_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->LXSH_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("联系商户：XXXX");
        QString label_str=get_label(ck1);
        add_items(status,str,21,label_str);
    }
}

void dpage::on_CM_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->CM_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->CM_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("尺码：38");
        QString label_str=get_label(ck1);
        add_items(status,str,22,label_str);
    }
}

void dpage::on_JYFS_checkBox_stateChanged()
{
    if(design)
    {
        bool status=ui->JYFS_checkBox->isChecked();
        if (ck1==0 && status==1)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("tips"),QString::fromLocal8Bit("请选择要替换的文本区域"));
            ui->JYFS_checkBox->setCheckState(Qt::Unchecked);
            return;
        }
        QString str=QString::fromLocal8Bit("营销方式：特价促销");
        QString label_str=get_label(ck1);
        add_items(status,str,23,label_str);
    }
}

void dpage::on_BAR_checkBox_stateChanged(int arg1)
{
    if(design){
        arg1=ui->BAR_checkBox->isChecked();
        if (arg1==1)
        {
            int w=(ui->page_widget->width())/1.5;
            int h=(ui->page_widget->height())/10;
            ui->label_bar->setGeometry(0,h*5,w,h*2);
            ui->label_bar->setVisible(true);
            QFile file("./barcode.png");
            file.open(QIODevice::ReadOnly);
            QByteArray data = file.readAll();
            QImage img = QImage::fromData(data);
            int w_la=ui->label_bar->width();
            int h_la=ui->label_bar->height();
            img = img.scaled(w_la,h_la,Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->label_bar->setPixmap(QPixmap::fromImage(img));
            update();
        }
        else
        {
            ui->label_bar->setVisible(false);
            ck1=0;
        }
    }
}

void dpage::on_DATA_checkBox_stateChanged(int arg1)
{
    if(design){
        arg1=ui->DATA_checkBox->isChecked();
        if (arg1==1)
        {
            int w=(ui->page_widget->width())/1.5;
            int h=(ui->page_widget->height())/10;
            ui->label_data->setGeometry(0,h*7,w,h);
            ui->label_data->setVisible(true);
            QFile file("./data.png");
            file.open(QIODevice::ReadOnly);
            QByteArray data = file.readAll();
            QImage img = QImage::fromData(data);
            int w_la=ui->label_data->width();
            int h_la=ui->label_data->height();
            img = img.scaled(w_la,h_la,Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->label_data->setPixmap(QPixmap::fromImage(img));
            update();
        }
        else
        {
            ui->label_data->setVisible(false);
            ck1=0;
        }
    }
}

QString dpage::get_font(int index)
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



void dpage::on_font_comboBox_currentIndexChanged(int index)
{
    QFont font;
    QString check_font;
    if (ck1==1)
    {
        font.setPixelSize(sizemap["label_ck1"].toFloat());
        check_font=get_font(index);
        font.setFamily(check_font);
        ui->label_ck1->setFont(font);
        fontmap["label_ck1"]=index;
    }
    if (ck1==2)
    {
        font.setPixelSize(sizemap["label_ck2"].toFloat());
        check_font=get_font(index);
        font.setFamily(check_font);
        ui->label_ck2->setFont(font);
        fontmap["label_ck2"]=index;

    }
    if (ck1==3)
    {
        font.setPixelSize(sizemap["label_ck3"].toFloat());
        check_font=get_font(index);
        font.setFamily(check_font);
        ui->label_ck3->setFont(font);
        fontmap["label_ck3"]=index;

    }
    if (ck1==4)
    {
        font.setPixelSize(sizemap["label_ck4"].toFloat());
        check_font=get_font(index);
        font.setFamily(check_font);
        ui->label_ck4->setFont(font);
        fontmap["label_ck4"]=index;


    }
    if (ck1==5)
    {
        font.setPixelSize(sizemap["label_ck5"].toFloat());
        check_font=get_font(index);
        font.setFamily(check_font);
        ui->label_ck5->setFont(font);
        fontmap["label_ck5"]=index;

    }
    //qDebug()<<fontmap;
}

void dpage::on_size_comboBox_currentTextChanged(const QString &arg1)
{
    QFont size;
    QString check_font;
    //qDebug()<<arg1;
    if (ck1==1)
    {
        check_font=get_font(fontmap["label_ck1"]);
        size.setFamily(check_font);
        size.setPixelSize(arg1.toFloat());
        ui->label_ck1->setFont(size);
        sizemap["label_ck1"]=ui->size_comboBox->currentText();

    }
    if (ck1==2)
    {
        check_font=get_font(fontmap["label_ck2"]);
        size.setFamily(check_font);
        size.setPixelSize(arg1.toFloat());
        ui->label_ck2->setFont(size);
        sizemap["label_ck2"]=ui->size_comboBox->currentText();
    }
    if (ck1==3)
    {
        check_font=get_font(fontmap["label_ck3"]);
        size.setFamily(check_font);
        size.setPixelSize(arg1.toFloat());
        ui->label_ck3->setFont(size);
        sizemap["label_ck3"]=ui->size_comboBox->currentText();
    }
    if (ck1==4)
    {
        check_font=get_font(fontmap["label_ck4"]);
        size.setFamily(check_font);
        size.setPixelSize(arg1.toFloat());
        ui->label_ck4->setFont(size);
        sizemap["label_ck4"]=ui->size_comboBox->currentText();
    }
    if (ck1==5)
    {
        check_font=get_font(fontmap["label_ck5"]);
        size.setFamily(check_font);
        size.setPixelSize(arg1.toFloat());
        ui->label_ck5->setFont(size);
        sizemap["label_ck5"]=ui->size_comboBox->currentText();
    }
}

void dpage::on_Align_comboBox_currentIndexChanged(int index)
{
    if (ck1==1)
    {
        if(index==0)
        {
            ui->label_ck1->setAlignment(Qt::AlignLeft);
            alignmap["label_ck1"]=0;
        }
        if(index==1)
        {
            ui->label_ck1->setAlignment(Qt::AlignCenter);
            alignmap["label_ck1"]=1;
        }
        if(index==2)
        {
            ui->label_ck1->setAlignment(Qt::AlignRight);
            alignmap["label_ck1"]=2;
        }
    }
    if (ck1==2)
    {
        if(index==0)
        {
            ui->label_ck2->setAlignment(Qt::AlignLeft);
            alignmap["label_ck2"]=0;
        }
        if(index==1)
        {
            ui->label_ck2->setAlignment(Qt::AlignCenter);
            alignmap["label_ck2"]=1;
        }
        if(index==2)
        {
            ui->label_ck2->setAlignment(Qt::AlignRight);
            alignmap["label_ck2"]=2;
        }
    }
    if (ck1==3)
    {
        if(index==0)
        {
            ui->label_ck3->setAlignment(Qt::AlignLeft);
            alignmap["label_ck3"]=0;
        }
        if(index==1)
        {
            ui->label_ck3->setAlignment(Qt::AlignCenter);
            alignmap["label_ck3"]=1;
        }
        if(index==2)
        {
            ui->label_ck3->setAlignment(Qt::AlignRight);
            alignmap["label_ck3"]=2;
        }
    }
    if (ck1==4)
    {
        if(index==0)
        {
            ui->label_ck4->setAlignment(Qt::AlignLeft);
            alignmap["label_ck4"]=0;
        }
        if(index==1)
        {
            ui->label_ck4->setAlignment(Qt::AlignCenter);
            alignmap["label_ck4"]=1;
        }
        if(index==2)
        {
            ui->label_ck4->setAlignment(Qt::AlignRight);
            alignmap["label_ck4"]=2;
        }
    }
    if (ck1==5)
    {
        if(index==0)
        {
            ui->label_ck5->setAlignment(Qt::AlignLeft);
            alignmap["label_ck5"]=0;
        }
        if(index==1)
        {
            ui->label_ck5->setAlignment(Qt::AlignCenter);
            alignmap["label_ck5"]=1;
        }
        if(index==2)
        {
            ui->label_ck5->setAlignment(Qt::AlignRight);
            alignmap["label_ck5"]=2;
        }
    }
    if (ck1==6)
    {
        if(index==0)
        {
            ui->label_bar->setAlignment(Qt::AlignLeft);
            alignmap["label_bar"]=0;
        }
        if(index==1)
        {
            ui->label_bar->setAlignment(Qt::AlignCenter);
            alignmap["label_bar"]=1;
        }
        if(index==2)
        {
            ui->label_bar->setAlignment(Qt::AlignRight);
            alignmap["label_bar"]=2;
        }
    }
    if (ck1==7)
    {
        if(index==0)
        {
            ui->label_data->setAlignment(Qt::AlignLeft);
            alignmap["label_data"]=0;
        }
        if(index==1)
        {
            ui->label_data->setAlignment(Qt::AlignCenter);
            alignmap["label_data"]=1;
        }
        if(index==2)
        {
            ui->label_data->setAlignment(Qt::AlignRight);
            alignmap["label_data"]=2;
        }
    }
}


void dpage::set_labeltext(int at_index,int label_index)
{
    if(label_index==1){
        QString str;
        if (at_index==1)
        {
            ui->PM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品名：休闲短裤");
        }

        if (at_index==2)
        {
            ui->ZYPL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("主营品类:鞋");
        }

        if (at_index==3)
        {
            ui->HH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("货号：22-213");
        }

        if (at_index==4)
        {
            ui->PP_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品牌：向阳木");
        }

        if (at_index==5)
        {
            ui->SLMC_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("四类名称:xxxx");
        }

        if (at_index==6)
        {
            ui->DL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("大类:上衣");
        }

        if (at_index==7)
        {
            ui->ZL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("中类：牛仔裤");
        }

        if (at_index==8)
        {
            ui->XL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("小类:直筒牛仔裤");
        }

        if (at_index==9)
        {
            ui->XB_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("性别：女");
        }

        if (at_index==10)
        {
            ui->YS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("颜色：黑色");
        }

        if (at_index==11)
        {
            ui->GYS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("供应商：浙商产业园");
        }

        if (at_index==12)
        {
            ui->ZTJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("正特价：特价");
        }

        if (at_index==13)
        {
            ui->JJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("季节：四季");
        }

        if (at_index==14)
        {
            ui->CGY_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购员：小王");
        }

        if (at_index==15)
        {
            ui->NLD_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年龄段：12~16");
        }

        if (at_index==16)
        {
            ui->NF_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年份：2022");
        }

        if (at_index==17)
        {
            ui->CCZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺寸组：L");
        }

        if (at_index==18)
        {
            ui->CGXZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购性质：内采");
        }

        if (at_index==19)
        {
            ui->BX_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("版型：宽松");
        }

        if (at_index==20)
        {
            ui->CKG_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("长宽高：30");
        }

        if (at_index==21)
        {
            ui->LXSH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("联系商户：XXXX");
        }

        if (at_index==22)
        {
            ui->CM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺码：38");
        }

        if (at_index==23)
        {
            ui->JYFS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("营销方式：特价促销");
        }
        ui->label_ck1->setText(str);
        ckmap["label_ck1"]=at_index;
    }
    if(label_index==2){
        QString str;
        if (at_index==1)
        {
            ui->PM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品名：休闲短裤");
        }

        if (at_index==2)
        {
            ui->ZYPL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("主营品类:鞋");
        }

        if (at_index==3)
        {
            ui->HH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("货号：22-213");
        }

        if (at_index==4)
        {
            ui->PP_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品牌：向阳木");
        }

        if (at_index==5)
        {
            ui->SLMC_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("四类名称:xxxx");
        }

        if (at_index==6)
        {
            ui->DL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("大类:上衣");
        }

        if (at_index==7)
        {
            ui->ZL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("中类：牛仔裤");
        }

        if (at_index==8)
        {
            ui->XL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("小类:直筒牛仔裤");
        }

        if (at_index==9)
        {
            ui->XB_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("性别：女");
        }

        if (at_index==10)
        {
            ui->YS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("颜色：黑色");
        }

        if (at_index==11)
        {
            ui->GYS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("供应商：浙商产业园");
        }

        if (at_index==12)
        {
            ui->ZTJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("正特价：特价");
        }

        if (at_index==13)
        {
            ui->JJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("季节：四季");
        }

        if (at_index==14)
        {
            ui->CGY_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购员：小王");
        }

        if (at_index==15)
        {
            ui->NLD_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年龄段：12~16");
        }

        if (at_index==16)
        {
            ui->NF_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年份：2022");
        }

        if (at_index==17)
        {
            ui->CCZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺寸组：L");
        }

        if (at_index==18)
        {
            ui->CGXZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购性质：内采");
        }

        if (at_index==19)
        {
            ui->BX_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("版型：宽松");
        }

        if (at_index==20)
        {
            ui->CKG_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("长宽高：30");
        }

        if (at_index==21)
        {
            ui->LXSH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("联系商户：XXXX");
        }

        if (at_index==22)
        {
            ui->CM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺码：38");
        }

        if (at_index==23)
        {
            ui->JYFS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("营销方式：特价促销");
        }
        ui->label_ck2->setText(str);
        ckmap["label_ck2"]=at_index;

    }
    if(label_index==3){
        QString str;
        if (at_index==1)
        {
            ui->PM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品名：休闲短裤");
        }

        if (at_index==2)
        {
            ui->ZYPL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("主营品类:鞋");
        }

        if (at_index==3)
        {
            ui->HH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("货号：22-213");
        }

        if (at_index==4)
        {
            ui->PP_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品牌：向阳木");
        }

        if (at_index==5)
        {
            ui->SLMC_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("四类名称:xxxx");
        }

        if (at_index==6)
        {
            ui->DL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("大类:上衣");
        }

        if (at_index==7)
        {
            ui->ZL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("中类：牛仔裤");
        }

        if (at_index==8)
        {
            ui->XL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("小类:直筒牛仔裤");
        }

        if (at_index==9)
        {
            ui->XB_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("性别：女");
        }

        if (at_index==10)
        {
            ui->YS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("颜色：黑色");
        }

        if (at_index==11)
        {
            ui->GYS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("供应商：浙商产业园");
        }

        if (at_index==12)
        {
            ui->ZTJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("正特价：特价");
        }

        if (at_index==13)
        {
            ui->JJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("季节：四季");
        }

        if (at_index==14)
        {
            ui->CGY_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购员：小王");
        }

        if (at_index==15)
        {
            ui->NLD_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年龄段：12~16");
        }

        if (at_index==16)
        {
            ui->NF_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年份：2022");
        }

        if (at_index==17)
        {
            ui->CCZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺寸组：L");
        }

        if (at_index==18)
        {
            ui->CGXZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购性质：内采");
        }

        if (at_index==19)
        {
            ui->BX_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("版型：宽松");
        }

        if (at_index==20)
        {
            ui->CKG_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("长宽高：30");
        }

        if (at_index==21)
        {
            ui->LXSH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("联系商户：XXXX");
        }

        if (at_index==22)
        {
            ui->CM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺码：38");
        }

        if (at_index==23)
        {
            ui->JYFS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("营销方式：特价促销");
        }
        ui->label_ck3->setText(str);
        ckmap["label_ck3"]=at_index;

    }
    if(label_index==4){
        QString str;
        if (at_index==1)
        {
            ui->PM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品名：休闲短裤");
        }

        if (at_index==2)
        {
            ui->ZYPL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("主营品类:鞋");
        }

        if (at_index==3)
        {
            ui->HH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("货号：22-213");
        }

        if (at_index==4)
        {
            ui->PP_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品牌：向阳木");
        }

        if (at_index==5)
        {
            ui->SLMC_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("四类名称:xxxx");
        }

        if (at_index==6)
        {
            ui->DL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("大类:上衣");
        }

        if (at_index==7)
        {
            ui->ZL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("中类：牛仔裤");
        }

        if (at_index==8)
        {
            ui->XL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("小类:直筒牛仔裤");
        }

        if (at_index==9)
        {
            ui->XB_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("性别：女");
        }

        if (at_index==10)
        {
            ui->YS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("颜色：黑色");
        }

        if (at_index==11)
        {
            ui->GYS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("供应商：浙商产业园");
        }

        if (at_index==12)
        {
            ui->ZTJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("正特价：特价");
        }

        if (at_index==13)
        {
            ui->JJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("季节：四季");
        }

        if (at_index==14)
        {
            ui->CGY_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购员：小王");
        }

        if (at_index==15)
        {
            ui->NLD_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年龄段：12~16");
        }

        if (at_index==16)
        {
            ui->NF_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年份：2022");
        }

        if (at_index==17)
        {
            ui->CCZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺寸组：L");
        }

        if (at_index==18)
        {
            ui->CGXZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购性质：内采");
        }

        if (at_index==19)
        {
            ui->BX_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("版型：宽松");
        }

        if (at_index==20)
        {
            ui->CKG_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("长宽高：30");
        }

        if (at_index==21)
        {
            ui->LXSH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("联系商户：XXXX");
        }

        if (at_index==22)
        {
            ui->CM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺码：38");
        }

        if (at_index==23)
        {
            ui->JYFS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("营销方式：特价促销");
        }
        ui->label_ck4->setText(str);
        ckmap["label_ck4"]=at_index;

    }
    if(label_index==5){
        QString str;
        if (at_index==1)
        {
            ui->PM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品名：休闲短裤");
        }

        if (at_index==2)
        {
            ui->ZYPL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("主营品类:鞋");
        }

        if (at_index==3)
        {
            ui->HH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("货号：22-213");
        }

        if (at_index==4)
        {
            ui->PP_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("品牌：向阳木");
        }

        if (at_index==5)
        {
            ui->SLMC_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("四类名称:xxxx");
        }

        if (at_index==6)
        {
            ui->DL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("大类:上衣");
        }

        if (at_index==7)
        {
            ui->ZL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("中类：牛仔裤");
        }

        if (at_index==8)
        {
            ui->XL_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("小类:直筒牛仔裤");
        }

        if (at_index==9)
        {
            ui->XB_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("性别：女");
        }

        if (at_index==10)
        {
            ui->YS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("颜色：黑色");
        }

        if (at_index==11)
        {
            ui->GYS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("供应商：浙商产业园");
        }

        if (at_index==12)
        {
            ui->ZTJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("正特价：特价");
        }

        if (at_index==13)
        {
            ui->JJ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("季节：四季");
        }

        if (at_index==14)
        {
            ui->CGY_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购员：小王");
        }

        if (at_index==15)
        {
            ui->NLD_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年龄段：12~16");
        }

        if (at_index==16)
        {
            ui->NF_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("年份：2022");
        }

        if (at_index==17)
        {
            ui->CCZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺寸组：L");
        }

        if (at_index==18)
        {
            ui->CGXZ_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("采购性质：内采");
        }

        if (at_index==19)
        {
            ui->BX_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("版型：宽松");
        }

        if (at_index==20)
        {
            ui->CKG_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("长宽高：30");
        }

        if (at_index==21)
        {
            ui->LXSH_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("联系商户：XXXX");
        }

        if (at_index==22)
        {
            ui->CM_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("尺码：38");
        }

        if (at_index==23)
        {
            ui->JYFS_checkBox->setCheckState(Qt::Checked);
            str=QString::fromLocal8Bit("营销方式：特价促销");
        }
        ui->label_ck5->setText(str);
        ckmap["label_ck5"]=at_index;
    }
}

void dpage::on_QX_pushButton_clicked()
{
    this->close();
}

void dpage::on_BC_pushButton_clicked()
{


    QFileDialog fileDialog;
    QString fileName = fileDialog.getSaveFileName(this,tr("Save File"),"/label",tr("*.ini"));
    if(fileName == "")
    {
        return;
    }
    else
    {
        QSettings *config_page= new QSettings(fileName,QSettings::IniFormat);

        config_page->beginGroup("page");
        config_page->setValue("page_w",pagewidth);
        config_page->setValue("page_h",pageheight);
        config_page->endGroup();

        config_page->beginGroup("label_ck1");
        config_page->setValue("start_point",ui->label_ck1->pos());
        config_page->setValue("label_w",ui->label_ck1->width());
        config_page->setValue("label_h",ui->label_ck1->height());
        config_page->setValue("attribute",ckmap["label_ck1"]);
        config_page->setValue("font",fontmap["label_ck1"]);
        config_page->setValue("font_size",sizemap["label_ck1"]);
        config_page->setValue("align",alignmap["label_ck1"]);
        config_page->endGroup();

        config_page->beginGroup("label_ck2");
        config_page->setValue("start_point",ui->label_ck2->pos());
        config_page->setValue("label_w",ui->label_ck2->width());
        config_page->setValue("label_h",ui->label_ck2->height());
        config_page->setValue("attribute",ckmap["label_ck2"]);
        config_page->setValue("font",fontmap["label_ck2"]);
        config_page->setValue("font_size",sizemap["label_ck2"]);
        config_page->setValue("align",alignmap["label_ck2"]);
        config_page->endGroup();

        config_page->beginGroup("label_ck3");
        config_page->setValue("start_point",ui->label_ck3->pos());
        config_page->setValue("label_w",ui->label_ck3->width());
        config_page->setValue("label_h",ui->label_ck3->height());
        config_page->setValue("attribute",ckmap["label_ck3"]);
        config_page->setValue("font",fontmap["label_ck3"]);
        config_page->setValue("font_size",sizemap["label_ck3"]);
        config_page->setValue("align",alignmap["label_ck3"]);
        config_page->endGroup();

        config_page->beginGroup("label_ck4");
        config_page->setValue("start_point",ui->label_ck4->pos());
        config_page->setValue("label_w",ui->label_ck4->width());
        config_page->setValue("label_h",ui->label_ck4->height());
        config_page->setValue("attribute",ckmap["label_ck4"]);
        config_page->setValue("font",fontmap["label_ck4"]);
        config_page->setValue("font_size",sizemap["label_ck4"]);
        config_page->setValue("align",alignmap["label_ck4"]);
        config_page->endGroup();

        config_page->beginGroup("label_ck5");
        config_page->setValue("start_point",ui->label_ck5->pos());
        config_page->setValue("label_w",ui->label_ck5->width());
        config_page->setValue("label_h",ui->label_ck5->height());
        config_page->setValue("attribute",ckmap["label_ck5"]);
        config_page->setValue("font",fontmap["label_ck5"]);
        config_page->setValue("font_size",sizemap["label_ck5"]);
        config_page->setValue("align",alignmap["label_ck5"]);
        config_page->endGroup();

        config_page->beginGroup("label_bar");
        config_page->setValue("ischeck",ui->BAR_checkBox->checkState());
        config_page->setValue("start_point",ui->label_bar->pos());
        config_page->setValue("label_w",ui->label_bar->width());
        config_page->setValue("label_h",ui->label_bar->height());
        config_page->setValue("align",alignmap["label_bar"]);
        config_page->endGroup();

        config_page->beginGroup("label_data");
        config_page->setValue("ischeck",ui->DATA_checkBox->checkState());
        config_page->setValue("start_point",ui->label_data->pos());
        config_page->setValue("label_w",ui->label_data->width());
        config_page->setValue("label_h",ui->label_data->height());
        config_page->setValue("align",alignmap["label_data"]);
        config_page->endGroup();

        QMessageBox::warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("保存文件成功"));
        filepath=fileName;
        emit signal_path();
        delete  config_page;
    }

}

void dpage::load(bool MR)
{
    //屏蔽设计模式下checkbox响应
    design=false;
    //取消所有勾选
    ui->PM_checkBox->setCheckState(Qt::Unchecked);
    ui->ZYPL_checkBox->setCheckState(Qt::Unchecked);
    ui->HH_checkBox->setCheckState(Qt::Unchecked);
    ui->PP_checkBox->setCheckState(Qt::Unchecked);
    ui->SLMC_checkBox->setCheckState(Qt::Unchecked);
    ui->DL_checkBox->setCheckState(Qt::Unchecked);
    ui->ZL_checkBox->setCheckState(Qt::Unchecked);
    ui->XL_checkBox->setCheckState(Qt::Unchecked);
    ui->XB_checkBox->setCheckState(Qt::Unchecked);
    ui->YS_checkBox->setCheckState(Qt::Unchecked);
    ui->GYS_checkBox->setCheckState(Qt::Unchecked);
    ui->ZTJ_checkBox->setCheckState(Qt::Unchecked);
    ui->JJ_checkBox->setCheckState(Qt::Unchecked);
    ui->CGY_checkBox->setCheckState(Qt::Unchecked);
    ui->NLD_checkBox->setCheckState(Qt::Unchecked);
    ui->NF_checkBox->setCheckState(Qt::Unchecked);
    ui->CCZ_checkBox->setCheckState(Qt::Unchecked);
    ui->CGXZ_checkBox->setCheckState(Qt::Unchecked);
    ui->BX_checkBox->setCheckState(Qt::Unchecked);
    ui->CKG_checkBox->setCheckState(Qt::Unchecked);
    ui->LXSH_checkBox->setCheckState(Qt::Unchecked);
    ui->CM_checkBox->setCheckState(Qt::Unchecked);
    ui->JYFS_checkBox->setCheckState(Qt::Unchecked);
    ui->BAR_checkBox->setCheckState(Qt::Unchecked);
    ui->DATA_checkBox->setCheckState(Qt::Unchecked);
    if(MR)
    {
        MR=false;
        QString file_full = "./default.ini";
        pageset(file_full);
        filepath=file_full;
    }
    else
    {
        qDebug()<<filepath;
        pageset(filepath);
    }
    design=true;
}

void dpage::label_align(int index,QString label){
    auto ind = findChild<CustomLabel*>(label);
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


void dpage::pageset(QString path)
{
    QSettings setting(path,QSettings::IniFormat);
    //设置页面大小
    setting.beginGroup("page");
    int w=setting.value("page_w").toInt();
    int h=setting.value("page_h").toInt();
    QString w1=QString::number(w);
    QString h1=QString::number(h);;
    ui->page_widget->resize(w*5,h*5);
    ui->W_lineEdit->setText(w1);
    ui->H_lineEdit->setText(h1);
    on_W_lineEdit_editingFinished();
    on_H_lineEdit_editingFinished();
    setting.endGroup();
    //设置文本框1
    setting.beginGroup("label_ck1");
    //获取相关参数
    QPoint point_ck1=setting.value("start_point").toPoint();
    int w_ck1=setting.value("label_w").toInt();
    int h_ck1=setting.value("label_h").toInt();
    int at_ck1=setting.value("attribute").toInt();
    int font_ck1=setting.value("font").toInt();
    int size_ck1=setting.value("font_size").toInt();
    int align_index1=setting.value("align").toInt();
    if(at_ck1 != 0)
    {
        //设置文本框位置
        ui->label_ck1->setGeometry(point_ck1.x(),point_ck1.y(),w_ck1,h_ck1);
        //设置文本框布局属性
        label_align(align_index1,"label_ck1");
        //设置文本属性索引
        ckmap["label_ck1"]=at_ck1;
        fontmap["label_ck1"]=font_ck1;
        sizemap["label_ck1"]=setting.value("font_size").toString();
        alignmap["label_ck1"]=align_index1;
        //设置文本显示属性
        set_labeltext(at_ck1,1);
        //设置文本字体与字号
        QFont font;
        QString check_font;
        check_font=get_font(font_ck1);
        font.setFamily(check_font);
        font.setPixelSize(size_ck1);
        ui->label_ck1->setFont(font);
    }
    else
    {
        ui->label_ck1->clear();
    }
    setting.endGroup();

    setting.beginGroup("label_ck2");
    QPoint point_ck2=setting.value("start_point").toPoint();
    int w_ck2=setting.value("label_w").toInt();
    int h_ck2=setting.value("label_h").toInt();
    int at_ck2=setting.value("attribute").toInt();
    int font_ck2=setting.value("font").toInt();
    int size_ck2=setting.value("font_size").toInt();
    int align_index2=setting.value("align").toInt();
    if(at_ck2 != 0)
    {
        //设置文本框位置
        ui->label_ck2->setGeometry(point_ck2.x(),point_ck2.y(),w_ck2,h_ck2);
        label_align(align_index2,"label_ck2");
        //设置文本属性索引
        ckmap["label_ck2"]=at_ck2;
        fontmap["label_ck2"]=font_ck2;
        sizemap["label_ck2"]=setting.value("font_size").toString();
        alignmap["label_ck2"]=align_index2;
        //设置文本显示属性
        set_labeltext(at_ck2,2);
        //设置文本字体与字号
        QFont font;
        QString check_font;
        check_font=get_font(font_ck2);
        font.setFamily(check_font);
        font.setPixelSize(size_ck2);
        ui->label_ck2->setFont(font);
    }
    else
    {
        ui->label_ck2->clear();
    }
    setting.endGroup();

    setting.beginGroup("label_ck3");
    QPoint point_ck3=setting.value("start_point").toPoint();
    int w_ck3=setting.value("label_w").toInt();
    int h_ck3=setting.value("label_h").toInt();
    int at_ck3=setting.value("attribute").toInt();
    int font_ck3=setting.value("font").toInt();
    int size_ck3=setting.value("font_size").toInt();
    int align_index3=setting.value("align").toInt();
    if(at_ck3 != 0)
    {
        //设置文本框位置
        ui->label_ck3->setGeometry(point_ck3.x(),point_ck3.y(),w_ck3,h_ck3);
        label_align(align_index3,"label_ck3");
        //设置文本属性索引
        ckmap["label_ck3"]=at_ck3;
        fontmap["label_ck3"]=font_ck3;
        sizemap["label_ck3"]=setting.value("font_size").toString();
        alignmap["label_ck3"]=align_index3;
        //设置文本显示属性
        set_labeltext(at_ck3,3);
        //设置文本字体与字号
        QFont font;
        QString check_font;
        check_font=get_font(font_ck3);
        font.setFamily(check_font);
        font.setPixelSize(size_ck3);
        ui->label_ck3->setFont(font);
    }
    else
    {
        ui->label_ck3->clear();
    }
    setting.endGroup();

    setting.beginGroup("label_ck4");
    QPoint point_ck4=setting.value("start_point").toPoint();
    int w_ck4=setting.value("label_w").toInt();
    int h_ck4=setting.value("label_h").toInt();
    int at_ck4=setting.value("attribute").toInt();
    int font_ck4=setting.value("font").toInt();
    int size_ck4=setting.value("font_size").toInt();
    int align_index4=setting.value("align").toInt();
    if(at_ck4 != 0)
    {
        //设置文本框位置
        ui->label_ck4->setGeometry(point_ck4.x(),point_ck4.y(),w_ck4,h_ck4);
        label_align(align_index4,"label_ck4");
        //设置文本属性索引
        ckmap["label_ck4"]=at_ck4;
        fontmap["label_ck4"]=font_ck4;
        sizemap["label_ck4"]=setting.value("font_size").toString();
        alignmap["label_ck4"]=align_index4;
        //设置文本显示属性
        set_labeltext(at_ck4,4);
        //设置文本字体与字号
        QFont font;
        QString check_font;
        check_font=get_font(font_ck4);
        font.setFamily(check_font);
        font.setPixelSize(size_ck4);
        ui->label_ck4->setFont(font);
    }
    else
    {
        ui->label_ck4->clear();
    }
    setting.endGroup();

    setting.beginGroup("label_ck5");
    QPoint point_ck5=setting.value("start_point").toPoint();
    int w_ck5=setting.value("label_w").toInt();
    int h_ck5=setting.value("label_h").toInt();
    int at_ck5=setting.value("attribute").toInt();
    int font_ck5=setting.value("font").toInt();
    int size_ck5=setting.value("font_size").toInt();
    int align_index5=setting.value("align").toInt();
    if(at_ck5 != 0)
    {
        //设置文本框位置
        ui->label_ck5->setGeometry(point_ck5.x(),point_ck5.y(),w_ck5,h_ck5);
        label_align(align_index5,"label_ck5");
        //设置文本属性索引
        ckmap["label_ck5"]=at_ck5;
        fontmap["label_ck5"]=font_ck5;
        sizemap["label_ck5"]=setting.value("font_size").toString();
        alignmap["label_ck5"]=align_index5;
        //设置文本显示属性
        set_labeltext(at_ck5,5);
        //设置文本字体与字号
        QFont font;
        QString check_font;
        check_font=get_font(font_ck5);
        font.setFamily(check_font);
        font.setPixelSize(size_ck5);
        ui->label_ck5->setFont(font);
    }
    else
    {
        ui->label_ck5->clear();
    }
    setting.endGroup();
    //设置条形码
    setting.beginGroup("label_bar");
    int ischeck=setting.value("ischeck").toInt();
    QPoint point_bar=setting.value("start_point").toPoint();
    int w_bar=setting.value("label_w").toInt();
    int h_bar=setting.value("label_h").toInt();
    int align_index6=setting.value("align").toInt();
    if(ischeck==2)
    {
        ui->label_bar->setVisible(true);
        ui->label_bar->setGeometry(point_bar.x(),point_bar.y(),w_bar,h_bar);
        ui->BAR_checkBox->setCheckState(Qt::Checked);
        label_align(align_index6,"label_bar");
        alignmap["label_bar"]=align_index6;
        QFile file("./barcode.png");
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        QImage img = QImage::fromData(data);
        int w_la=ui->label_bar->width();
        int h_la=ui->label_bar->height();
        img = img.scaled(w_la,h_la,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_bar->setPixmap(QPixmap::fromImage(img));
        update();
    }
    else {
        ui->label_bar->setVisible(false);
    }
    setting.endGroup();
    //设置序列号
    setting.beginGroup("label_data");
    int ischeck_data=setting.value("ischeck").toInt();
    QPoint point_data=setting.value("start_point").toPoint();
    int w_data=setting.value("label_w").toInt();
    int h_data=setting.value("label_h").toInt();
    int align_index7=setting.value("align").toInt();
    if(ischeck_data==2)
    {
        ui->label_data->setVisible(true);
        ui->label_data->setGeometry(point_data.x(),point_data.y(),w_data,h_data);
        ui->DATA_checkBox->setCheckState(Qt::Checked);
        label_align(align_index7,"label_data");
        alignmap["label_data"]=align_index7;
        QFile file("./data.png");
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        QImage img = QImage::fromData(data);
        int w_la=ui->label_data->width();
        int h_la=ui->label_data->height();
        img = img.scaled(w_la,h_la,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_data->setPixmap(QPixmap::fromImage(img));
        update();

    }
    else {
        ui->label_data->setVisible(false);
    }
    setting.endGroup();
    //设置完毕后恢复设计模式
}

void dpage::on_ZR_pushButton_clicked()
{
    QString file_full = QFileDialog::getOpenFileName(this,tr("Save File"),tr("*.ini"));
    if(file_full==""){
        return;
    }
    filepath=file_full;
    //屏蔽设计模式下checkbox响应
    design=false;
    //取消所有勾选
    ui->PM_checkBox->setCheckState(Qt::Unchecked);
    ui->ZYPL_checkBox->setCheckState(Qt::Unchecked);
    ui->HH_checkBox->setCheckState(Qt::Unchecked);
    ui->PP_checkBox->setCheckState(Qt::Unchecked);
    ui->SLMC_checkBox->setCheckState(Qt::Unchecked);
    ui->DL_checkBox->setCheckState(Qt::Unchecked);
    ui->ZL_checkBox->setCheckState(Qt::Unchecked);
    ui->XL_checkBox->setCheckState(Qt::Unchecked);
    ui->XB_checkBox->setCheckState(Qt::Unchecked);
    ui->YS_checkBox->setCheckState(Qt::Unchecked);
    ui->GYS_checkBox->setCheckState(Qt::Unchecked);
    ui->ZTJ_checkBox->setCheckState(Qt::Unchecked);
    ui->JJ_checkBox->setCheckState(Qt::Unchecked);
    ui->CGY_checkBox->setCheckState(Qt::Unchecked);
    ui->NLD_checkBox->setCheckState(Qt::Unchecked);
    ui->NF_checkBox->setCheckState(Qt::Unchecked);
    ui->CCZ_checkBox->setCheckState(Qt::Unchecked);
    ui->CGXZ_checkBox->setCheckState(Qt::Unchecked);
    ui->BX_checkBox->setCheckState(Qt::Unchecked);
    ui->CKG_checkBox->setCheckState(Qt::Unchecked);
    ui->LXSH_checkBox->setCheckState(Qt::Unchecked);
    ui->CM_checkBox->setCheckState(Qt::Unchecked);
    ui->JYFS_checkBox->setCheckState(Qt::Unchecked);
    ui->BAR_checkBox->setCheckState(Qt::Unchecked);
    ui->DATA_checkBox->setCheckState(Qt::Unchecked);
    pageset(file_full);
    design=true;
}

void dpage::on_MR_pushButton_clicked()
{
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确定恢复默认模板吗？"), QMessageBox::Yes|QMessageBox::No);
    if(box==QMessageBox::No)
       return;
    else {
        MR=true;
        load(MR);
    }
}

void dpage::on_QR_pushButton_clicked()
{
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确定使用当前模板吗？"), QMessageBox::Yes|QMessageBox::No);
    if(box==QMessageBox::No)
       return;
    else {
        QString cpath="./temporary.ini";
        QSettings *config_page= new QSettings(cpath,QSettings::IniFormat);

        config_page->beginGroup("page");
        config_page->setValue("page_w",pagewidth);
        config_page->setValue("page_h",pageheight);
        config_page->endGroup();

        config_page->beginGroup("label_ck1");
        config_page->setValue("start_point",ui->label_ck1->pos());
        config_page->setValue("label_w",ui->label_ck1->width());
        config_page->setValue("label_h",ui->label_ck1->height());
        config_page->setValue("attribute",ckmap["label_ck1"]);
        config_page->setValue("font",fontmap["label_ck1"]);
        config_page->setValue("font_size",sizemap["label_ck1"]);
        config_page->setValue("align",alignmap["label_ck1"]);
        config_page->endGroup();

        config_page->beginGroup("label_ck2");
        config_page->setValue("start_point",ui->label_ck2->pos());
        config_page->setValue("label_w",ui->label_ck2->width());
        config_page->setValue("label_h",ui->label_ck2->height());
        config_page->setValue("attribute",ckmap["label_ck2"]);
        config_page->setValue("font",fontmap["label_ck2"]);
        config_page->setValue("font_size",sizemap["label_ck2"]);
        config_page->setValue("align",alignmap["label_ck2"]);
        config_page->endGroup();

        config_page->beginGroup("label_ck3");
        config_page->setValue("start_point",ui->label_ck3->pos());
        config_page->setValue("label_w",ui->label_ck3->width());
        config_page->setValue("label_h",ui->label_ck3->height());
        config_page->setValue("attribute",ckmap["label_ck3"]);
        config_page->setValue("font",fontmap["label_ck3"]);
        config_page->setValue("font_size",sizemap["label_ck3"]);
        config_page->setValue("align",alignmap["label_ck3"]);
        config_page->endGroup();

        config_page->beginGroup("label_ck4");
        config_page->setValue("start_point",ui->label_ck4->pos());
        config_page->setValue("label_w",ui->label_ck4->width());
        config_page->setValue("label_h",ui->label_ck4->height());
        config_page->setValue("attribute",ckmap["label_ck4"]);
        config_page->setValue("font",fontmap["label_ck4"]);
        config_page->setValue("font_size",sizemap["label_ck4"]);
        config_page->setValue("align",alignmap["label_ck4"]);
        config_page->endGroup();

        config_page->beginGroup("label_ck5");
        config_page->setValue("start_point",ui->label_ck5->pos());
        config_page->setValue("label_w",ui->label_ck5->width());
        config_page->setValue("label_h",ui->label_ck5->height());
        config_page->setValue("attribute",ckmap["label_ck5"]);
        config_page->setValue("font",fontmap["label_ck5"]);
        config_page->setValue("font_size",sizemap["label_ck5"]);
        config_page->setValue("align",alignmap["label_ck5"]);
        config_page->endGroup();

        config_page->beginGroup("label_bar");
        config_page->setValue("ischeck",ui->BAR_checkBox->checkState());
        config_page->setValue("start_point",ui->label_bar->pos());
        config_page->setValue("label_w",ui->label_bar->width());
        config_page->setValue("label_h",ui->label_bar->height());
        config_page->setValue("align",alignmap["label_bar"]);
        config_page->endGroup();

        config_page->beginGroup("label_data");
        config_page->setValue("ischeck",ui->DATA_checkBox->checkState());
        config_page->setValue("start_point",ui->label_data->pos());
        config_page->setValue("label_w",ui->label_data->width());
        config_page->setValue("label_h",ui->label_data->height());
        config_page->setValue("align",alignmap["label_data"]);
        config_page->endGroup();

        filepath=cpath;
        emit reload();
    }
}

