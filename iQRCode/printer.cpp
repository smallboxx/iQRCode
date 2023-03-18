#include "printer.h"
#include "ui_printer.h"
#include <QMessageBox>
#include "customlabel.h"
#include <QDebug>
#include <QFontDatabase>
extern int iq_w;
extern int iq_h;

printer::printer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::printer)
{
    ui->setupUi(this);
    temptimer=new QTimer;
    QRect page_rect(QPoint(iq_w/2,iq_h/2),QSize(iq_w/2,iq_w/1.8));
    this->setMinimumSize(QSize(iq_w/2,iq_w/1.8));
    this->setGeometry(page_rect);
    setWindowTitle("标签设计界面");
    ui->label_page->setStyleSheet("background:gray");
    tagpage=new QWidget(ui->label_page);
    resizeEvent(ev);
    QStringList zt_texts;
    zt_texts<<QString::fromUtf8("宋体")<<QString::fromUtf8("仿宋")<<QString::fromUtf8("楷体")
              <<QString::fromUtf8("微软雅黑")<<QString::fromUtf8("微软正黑")<<QString::fromUtf8("细明体")
                <<QString::fromUtf8("黑体");
    ui->comboBox_zt->addItems(zt_texts);
    QStringList zh_texts;
    zh_texts<<QString::fromUtf8("7")<<QString::fromUtf8("8")<<QString::fromUtf8("9")<<QString::fromUtf8("10")
              <<QString::fromUtf8("11")<<QString::fromUtf8("12")<<QString::fromUtf8("13")<<QString::fromUtf8("14")
                <<QString::fromUtf8("15")<<QString::fromUtf8("16")<<QString::fromUtf8("17")<<QString::fromUtf8("18");
    ui->comboBox_zh->addItems(zh_texts);
    connect(temptimer,&QTimer::timeout,this,&printer::load_templabel);
    temptimer->singleShot(100,this,&printer::load_templabel);
}

printer::~printer()
{
    delete ui;
}
void printer::load_templabel(){
    set_tagpage("temp.ini");
    for(CustomLabel *index:findChildren<CustomLabel*>()){
        if(index->objectName()==QString::fromUtf8("barcode")){
            QImage *img;
            img=new QImage;
            img->load("barcode.png");
            img->scaled(index->size(),Qt::KeepAspectRatio);
            index->setScaledContents(true);
            index->setPixmap(QPixmap::fromImage(*img));
        }
        else{
            index->adjustSize();
        }
        index->compute_r_geometry();
    }
}

void printer::closeEvent(QCloseEvent *event){
    emit close();
}

void printer::resizeEvent(QResizeEvent *event){
    QString textw=ui->lineEdit_w->text();
    QString texth=ui->lineEdit_h->text();
    if(!isnumber(textw)||!isnumber(texth)){
        QMessageBox::information(this,QString::fromUtf8("警告"),QString::fromUtf8("请输入0~9数字!"));
        return;
    }
    double linew=(ui->lineEdit_w->text()).toDouble();
    double lineh=(ui->lineEdit_h->text()).toDouble();
    double sysw=ui->label_page->width();
    double sysh=ui->label_page->height();
    resize_tag(linew,lineh,sysw,sysh);
}

void  printer::resize_tag(double linew, double lineh, double sysw, double sysh){
    if(linew > 80 || lineh > 80 || linew < 0 || lineh < 0){
        QMessageBox::information(this,QString::fromUtf8("警告"),QString::fromUtf8("标签宽高应小于等于80mm!"));
        return;
    }
    double ratio_w=linew/80;
    double ratio_h=lineh/80;
    double new_h=sysh*ratio_h;
    double new_w=sysh*ratio_w;
    QRect tag_rect(QPoint((sysw-new_w)/2,(sysh-new_h)/2),QSize(new_w,new_h));
    tagpage->setGeometry(tag_rect);
    tagpage->setStyleSheet("background:white");
    for(CustomLabel* index:tagpage->findChildren<CustomLabel*>()){
        double x=(index->rx)*new_w;
        double y=(index->ry)*new_h;
        double w=(index->rw)*new_w;
        double h=(index->rh)*new_h;
        QRect label_rect(QPoint(x,y),QSize(w,h));
        index->setGeometry(label_rect);
    }
}

