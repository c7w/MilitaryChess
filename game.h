#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "gameconnection.h"
#include <QThread>
#include <QVector>
#include <QLabel>
#include "constants.h"


enum GameStatus {
    OFFLINE,
    HOSTING,
    CONNECTING,
    READY,
    PLAYING,
    WAITING,
    END
};

class Game : public QObject
{
    Q_OBJECT
private:
    GameStatus status;
    QThread connectionThread;
    GameConnection* connection;

    QVector<QLabel*>* icons;

signals:
    void initConnection();
    void writeData(const QString& str);

public slots:
    void getData(const QString& str);

public:
    Game();
    ~Game();

    GameStatus getStatus() { return status; }    
    void setIcons(QVector<QLabel*>* icons) { this->icons = icons; }

    void setIcon(int pos, Constants::ChessPiece piece);
    void initIcon();

    void startConnection(QString role, QString ip_addr);

};

#endif // GAME_H
