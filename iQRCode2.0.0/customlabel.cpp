#include "customlabel.h"
#include <QDebug>
#include <QWidgetAction>
#include "ui_mainwindow.h"

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent)
{
    QRect label_rect(QPoint(0,0),QSize(150,50));
    this->setGeometry(label_rect);
    this->setStyleSheet("border:2px dashed;background:transparent");
    QFont font;
    font.setPointSize(9);
    this->setFont(font);
    label_menu = new QMenu;
    label_menu->setWindowFlags(label_menu->windowFlags() | Qt::FramelessWindowHint);
    QWidgetAction* pWdtAction1 = new QWidgetAction(label_menu);
    pWdtAction1->setDefaultWidget(new QLabel(QString::fromUtf8("文本框设置:"), label_menu));
    m_del = new QAction(QString::fromUtf8("删除"));
    m_left = new QAction(QString::fromUtf8("左对齐"));
    m_center = new QAction(QString::fromUtf8("居中"));
    m_right = new QAction(QString::fromUtf8("右对齐"));
    //右键菜单添加属性
    label_menu->addAction(pWdtAction1);
    label_menu->addAction(m_del);
    label_menu->addAction(m_left);
    label_menu->addAction(m_center);
    label_menu->addAction(m_right);
    connect(m_del,&QAction::triggered,this,&CustomLabel::del_click);
    connect(m_left,&QAction::triggered,this,&CustomLabel::left_click);
    connect(m_center,&QAction::triggered,this,&CustomLabel::center_click);
    connect(m_right,&QAction::triggered,this,&CustomLabel::right_click);
    emit get_tagsize();
}

void CustomLabel::del_click(){
    QString name=this->objectName();
    emit send_objname(name);
    delete this;
}

void CustomLabel::left_click(){
    this->setGeometry(0,this->y(),this->width(),this->height());
}

void CustomLabel::right_click(){
    this->setGeometry(tagsize.x()-this->width(),this->y(),this->width(),this->height());
}

void CustomLabel::center_click(){
    this->setGeometry((tagsize.x()-this->width())/2,this->y(),this->width(),this->height());
}

void CustomLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        ismove=true;
        isscale=true;
        setMouseTracking(true);
        front_posx=cursor().pos().x();
        front_posy=cursor().pos().y();
        this->setStyleSheet("border:2px dashed;background-color:#CCCCCC");
        QString name=this->objectName();
        emit send_font_objname(name);
    }
    else if (ev->button()==Qt::RightButton) {
        emit get_tagsize();
        label_menu->exec(QCursor::pos());
    }
    QLabel::mousePressEvent(ev);
}

void CustomLabel::set_label_status(int x, int y){
    QRect label_rect(QPoint(this->x(),this->y()),QSize(this->width(),this->height()));
    QPoint point(x,y);
    if(label_rect.contains(point)){
        return;
    }
    else {
        this->setStyleSheet("border:2px dashed;background:transparent");
    }
}

void CustomLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        ismove=false;
        isscale=false;
        this->compute_r_geometry();
    }
}

void CustomLabel::tagsize_message(int w, int h){
    tagsize.setX(w);
    tagsize.setY(h);
}
void CustomLabel::mouseMoveEvent(QMouseEvent *event)
{
    int x_dif=abs(event->pos().x()-this->width());
    int y_dif=abs(event->pos().y()-this->height());
    int x_of=cursor().pos().x()-front_posx;
    int y_of=cursor().pos().y()-front_posy;
    if(x_dif<10 && y_dif<10){
        setCursor(QCursor(Qt::SizeFDiagCursor));
        if(isscale)
            this->setGeometry(this->x(),this->y(),event->pos().x(),event->pos().y());
    }
    else if(ismove && cursor()==Qt::PointingHandCursor){
        emit get_tagsize();
        int new_x=this->pos().x()+x_of;
        int new_y=this->pos().y()+y_of;
        if(new_x<0 || new_y<0 || new_x+this->width()>tagsize.x() || new_y+this->height()>tagsize.y())
            return;
        this->setGeometry(new_x,new_y,this->width(),this->height());
    }
    front_posx=cursor().pos().x();
    front_posy=cursor().pos().y();
    QLabel::mouseMoveEvent(event);
}

void CustomLabel::enterEvent(QEvent *event)
{
    setCursor(QCursor(Qt::PointingHandCursor));
    QLabel::enterEvent(event);
}

void CustomLabel::leaveEvent(QEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor));
    QLabel::leaveEvent(event);
}

void CustomLabel::compute_r_geometry(){
    emit get_tagsize();
    double x=this->x();
    double y=this->y();
    double w=this->width();
    double h=this->height();
    double tagw=tagsize.x();
    double tagh=tagsize.y();
    this->rx=x/tagw;
    this->ry=y/tagh;
    this->rw=w/tagw;
    this->rh=h/tagh;
}

RLabel::RLabel(QWidget *parent) : QLabel(parent)
{

}
void RLabel::compute_r_geometry(double tagw,double tagh){
    double x=this->x();
    double y=this->y();
    double w=this->width();
    double h=this->height();
    this->rx=x/tagw;
    this->ry=y/tagh;
    this->rw=w/tagw;
    this->rh=h/tagh;
}

