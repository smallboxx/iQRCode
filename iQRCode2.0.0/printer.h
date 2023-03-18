#ifndef PRINTER_H
#define PRINTER_H

#include <QWidget>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QTimer>

namespace Ui {
class printer;
}

class printer : public QWidget
{
    Q_OBJECT

public:
    explicit printer(QWidget *parent = nullptr);
    ~printer();
    void initPrinter();
    void closeEvent(QCloseEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;
    void resize_tag(double linew,double lineh,double sysw,double sysh);
    bool isnumber(QString str);
    void send_cuatomlabel_message();
    void reset_button(QString name);
    void get_label_name(QString name);
    QFont get_font(QString ztstr,QString zhstr);
    void set_tagpage(QString path);
    void load_templabel();
    void new_customlabel(QString str,QString name);
    void save_label(QString path);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;

signals:
    void close();
    void send_tagesize_message(int,int);
    void send_cursor_xy(int,int);
    void send_mainwindow();

private slots:
    void on_lineEdit_w_editingFinished();

    void on_lineEdit_h_editingFinished();

    void on_pushButton_wbpm_clicked();

    void on_pushButton_wbkh_clicked();

    void on_pushButton_ys_clicked();

    void on_pushButton_cm_clicked();

    void on_pushButton_xb_clicked();

    void on_pushButton_zypl_clicked();

    void on_pushButton_gys_clicked();

    void on_pushButton_lxhh_clicked();

    void on_pushButton_id_clicked();

    void on_pushButton_barcode_clicked();

    void on_comboBox_zt_currentIndexChanged(const QString &arg1);

    void on_comboBox_zh_currentIndexChanged(const QString &arg1);

    void on_pushButton_bc_clicked();

    void on_pushButton_zr_clicked();

    void on_pushButton_mr_clicked();

    void on_pushButton_dqmb_clicked();

    void on_pushButton_qx_clicked();

private:
    Ui::printer *ui;
    QResizeEvent *ev;
    QWidget *tagpage;
    QString objname;
    QTimer *temptimer;
};









//CPCL
typedef int(__stdcall *PrinterCreator)(void**, const char*);//
typedef int(__stdcall *PrinterCreatorS)(char*);
typedef int(__stdcall *PortOpen)(void*, const char*);//
typedef int(__stdcall *PortClose)(void*);//
typedef int(__stdcall *PrinterDestroy)(void*);//
typedef int(__stdcall *DirectIO)(void* handle, unsigned char writedata[], unsigned int writenum, unsigned char readdata[], unsigned int readnum, unsigned int* preadednum);
typedef int(__stdcall *CPCL_AddLabel)(void* , int , int , int );
typedef int(__stdcall *CPCL_SetAlign)(void* , int );
typedef int(__stdcall *CPCL_AddText)(void* , int , int , int , int , int , char*);
typedef int(__stdcall *CPCL_AddBarCode)(void* , int , int , int , int , int , int , int , const char* );
typedef int(__stdcall *CPCL_AddBarCodeText)(void* , int , int , int , int );
typedef int(__stdcall *CPCL_AddQRCode)(void* , int , int , int , int , int , int , const char* );
typedef int(__stdcall *CPCL_AddPDF417)(void* , int , int , int , int , int , int , int , int , const char* );
typedef int(__stdcall *CPCL_AddBox)(void* , int , int , int , int , int );
typedef int(__stdcall *CPCL_AddLine)(void* , int , int , int , int , int );
typedef int(__stdcall *CPCL_SetFontSize)(void* , int , int );
typedef int(__stdcall *CPCL_SetDensity)(void* , int );
typedef int(__stdcall *CPCL_SetSpeed)(void* , int );
typedef int(__stdcall *CPCL_SetTextSpacing)(void* , int );
typedef int(__stdcall *CPCL_SetTextBold)(void* , int );
typedef int(__stdcall *CPCL_SetTextUnderline)(void* , int );
typedef int(__stdcall *CPCL_Print)(void*);
typedef int(__stdcall *CPCL_NextLabelPos)(void* );
typedef int(__stdcall *CPCL_PreFeed)(void* , int );
typedef int(__stdcall *CPCL_PostFeed)(void* , int );
typedef int(__stdcall *CPCL_AddImage)(void* , int ,int,int,const char*);

extern void* hprinter;


extern PrinterCreator                               DemoPrinterCreator;
extern PrinterCreatorS                              DemoPrinterCreatorS;
extern PrinterDestroy                               DemoPrinterDestroy;
extern PortOpen                                     DemoPortOpen;
extern PortClose                                    DemoPortClose;
extern DirectIO                                     DemoDirectIO;
extern CPCL_AddLabel                                AddLabel;
extern CPCL_SetAlign                                SetAlign;
extern CPCL_AddText                                 AddText;
extern CPCL_AddBarCode                              AddBarCode;
extern CPCL_AddBarCodeText                          AddBarCodeText;
extern CPCL_AddQRCode                               AddQRCode;
extern CPCL_AddPDF417                               AddPDF417;
extern CPCL_AddBox                                  AddBox;
extern CPCL_AddLine                                 AddLine;
extern CPCL_SetFontSize                             SetFontSize;
extern CPCL_SetDensity                              SetDensity;
extern CPCL_SetSpeed                                SetSpeed;
extern CPCL_SetTextSpacing                          SetTextSpacing;
extern CPCL_SetTextBold                             SetTextBold;
extern CPCL_SetTextUnderline                        SetTextUnderline;
extern CPCL_Print                                   LabelPrint;
extern CPCL_NextLabelPos                            NextLabelPos;
extern CPCL_PreFeed                                 PreFeed;
extern CPCL_PostFeed                                PostFeed;
extern CPCL_AddImage                                AddImage;

//TSPL
typedef int(__stdcall*TSPL_BitMap)(void* , int, int , int , const char*);//
typedef int(__stdcall*TSPL_Setup)(void* , int , int , int , int , int , int, int );//
typedef int(__stdcall*TSPL_ClearBuffer)(void* );//
typedef int(__stdcall*TSPL_Print)(void* , int , int );//
typedef int(__stdcall*TSPL_Direction)(void* , int );//
typedef int(__stdcall*TSPL_GetPrinterStatus)(void*, int*);//
typedef int(__stdcall*TSPL_Home)(void* );

extern TSPL_BitMap                                  DemBitMap;//
extern TSPL_Setup                                   DemSetup;//
extern TSPL_ClearBuffer                             DemClearBuffer;//
extern TSPL_Print                                   DemPrint;//
extern TSPL_Direction                               DemDirection;//
extern TSPL_GetPrinterStatus                        DemStatus;//
extern TSPL_Home                                    DemHome;

#endif // PRINTER_H
