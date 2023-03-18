/***********************************
*  File: customlabel.h
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/
#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QTimer>
#include <QMenu>
class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CustomLabel(QWidget *parent = nullptr);
    void move_label(QPoint movep,int w,int h);

signals:
    void clicked();



protected:
    void mousePressEvent(QMouseEvent* ev) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPoint relativep;
    QPoint startpos;
    QPoint startglobalpos;
    QPoint mousetrack;
    QPoint tl;
    QPoint br;
    QPoint page_size;
    bool ispressed;
    bool ismove;
    QTimer *timer;
    int w;
    int h;
    int w_move;
    int h_move;
    bool movecss;
    bool scalecss;


};
#endif // CUSTOMLABEL_H
