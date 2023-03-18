#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QCloseEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>



namespace Ui {
class WebSocketServer;
}

//服务端
class WebSocketServer : public QWidget
{
    Q_OBJECT

public:
    explicit WebSocketServer(QWidget *parent = nullptr);
    ~WebSocketServer();

signals:
    void sendMessage(const QString &text);
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_stop_clicked();

private:
    void clearClient();
    void onNewConnection();

private:
    Ui::WebSocketServer *ui;

    QWebSocketServer *server;
    QList<QWebSocket*> clientList;
    QNetworkAccessManager manager;
    QNetworkRequest request;
    bool flag;
    void* mainPtr;
};

#endif // WEBSOCKETSERVER_H