void printer::on_lineEdit_w_editingFinished()
{
    if(ui->lineEdit_w->hasFocus()){
        resizeEvent(ev);
    }
}

void printer::on_lineEdit_h_editingFinished()
{
    if(ui->lineEdit_h->hasFocus()){
        resizeEvent(ev);
    }
}

bool printer::isnumber(QString str){
    QByteArray ba = str.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if(*s>='0' && *s<='9')
        {

        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}

void printer::mousePressEvent(QMouseEvent *event){
    int x=event->x()-ui->label_page->x()-tagpage->x();
    int y=event->y()-ui->label_page->y()-tagpage->y();
    emit send_cursor_xy(x,y);
}

void printer::send_cuatomlabel_message(){
    int w=tagpage->width();
    int h=tagpage->height();
    emit send_tagesize_message(w,h);
}

void printer::get_label_name(QString name){
    objname=name;
    CustomLabel* index=findChild<CustomLabel*>(name);
    ui->comboBox_zh->setCurrentText(QString::number(index->font().pointSize()));
    if(index->font().family()==QString::fromUtf8("SimHei")){
        ui->comboBox_zt->setCurrentText(QString::fromUtf8("黑体"));
    }
    if(index->font().family()==QString::fromUtf8("KaiTi")){
        ui->comboBox_zt->setCurrentText(QString::fromUtf8("楷体"));
    }
    if(index->font().family()==QString::fromUtf8("SimSun")){
        ui->comboBox_zt->setCurrentText(QString::fromUtf8("宋体"));
    }
    if(index->font().family()==QString::fromUtf8("Microsoft YaHei")){
        ui->comboBox_zt->setCurrentText(QString::fromUtf8("微软雅黑"));
    }
    if(index->font().family()==QString::fromUtf8("Microsoft JhengHei")){
        ui->comboBox_zt->setCurrentText(QString::fromUtf8("微软正黑"));
    }
    if(index->font().family()==QString::fromUtf8("MingLiU")){
        ui->comboBox_zt->setCurrentText(QString::fromUtf8("细明体"));
    }
    if(index->font().family()==QString::fromUtf8("FangSong_GB2312")){
        ui->comboBox_zt->setCurrentText(QString::fromUtf8("仿宋"));
    }
}

void printer::reset_button(QString name){
    QString buttonname="pushButton_"+name;
    QPushButton* index=findChild<QPushButton*>(buttonname);
    index->setDisabled(false);
}

void printer::new_customlabel(QString str, QString name){
    CustomLabel *text=new CustomLabel(tagpage);
    text->setObjectName(str);
    text->setText(name);
    text->show();
    connect(text,&CustomLabel::get_tagsize,this,&printer::send_cuatomlabel_message);
    connect(text,&CustomLabel::send_objname,this,&printer::reset_button);
    connect(text,&CustomLabel::send_font_objname,this,&printer::get_label_name);
    connect(this,&printer::send_tagesize_message,text,&CustomLabel::tagsize_message);
    connect(this,&printer::send_cursor_xy,text,&CustomLabel::set_label_status);
}
void printer::on_pushButton_wbpm_clicked()
{
    new_customlabel(QString::fromUtf8("wbpm"),QString::fromUtf8("外部品名：XXXX"));
    ui->pushButton_wbpm->setDisabled(true);

}

void printer::on_pushButton_wbkh_clicked()
{
    new_customlabel(QString::fromUtf8("wbkh"),QString::fromUtf8("外部款号：XXXX"));
    ui->pushButton_wbkh->setDisabled(true);
}

void printer::on_pushButton_ys_clicked()
{
    new_customlabel(QString::fromUtf8("ys"),QString::fromUtf8("颜色：XXXX"));
    ui->pushButton_ys->setDisabled(true);
}

void printer::on_pushButton_cm_clicked()
{
    new_customlabel(QString::fromUtf8("cm"),QString::fromUtf8("尺码：XXXX"));
    ui->pushButton_cm->setDisabled(true);
}

void printer::on_pushButton_xb_clicked()
{
    new_customlabel(QString::fromUtf8("xb"),QString::fromUtf8("性别：XXXX"));
    ui->pushButton_xb->setDisabled(true);
}

void printer::on_pushButton_zypl_clicked()
{
    new_customlabel(QString::fromUtf8("zypl"),QString::fromUtf8("主营品类：XXXX"));
    ui->pushButton_zypl->setDisabled(true);
}

void printer::on_pushButton_gys_clicked()
{
    new_customlabel(QString::fromUtf8("gys"),QString::fromUtf8("供应商：XXXX"));
    ui->pushButton_gys->setDisabled(true);
}

void printer::on_pushButton_lxhh_clicked()
{
    new_customlabel(QString::fromUtf8("lxhh"),QString::fromUtf8("乐销货号：XXXX"));
    ui->pushButton_lxhh->setDisabled(true);
}

void printer::on_pushButton_id_clicked()
{
    new_customlabel(QString::fromUtf8("id"),QString::fromUtf8("商品id：XXXX"));
    ui->pushButton_id->setDisabled(true);
}

void printer::on_pushButton_barcode_clicked()
{
    CustomLabel *text=new CustomLabel(tagpage);
    text->setObjectName(QString::fromUtf8("barcode"));
    QImage *img;
    img=new QImage;
    img->load("barcode.png");
    img->scaled(text->size(),Qt::KeepAspectRatio);
    text->setScaledContents(true);
    text->setPixmap(QPixmap::fromImage(*img));
    text->show();
    connect(text,&CustomLabel::get_tagsize,this,&printer::send_cuatomlabel_message);
    connect(text,&CustomLabel::send_objname,this,&printer::reset_button);
    connect(this,&printer::send_tagesize_message,text,&CustomLabel::tagsize_message);
    connect(this,&printer::send_cursor_xy,text,&CustomLabel::set_label_status);
    ui->pushButton_barcode->setDisabled(true);
}

QFont printer::get_font(QString ztstr,QString zhstr){
    QFont font;
    int zh=zhstr.toInt();
    if(ztstr==QString::fromUtf8("黑宋")){
        font.setFamily("SimHei");
    }
    if(ztstr==QString::fromUtf8("楷体")){
        font.setFamily("KaiTi");
    }
    if(ztstr==QString::fromUtf8("宋体")){
        font.setFamily("SimSun");
    }
    if(ztstr==QString::fromUtf8("微软雅黑")){
        font.setFamily("Microsoft YaHei");
    }
    if(ztstr==QString::fromUtf8("微软正黑")){
        font.setFamily("Microsoft JhengHei");
    }
    if(ztstr==QString::fromUtf8("细明体")){
        font.setFamily("MingLiU");
    }
    if(ztstr==QString::fromUtf8("仿宋")){
        font.setFamily("FangSong_GB2312");
    }
    font.setPointSize(zh);
    return font;
}

void printer::on_comboBox_zt_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBox_zt->hasFocus()){
        QString zhstr=ui->comboBox_zh->currentText();
        QFont font=get_font(arg1,zhstr);
        CustomLabel* index=findChild<CustomLabel*>(objname);
        index->setFont(font);
        index->adjustSize();
    }
}

