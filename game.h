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
#include <QTimer>

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
public:
    struct Info {QString ColorMe; QString ColorOpponent; int TurnCount=0; int TimeOut=3; QString ColorNow; int LeftTime=20; };

private:
    GameStatus status;

    // Used for pre-game communications
    QString role;
    QVector<bool> readyState = {false, false}; // Only Server would use this

    // Used for establishing connections
    QThread connectionThread;
    GameConnection* connection;
    void setIcon(int pos, EnumChessPiece piece);

    QVector<QLabel*>* icons; // [0,60), namely sixty labels

    // Ingame
    int offensive;
    Faction color = None;
    QVector<bool> gameState = {false, false}; // Opponent JunQi ready to be eaten; Faction decided.
    QVector<int> board; // Map [0, 60) to [0, 50], while 0 means there does not exist a chess piece, and 1~50 refers to initialized ID
    QVector<ChessPiece*> pieces = {nullptr,}; // [1, 50], takes control of 50 chess pieces in initID order.
    QTimer* timer = nullptr;

signals:
    void initConnection();
    void enablePlayButton();
    void disablePlayButton();
    void enableAdmitDefeatButton();
    void setInfo(const Info info);
    void setPrompt(const QString& message);
    void writeData(const QString& str);

public slots:
    void onGetReady(); // Slots function for menubar: get ready
    void onAdmitDefeat();

    void turnTimeout();
    void turnTimeoutOpponent();
    void getData(const QString& str);
    void onPressedBoard(int pos);



public:
    Game();
    ~Game();


    struct Info GameInfo;

    GameStatus getStatus() { return status; }    

    void setIcons(QVector<QLabel*>* icons) { this->icons = icons; }

    void startConnection(QString role, QString ip_addr);
    bool cancelConnection();

    // Init game
    void initChessboard(); // Generate 50 new Chess pieces and shuffle chess pieces across chess board;
    void initIcon(); // Render 60 icons
    void updateIcon(int pos);

    // Ingame
    QVector<Faction> revealedHistory;
//    int getInitIDfromBoard(int pos) {return this->board[pos];}
//    void setBoard(int pos, int initID) {this->board[pos] = initID;}
//    ChessPiece* getPieces(int initID) {return this->pieces[initID];}



};

#endif // GAME_H
