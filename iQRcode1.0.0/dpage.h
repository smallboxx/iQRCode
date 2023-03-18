/***********************************
*  File: dpage.h
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/
#ifndef DPAGE_H
#define DPAGE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
namespace Ui {
class dpage;
}

class dpage : public QWidget
{
    Q_OBJECT

public:
    explicit dpage(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
    ~dpage();


signals:
    void signal_path();
    void reload();

private slots:

    void add_click();

    void del_click();//右键菜单点击删除操作响应函数

    void left_click();

    void center_click();

    void right_click();

    void add_items(bool status,QString str,int ck,QString label_str);

    QString get_label(int ck1);

    void uncheckbox(QString label_str);

    QString get_font(int index);

    void set_labeltext(int at_index,int label_index);

    void label_align(int index,QString label);

    void pageset(QString path);

    void load(bool MR);

    void on_BC_pushButton_clicked();

    void on_W_lineEdit_editingFinished();

    void on_H_lineEdit_editingFinished();

    void on_PM_checkBox_stateChanged();

    void on_ZYPL_checkBox_stateChanged();

    void on_HH_checkBox_stateChanged();

    void on_PP_checkBox_stateChanged();

    void on_SLMC_checkBox_stateChanged();

    void on_DL_checkBox_stateChanged();

    void on_ZL_checkBox_stateChanged();

    void on_XL_checkBox_stateChanged();

    void on_XB_checkBox_stateChanged();

    void on_YS_checkBox_stateChanged();

    void on_GYS_checkBox_stateChanged();

    void on_ZTJ_checkBox_stateChanged();

    void on_JJ_checkBox_stateChanged();

    void on_CGY_checkBox_stateChanged();

    void on_NLD_checkBox_stateChanged();

    void on_NF_checkBox_stateChanged();

    void on_CCZ_checkBox_stateChanged();

    void on_CGXZ_checkBox_stateChanged();

    void on_BX_checkBox_stateChanged();

    void on_CKG_checkBox_stateChanged();

    void on_LXSH_checkBox_stateChanged();

    void on_CM_checkBox_stateChanged();

    void on_JYFS_checkBox_stateChanged();

    void on_BAR_checkBox_stateChanged(int arg1);

    void on_DATA_checkBox_stateChanged(int arg1);

    void on_font_comboBox_currentIndexChanged(int index);

    void on_size_comboBox_currentTextChanged(const QString &arg1);

    void on_QX_pushButton_clicked();

    void on_ZR_pushButton_clicked();

    void on_MR_pushButton_clicked();

    void on_QR_pushButton_clicked();

    void on_Align_comboBox_currentIndexChanged(int index);



private:
    std::map<QString,int> ckmap;
    std::map<QString,int> fontmap;
    std::map<QString,int> alignmap;
    std::map<QString,QString> sizemap;
    int ck1=0;
    Ui::dpage *ui;
    QTimer *mtimer;
    bool design;
    bool MR;
    QMenu* label_menu;//对文本框进行添加删除操作
    bool boundingflag;//防止弹窗两次
    int d_member;//文件框间距系数
    void* maindpagePtr;
};

#endif // DPAGE_H
