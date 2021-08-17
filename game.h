#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "gameconnection.h"
#include <QThread>
#include <QVector>
#include <QLabel>
#include "constants.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

public:
    Game();
    ~Game();

    GameStatus getStatus() { return status; }    
    void setIcons(QVector<QLabel*>* icons) { this->icons = icons; }

    void setIcon(int pos, Constants::ChessPiece piece);
    void initIcon();

    void startConnection(QString role);

};

#endif // GAME_H
