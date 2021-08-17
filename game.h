#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "gameconnection.h"
#include <QThread>
#include <QVector>
#include <QLabel>
#include "constants.h"
#include "gamelogic.h"

class GameLogic;

enum GameStatus {
    OFFLINE,
    HOSTING,
    CONNECTING,
    WAIT_PLAY_CONFIRMATION,
    READY,
    PLAYING,
    WAITING,
    END
};

class Game : public QObject
{
    Q_OBJECT

friend class GameLogic;

private:
    GameStatus status;
    QThread connectionThread;
    GameConnection* connection;

    QVector<QLabel*>* icons;

signals:
    void initConnection();
    void setPrompt(const QString& message);
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
    bool cancelConnection();

};

#endif // GAME_H