void printer::on_comboBox_zh_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBox_zh->hasFocus()){
        QString ztstr=ui->comboBox_zt->currentText();
        QFont font=get_font(ztstr,arg1);
        CustomLabel* index=findChild<CustomLabel*>(objname);
        index->setFont(font);
        index->adjustSize();
    }
}

void printer::save_label(QString path){
    QSettings *config_page= new QSettings(path,QSettings::IniFormat);
    config_page->beginGroup("page");
    config_page->setValue("page_w",ui->lineEdit_w->text());
    config_page->setValue("page_h",ui->lineEdit_h->text());
    config_page->endGroup();
    double tagw=tagpage->width();
    double tagh=tagpage->height();
    QList<CustomLabel*> namelist=findChildren<CustomLabel*>();
    for (CustomLabel* index:namelist) {
        double x=index->x();
        double y=index->y();
        double w=index->width();
        double h=index->height();
        double rx=x/tagw;
        double ry=y/tagh;
        double rw=w/tagw;
        double rh=h/tagh;
        config_page->beginGroup(index->objectName());
        config_page->setValue("rx",rx);
        config_page->setValue("ry",ry);
        config_page->setValue("rw",rw);
        config_page->setValue("rh",rh);
        config_page->setValue("font",index->font().family());
        config_page->setValue("fontsize",index->font().pointSize());
        config_page->endGroup();
    }
    QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("保存文件成功"));
    delete  config_page;
}

