#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QTimer>
#include <QMenu>
#include <QTextBrowser>
class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CustomLabel(QWidget *parent = nullptr);
    void move_label(QPoint movep,int w,int h);
    void tagsize_message(int w,int h);
    void set_label_status(int x,int y);
    void del_click();
    void right_click();
    void center_click();
    void left_click();
    void compute_r_geometry();
    struct{
        double rx;
        double ry;
        double rw;
        double rh;
    };//相对标签Geometry

signals:
    void get_tagsize();
    void send_objname(QString);
    void send_font_objname(QString);

protected:
    virtual void mousePressEvent(QMouseEvent* ev) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
private:
    int front_posx;
    int front_posy;
    QPoint tagsize;//标签大小
    QMenu *label_menu;
    QAction* m_del;
    QAction* m_left;
    QAction* m_center;
    QAction* m_right;
    bool ismove;
    bool isscale;

};

class RLabel : public QLabel
{
    Q_OBJECT
public:
    explicit RLabel(QWidget *parent = nullptr);
    void compute_r_geometry(double tagw,double tagh);
    struct{
        double rx;
        double ry;
        double rw;
        double rh;
    };//相对标签Geometry
};

#endif // CUSTOMLABEL_H
