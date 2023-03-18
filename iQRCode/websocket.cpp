#include "websocket.h"
#include "ui_websocket.h"
extern int iq_w;
extern int iq_h;

websocket::websocket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::websocket)
{
    ui->setupUi(this);
    QRect web_rect(QPoint(iq_w*1.5,iq_h/2),QSize(iq_h/2,iq_h/2+70));
    this->setMinimumSize(iq_h/2,iq_h/2);
    this->setGeometry(web_rect);
    ui->wx_qrcode_label->setStyleSheet("background:white");
    setWindowTitle("wx");
    QFile file("./wx_takephoto_iqrcode.jpg");
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    QImage img = QImage::fromData(data);
    int w_la=this->width()-50;
    int h_la=this->height()-50;
    img = img.scaled(w_la,h_la,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->wx_qrcode_label->setPixmap(QPixmap::fromImage(img));
    flag=false;


//    构造：QWebSocketServer（const QString& serverName,QWebSocketServer::SslMode secureMode,QObject *parent=nullptr）
//    使用给定的serverName构造一个新的QWebSocketServer。
//    该服务器名称将在HTTP握手阶段被用来识别服务器。它可以为空，此时不会将服务器名称发送给客户端。
//    SslMode指示服务器是通过wss（SecureMode）还是ws（NonSecureMode）运行
//    QWebSocketServer::SecureMode服务器以安全模式运行（通过wss）
//    QWebSocketServer::NonSecureMode服务器以非安全模式运行（通过ws）
    server=new QWebSocketServer("Server",QWebSocketServer::NonSecureMode,this);

    //服务器监听
    QHostAddress address;
    address=QHostAddress::Any;
    //判断是否连接上
    if(server->listen(address,9525)){
        qDebug()<<"connect_yes";
    }
    else {
        qDebug()<<"disconnect";
    }
    //新的连接进来
    connect(server,&QWebSocketServer::newConnection,this,&websocket::onNewConnection);
}

websocket::~websocket()
{
    delete ui;
}

void websocket::clearClient()
{
    for(int i=clientList.count()-1;i>=0;i--)
    {
        clientList.at(i)->disconnect();
        clientList.at(i)->close();
    }
    qDeleteAll(clientList);
    clientList.clear();
}

void websocket::onNewConnection()
{
    QWebSocket *socket=server->nextPendingConnection();
    if(!socket)
        return;
    clientList.push_back(socket);
    //收到消息
    connect(socket,&QWebSocket::textMessageReceived,[this](const QString &msg){
        qDebug()<<msg;
        request.setUrl(msg);
        QNetworkReply *reply=manager.get(request);
        QEventLoop loop;
        connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
        loop.exec();
        int iError = reply->error();
        if(iError == QNetworkReply::NoError){
            QFile file("./temp_takephoto.png");
            if(!file.open(QIODevice::WriteOnly))
                return false;
            file.write(reply->readAll());
            file.close();
            delete reply;
            flag=true;
        }
        if(flag){
            flag=false;
            QString filename("temp_takephoto.png");
            emit upload(filename);
        }
    });
    //断开连接，释放
    connect(socket,&QWebSocket::disconnected,[this,socket](){
        clientList.removeAll(socket);
        socket->deleteLater();
    });
}

void websocket::closeEvent(QCloseEvent * event)
{
    emit close();
    clearClient();
    server->close();
    server->deleteLater();
}
