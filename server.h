#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtDebug>
#include <QVector>

class server : public QObject
{
    Q_OBJECT
private:
    QTcpServer* chatServer;
    QVector<QTcpSocket*>* allClients;
public:
    explicit server(QObject *parent = nullptr);
    void startServer();
    void sendMessageToCliets(QString message);
signals:

public slots:
    void newClientConnection();
    void socketDisconnected();
    void socketReayRead();
    void socketStateChanged(QAbstractSocket::SocketState state);
};

#endif // SERVER_H
