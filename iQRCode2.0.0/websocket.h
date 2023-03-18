#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QCloseEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ui {
class websocket;
}

class websocket : public QWidget
{
    Q_OBJECT

public:
    explicit websocket(QWidget *parent = nullptr);
    ~websocket();

signals:
    void sendMessage(const QString &text);
    void upload(QString);
    void close();
protected:
    void closeEvent(QCloseEvent *event);

private:
    void clearClient();
    void onNewConnection();

private:
    Ui::websocket *ui;
    QWebSocketServer *server;
    QList<QWebSocket*> clientList;
    QNetworkAccessManager manager;
    QNetworkRequest request;
    bool flag;
};

#endif // WEBSOCKET_H
