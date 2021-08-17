#ifndef GAMECONNECTION_H
#define GAMECONNECTION_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class GameConnection : public QObject
{
    Q_OBJECT
public:
    explicit GameConnection(QString ip_addr, QObject *parent = nullptr);
    bool startServer();
    bool startClient();
    ~GameConnection();


signals:
    void MessageToGame(const QString str);

public slots:
    void GameToMessage(const QString& str);

private:

    void acquiredNewMessageFromClient();
    void readMessage();
    void writeMessage(const QString& message);

    QString role;
    QTcpServer* server = nullptr;
    QTcpSocket* client = nullptr;

    QHostAddress IP;

};

#endif // GAMECONNECTION_H
