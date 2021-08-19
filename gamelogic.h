#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include "game.h"
#include "endgame.h"

class Game;


class GameLogic : public QObject
{
    Q_OBJECT
private:

    static QString WriteChessboardToString(Game* game);

    struct AR {QSet<int> Approachable; QSet<int> Eatable; QSet<int> EatableWhileSelfDestroyed;};

    // Related to click event
    static int OperationRecord;
    static AR AffectedRecord;

    static void DFS(Game* game, int initId, int next);
    static int GenerateRecordForPosition(Game* game, int pos);

    static void MovePiece(Game* game, int from, int to);
    static void EatPiece(Game* game, int from, int to);
    static void EatPieceWhileSelfDestroyed(Game* game, int from, int to);

public:
    explicit GameLogic(QObject *parent = nullptr);

    static void onGetReady(Game* game);
    static void tryStartGame(Game* game);
    static void startGameLoop(Game* game);
    static void startTurn(Game* game, const QVector<QString>& args);
    static void endTurn(Game* game);
    static void turnTimeout(Game* game);
    static void turnTimeoutOpponent(Game* game);

    static void MessageProcess(Game* game, const QString& message);
    static void clickBoard(Game* game, int pos);

    static void winGame(Game* game, bool isMe);


signals:

};

#endif // GAMELOGIC_H
