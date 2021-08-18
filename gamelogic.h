#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include "game.h"

class Game;


class GameLogic : public QObject
{
    Q_OBJECT
private:

    struct AR {QVector<int> Approachable; QVector<int> Eatable; QVector<int> EatableWhileSelfDestroyed;};

    // Related to click event
    static int OperationRecord;
    static AR AffectedRecord;
    static void GenerateRecordForPosition(Game* game, int pos);

    static void MovePiece(Game* game, int from, int to);
    static void EatPiece(Game* game, int from, int to);
    static void EatPieceWhileSelfDestroyed(Game* game, int from, int to);


public:
    explicit GameLogic(QObject *parent = nullptr);
    static void MessageProcess(Game* game, const QString& message);
    static void clickBoard(Game* game, int pos);


signals:

};

#endif // GAMELOGIC_H
