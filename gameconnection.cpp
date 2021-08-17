#include "gameconnection.h"

GameConnection::GameConnection(QString role, QString ip_addr, QObject *parent) : QObject(parent)
{
    this->role = role;
    IP = QHostAddress(ip_addr);
}

void GameConnection::start(){
    if (role == "Server") {
        startServer();
    } else {
        startClient();
    }
}

bool GameConnection::startServer() {
    role = "Server";

    server = new QTcpServer();
    bool bind = server->listen(IP, 23333);
    if (!bind) {
        return false;
    }

    connect(server, &QTcpServer::newConnection, this, &GameConnection::acquiredNewMessageFromClient);

    return true;
}

bool GameConnection::startClient() {
    role = "Client";
    client = new QTcpSocket();
    client->connectToHost(IP, 23333);

    connect(client, &QTcpSocket::readyRead, this, &GameConnection::readMessage);
    client->write("123");
    return true;

}

GameConnection::~GameConnection() {
    delete server;
    delete client;
}

// Used for Server: Establish socket and connect readyRead with parseMessage
void GameConnection::acquiredNewMessageFromClient() {
    client = server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &GameConnection::readMessage);
}

void GameConnection::readMessage() {
    QString message = client->readAll();
    // TODO : parse Message

    emit MessageToGame(message);
}

void GameConnection::writeMessage(const QString& message) {
    // TODO : parse Message
    int sendStatus = client->write(message.toStdString().c_str());
    if (sendStatus == -1){
        // TODO: catch network error
    }
}

void GameConnection::GameToMessage(const QString& message) {
    writeMessage(message);
}
