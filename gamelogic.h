#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include "game.h"

class Game;

class GameLogic : public QObject
{
    Q_OBJECT
public:
    explicit GameLogic(QObject *parent = nullptr);
    static void MessageProcess(Game* game, const QString& message);

public:


signals:

};

#endif // GAMELOGIC_H