void printer::on_pushButton_bc_clicked()
{
    QFileDialog fileDialog;
    QString fileName = fileDialog.getSaveFileName(this,tr("Save File"),"/label",tr("*.ini"));
    if(fileName == "")
    {
        return;
    }
    if(QFile::exists(fileName)){
        QFile::remove(fileName);
    }
    save_label(fileName);
}

void printer::set_tagpage(QString path){
    for(CustomLabel* index:tagpage->findChildren<CustomLabel*>()){
        index->del_click();
    }
    QSettings config_page(path,QSettings::IniFormat);
    config_page.beginGroup("page");
    int w=config_page.value("page_w").toInt();
    int h=config_page.value("page_h").toInt();
    ui->lineEdit_w->setText(QString::number(w));
    ui->lineEdit_h->setText(QString::number(h));
    resizeEvent(ev);
    config_page.endGroup();
    for(QString groupname:config_page.childGroups()){
        if(groupname=="page"){
            continue;
        }
        if(groupname=="wbpm"){
            on_pushButton_wbpm_clicked();
        }
        if(groupname=="wbkh"){
            on_pushButton_wbkh_clicked();
        }
        if(groupname=="ys"){
            on_pushButton_ys_clicked();
        }
        if(groupname=="cm"){
            on_pushButton_cm_clicked();
        }
        if(groupname=="xb"){
            on_pushButton_xb_clicked();
        }
        if(groupname=="zypl"){
            on_pushButton_zypl_clicked();
        }
        if(groupname=="gys"){
            on_pushButton_gys_clicked();
        }
        if(groupname=="lxhh"){
            on_pushButton_lxhh_clicked();
        }
        if(groupname=="id"){
            on_pushButton_id_clicked();
        }
        if(groupname=="barcode"){
            on_pushButton_barcode_clicked();
        }
        CustomLabel* text=findChild<CustomLabel*>(groupname);
        config_page.beginGroup(groupname);
        QFont  font;
        font.setFamily(config_page.value("font").toString());
        font.setPointSize(config_page.value("fontsize").toInt());
        text->setFont(font);
        double rx=config_page.value("rx").toDouble();
        double ry=config_page.value("ry").toDouble();
        double rw=config_page.value("rw").toDouble();
        double rh=config_page.value("rh").toDouble();
        int x=tagpage->width()*rx;
        int y=tagpage->height()*ry;
        int w=tagpage->width()*rw;
        int h=tagpage->height()*rh;
        QRect rect(QPoint(x,y),QSize(w,h));
        text->setGeometry(rect);
        config_page.endGroup();
    }
}

void printer::on_pushButton_zr_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),tr("*.ini"));
    if(path==""){
        return;
    }
    set_tagpage(path);
}

void printer::on_pushButton_mr_clicked()
{
    set_tagpage("default.ini");
}

void printer::on_pushButton_dqmb_clicked()
{
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this, QString::fromUtf8("提示"), QString::fromUtf8("是否使用当前模板"), QMessageBox::Yes|QMessageBox::No);
    if(box==QMessageBox::No)
    {
        return;
    }
    if(QFile::exists("temp.ini")){
        QFile::remove("temp.ini");
    }
    save_label("temp.ini");
    emit  send_mainwindow();
}

void printer::on_pushButton_qx_clicked()
{
    emit close();
}
