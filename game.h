#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "gameconnection.h"
#include <QThread>

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

public:
    Game();
    ~Game();

    GameStatus getStatus() { return status; }    

    void startConnection(QString role);

};

#endif // GAME_H
