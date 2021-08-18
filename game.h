#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "gameconnection.h"
#include <QThread>
#include <QVector>
#include <QLabel>
#include "constants.h"
#include "chesspiece.h"
#include "gamelogic.h"

class GameLogic;

enum GameStatus {
    OFFLINE,
    HOSTING,
    CONNECTING,
    WAIT_PLAY_CONFIRMATION,
    READY,
    PLAYING_WAITING,
    PLAYING_THINKING,
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
    void setIcon(int pos, EnumChessPiece piece);

    QVector<QLabel*>* icons; // [0,60), namely sixty labels

    // Ingame
    QVector<int> board; // Map [0, 60) to [0, 50], while 0 means there does not exist a chess piece, and 1~50 refers to initialized ID
    QVector<ChessPiece*> pieces = {nullptr,}; // [1, 50], takes control of 50 chess pieces in initID order.
    struct {Faction ColorMe; int TurnCount; Faction ColorNow; QString LeftTime; } GameInfo;

signals:
    void initConnection();
    void setPrompt(const QString& message);
    void writeData(const QString& str);

public slots:
    void getData(const QString& str);
    void onPressedBoard(int pos);


public:
    Game();
    ~Game();

    GameStatus getStatus() { return status; }    

    void setIcons(QVector<QLabel*>* icons) { this->icons = icons; }

    void startConnection(QString role, QString ip_addr);
    bool cancelConnection();

    // Init game
    void initChessboard(); // Generate 50 new Chess pieces and shuffle chess pieces across chess board;
    void initIcon(); // Render 60 icons
    void updateIcon(int pos);

    // Ingame
//    int getInitIDfromBoard(int pos) {return this->board[pos];}
//    void setBoard(int pos, int initID) {this->board[pos] = initID;}
//    ChessPiece* getPieces(int initID) {return this->pieces[initID];}



};

#endif // GAME_H
