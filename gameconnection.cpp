#include "gameconnection.h"
#include "unistd.h"
#include <QThread>

GameConnection::GameConnection(QString role, QString ip_addr, QObject *parent) : QObject(parent)
{
    this->role = role;
    IP = QHostAddress(ip_addr);
}

// Start connection
void GameConnection::start(){
    if (role == "Server") {
        startServer();
    } else {
        startClient();
    }
}

// Start connection and run as server
bool GameConnection::startServer() {
    role = "Server";
    emit MessageToGame("0 3D9140 Starting server...");

    server = new QTcpServer();
    bool bind = server->listen(IP, 23333);
    if (!bind) {
        emit MessageToGame("0 E3170D Cannot bind to that port! Please cancel the connection and try again!");
        return false;
    }
    emit MessageToGame("0 4169E1 Bind successfully. Listening on " + IP.toString() + " !");

    connect(server, &QTcpServer::newConnection, this, &GameConnection::acquiredNewConnectionFromClient);

    return true;
}

// Start connection and run as client
bool GameConnection::startClient() {
    emit MessageToGame("0 3D9140 Starting client...");
    role = "Client";
    client = new QTcpSocket();


    connect(client, &QTcpSocket::connected, this,  ([=](){emit MessageToGame("0 4169E1 Connected successfully. Connecting to " + IP.toString() + " !");})  );
    connect(client, &QTcpSocket::errorOccurred, this,  ([=](){emit MessageToGame("0 E3170D Connection Failed! Please cancel the connection and try again!");})  );
    connect(client, &QTcpSocket::readyRead, this, &GameConnection::readMessage);
    client->connectToHost(IP, 23333);

    return true;
}

// Used for Server: When acquired new connection request, establish socket and connect readyRead with parseMessage
void GameConnection::acquiredNewConnectionFromClient() {
    emit MessageToGame("0 F4A460 Found new connection from client!");
    client = server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &GameConnection::readMessage);
    connect(client, &QTcpSocket::errorOccurred, this,  ([=](){emit MessageToGame("0 E3170D Connection Failed! Please cancel the connection and try again!");})  );
    writeMessage("200");
}

// Used for Universal: readMessage from Socket
void GameConnection::readMessage() {
    QString message = client->readAll();
    emit MessageToGame(message);
}

// Used for Universal: writeMessage into Socket
void GameConnection::writeMessage(const QString& message) {
    int result = client->write(message.toStdString().c_str());
    if (result == -1) {
        emit client->errorOccurred(QTcpSocket::SocketTimeoutError);
    }
}

// Slot function for Game::writeMessage
void GameConnection::GameToMessage(const QString& message) {
    writeMessage(message);
}

GameConnection::~GameConnection() {
    if (server) {
        server->deleteLater();
        server = nullptr;
    }
    if (client) {
        client->deleteLater();
        server = nullptr;
    }
}
