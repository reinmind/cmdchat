#include "server.h"

server::server(QObject *parent) : QObject(parent)
{

}
void server::startServer()
{
    allClients = new QVector<QTcpSocket*>;

    chatServer = new QTcpServer();
    chatServer -> setMaxPendingConnections(10);
    connect(chatServer,SIGNAL(newConnection()),this,
            SLOT(newClientConnection()));

    if(chatServer->listen(QHostAddress::Any,8002))
    {
        qDebug() << "Server has started. Listening to port 8002";
    }
    else
    {
        qDebug() << "Server failed to start.Error: "+ chatServer -> errorString();

    }

}

void server::newClientConnection()
{
    QTcpSocket* client = chatServer -> nextPendingConnection();
    QString ipAddress = client -> peerAddress().toString();
    int port = client -> peerPort();

    connect(client,&QTcpSocket::disconnected,this,&server::socketDisconnected);
    connect(client,&QTcpSocket::readyRead,this,&server::socketReayRead);
    connect(client,&QTcpSocket::stateChanged,this,&server::socketStateChanged);

    allClients -> push_back(client);

    qDebug() << "Socket connected from " + ipAddress + ":" +
                QString::number(port);
}

void server::socketDisconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();
    qDebug() << "Socket disconnected from "+socketIpAddress + ":" +
                QString::number(port);
}

void server::socketReayRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();

    QString data = QString(client->readAll());

    qDebug() << "Message: " + data + "(" + socketIpAddress + ":" +
                QString::number(port) + ")";

    sendMessageToCliets(data);
}

void server::socketStateChanged(QAbstractSocket::SocketState state)
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();

    QString desc;

    if(state == QAbstractSocket::UnconnectedState)
        desc = "The socket is not connected.";
    else if (state == QAbstractSocket::HostLookupState)
        desc = "The socket is performing a host name lookup";
    else if (state == QAbstractSocket::ConnectingState)
        desc = "The socket has started establishing a connection.";
    else if (state == QAbstractSocket::ConnectedState)
        desc = "A connection is established.";
    else if (state == QAbstractSocket::BoundState)
        desc = "The socket is about to close (data may still be waiting to be written).";
    else if (state == QAbstractSocket::ListeningState)
        desc = "For internal use only";

    qDebug() << "Socket state changed (" + socketIpAddress + ":" +
                QString::number(port) + "):" + desc;
}
void server::sendMessageToCliets(QString message)
{
    if(allClients->size() > 0)
    {
        for(int i = 0;i < allClients->size();i++)
        {
            if(allClients->at(i)->isOpen() && allClients -> at(i)
                    -> isWritable())
            {
                allClients->at(i)->write(message.toUtf8());
            }
        }
    }
}
