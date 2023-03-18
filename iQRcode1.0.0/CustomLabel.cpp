/***********************************
*  File: CustomLabel.cpp
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/
#include "customlabel.h"
#include <QDebug>
extern QString pagewidth;
extern QString pageheight;

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent)
{
    QAction* m_del = new QAction(QString::fromLocal8Bit("删除"));

}

void CustomLabel::mousePressEvent(QMouseEvent *ev)
{
    w_move=pagewidth.toInt()*5;
    h_move=pageheight.toInt()*5;
    setMouseTracking(true);
    //如果单击了就触发clicked信号
    movecss=false;
    scalecss=false;
    if (ev->button() == Qt::LeftButton)
    {
        //触发clicked信号
        emit clicked();
        ispressed=true;
        startglobalpos=ev->globalPos();
        startpos=this->geometry().topLeft();
        //qDebug()<<startpos
        relativep=startglobalpos - startpos;
    }
    //将该事件传给父类处理
    QLabel::mousePressEvent(ev);
}

void CustomLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        ispressed=false;
        relativep.setX(0);
        relativep.setY(0);
    }
}

void CustomLabel::mouseMoveEvent(QMouseEvent *event)
{

    tl=this->geometry().topLeft();
    br=this->geometry().bottomRight();
    int w_label=this->width();
    int h_label=this->height();
    QPoint curPos=this->pos();
    mousetrack=event->pos();
    w=abs(this->width()-mousetrack.x());
    h=abs(this->height()-mousetrack.y());

    if(w<5 && h<5){
        setCursor(QCursor(Qt::SizeFDiagCursor));
        scalecss=true;
        movecss=false;
    }
    else if ((h<5&&w>5)||(h>25&&w>5)) {
        setCursor(QCursor(Qt::SizeAllCursor));
        movecss=true;
        scalecss=false;
    }
    else{
        setCursor(QCursor(Qt::PointingHandCursor));
        scalecss=false;
        movecss=false;
    }
    //qDebug()<<tl<<br;
    if(ispressed && movecss && (tl.x()>-1) && tl.y()>-1 && br.x()<w_move+1 && br.y()<h_move+1)
    {
        QPoint mousep = event->globalPos();
        //计算新的起始点
        QPoint movep = mousep - startglobalpos;
        this->move(movep+startpos);
    }
    if(ispressed && scalecss)
    {
    //qDebug()<<mousetrack;
        this->setGeometry(this->x(),this->y(),mousetrack.x(),mousetrack.y());
    }

    if(tl.x()<0)
    {
        curPos.setX(0);
        move(curPos);
    }
    if(tl.y()<0)
    {
        curPos.setY(0);
        move(curPos);
    }
    if(br.x()>w_move)
    {
        curPos.setX(w_move-w_label);
        move(curPos);
    }
    if(br.y()>h_move)
    {
        curPos.setY(h_move-h_label);
        move(curPos);
    }
}



void CustomLabel::enterEvent(QEvent *event)
{
    setCursor(QCursor(Qt::PointingHandCursor));
}

void CustomLabel::leaveEvent(QEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor));
}
