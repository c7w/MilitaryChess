#include "gamelogic.h"
#include <iostream>
#include <sstream>
#include <time.h>

GameLogic::GameLogic(QObject *parent) : QObject(parent)
{

}

QString formatPrompt(QString color, QString message) {
    return "<p style=\"color: #"+ color +"\">" + message + "</p>";
}

int GameLogic::OperationRecord = {-1,};
GameLogic::AR GameLogic::AffectedRecord = {{}, {}, {}};

QString GameLogic::WriteChessboardToString(Game* game) {
    QString result = QString::number(game->board[0]);
    for (int i = 1; i < 60; ++i) {
        result += ' ';
        result += QString::number(game->board[i]);
    }
    return result;
}

void GameLogic::MessageProcess(Game* game, const QString& raw_message) {
    QVector<QString> args;

    {
        std::stringstream ss(raw_message.toStdString());
        std::string s;
        while(std::getline(ss, s, ' ')) {
            args.push_back(QString::fromStdString(s));
        }
    }

    // 0: set prompt message
    if (args[0] == "0") {
        QString message = args[2];
        for(int i = 3; i < args.length(); i++) {
            message += ' ';
            message += args[i];
        }
        emit game->setPrompt(formatPrompt(args[1], message));
    }

    // 200: Connection Established
    if(args[0] == "200") {
        if(game->getStatus() == HOSTING) {
            // TODO: Create chessboard
            game->initChessboard();
            game->initIcon();
            // TODO: Send chessboard
            emit game->writeData("201 " + WriteChessboardToString(game));
            emit game->setPrompt(formatPrompt("6F46F1", "Sending data to server!"));
        } else if (game->getStatus() == CONNECTING) {
            emit game->setPrompt(formatPrompt("6F46F1", "Receiving data from server..."));
            emit game->writeData("200 OK");
        }
        return;
    }

    // 201 InitChessboard
    if(args[0] == "201") {
        if (game->getStatus() == CONNECTING){
            // Generate
            for (int i = 1; i <= 50; ++i) {
                game->pieces.push_back(ChessPiece::getChesePiece(i));
            }

            for(int i = 1; i <= 60; ++i) {
                game->board.push_back(args[i].toInt());
            }
            game->initIcon();
            emit game->writeData("201 Received");
        }

        game->status = WAIT_PLAY_CONFIRMATION;
        emit game->setPrompt(formatPrompt("872657", "Please get ready!"));

        // Change Menubar: enable "Play"
        emit game->enablePlayButton();

        return;
    }

    // 202 Client Ready
    if( args[0] == "202" ) {
        if(game->role == "Server" ){
            game->readyState[1] = true;
            tryStartGame(game);
        }
    }

    // 203 Game start
    if (args[0] == "203") {
        if (game->role != "Server") {
            game->offensive = args[1].toInt();
            startGameLoop(game);
        }
    }


    // 300 Unreveal chess
    if(args[0] == "300") {
        int pos = args[1].toInt();
        game->pieces[game->board[pos]]->setRevealed();
        game->updateIcon(pos);
        return;
    }
    // 301 Move chess
    if(args[0] == "301") {
        MovePiece(game, args[1].toInt(), args[2].toInt());
        return;
    }
    // 302 Eat chess
    if(args[0] == "302") {
        EatPiece(game, args[1].toInt(), args[2].toInt());
        return;
    }
    // 303 Eat chess and self-destroy
    if(args[0] == "303") {
        EatPieceWhileSelfDestroyed(game, args[1].toInt(), args[2].toInt());
        return;
    }

    // 400 Switch Turn
    if( args[0] == "400" ) {
        // Kill timer
        if(game->timer) {
            game->timer->stop();
            delete game->timer;
            game->timer = nullptr;
        }

        startTurn(game, args);
        game->status = PLAYING_THINKING;
    }

    // 401 Win Game
    if ( args[0] == "401" ) {
        if (args[1] == "1") winGame(game, true);
        else winGame(game, false);
    }

    // 402 Connection Error
    if ( args[0] == "402" ) {
        // Kill timer
        if(game->timer) {
            game->timer->stop();
            delete game->timer;
            game->timer = nullptr;
        }
        game->status = END;
    }
}


void GameLogic::onGetReady(Game *game) {

    emit game->setPrompt(formatPrompt("E14890", "Waiting your opponent to get ready..."));

    if (game->role == "Server") {
        game->readyState[0] = true;
        tryStartGame(game);
    } else {
        emit game->writeData("202 Ready");
    }
}

// Server side: try start game
void GameLogic::tryStartGame(Game *game) {
    if ( game->readyState[0] && game->readyState[1]) {

        // Rand the offensive
        srand((unsigned)time(NULL));
        game->offensive = rand() % 2; // If 0 server takes the offensive, and 1 for client
        startGameLoop(game);

        emit game->writeData("203 " + QString::number(game->offensive));
    }
}

// Both side: start game loop
void GameLogic::startGameLoop(Game *game) {
    game->GameInfo.ColorNow = "■(Offensive)";
    game->GameInfo.TurnCount = 0;
    game->GameInfo.LeftTime = 20;
    //  0 Server first
    const bool MeFirst = (game->role == "Server" && game->offensive == 0) || (game->role != "Server" && game->offensive == 1);
    if ( MeFirst ) {
        game->GameInfo.ColorMe = "■(Offensive)";
        game->GameInfo.ColorOpponent = "■(Defensive)";
        game->status = PLAYING_THINKING;
        emit game->setPrompt(formatPrompt("36E1CC", "Game started! You First!"));
        startTurn(game, {});
    } else {
        game->GameInfo.ColorMe = "■(Defensive)";
        game->GameInfo.TurnCount = 1;
        game->GameInfo.ColorOpponent = "■(Offensive)";
        game->status = PLAYING_WAITING;
        emit game->setPrompt(formatPrompt("36E1CC", "Game started! Your opponent took the first hand!"));

        // New timer
        game->timer = new QTimer();
        connect(game->timer, &QTimer::timeout, game, &Game::turnTimeoutOpponent);
        game->timer->start(1000);
    }
    emit game->setInfo(game->GameInfo);

}

void GameLogic::DFS(Game *game, int initId, int next) {
    if ( AffectedRecord.Approachable.contains(next) || AffectedRecord.Eatable.contains(next) || AffectedRecord.EatableWhileSelfDestroyed.contains(next)) return;
    int toId = game->board[next];
    if ( toId == 0 ) {
        AffectedRecord.Approachable.insert(next);
        int side[] = {5, 1, -5, -1};
        for (auto d : side) {
            if(Constants::hasEdge(next, next+d) == 2) DFS(game, initId, next+d);
        }

    } else {
        if (! game->pieces[toId]->revealed) { return; }
        if ( game->pieces[initId]->canEat(game->pieces[toId]) == 1) {
            if(game->pieces[toId]->getArmType() == JunQi && !game->gameState[0]) return;
            AffectedRecord.Eatable.insert(next);
        }
        if ( game->pieces[initId]->canEat(game->pieces[toId]) == 2) { AffectedRecord.EatableWhileSelfDestroyed.insert(next); }
    }
}

int GameLogic::GenerateRecordForPosition(Game *game, int pos) {
    AffectedRecord.Approachable.clear();
    AffectedRecord.Eatable.clear();
    AffectedRecord.EatableWhileSelfDestroyed.clear();

    if (pos == -1) {
        return 0;
    }

    // First retrieve the ChessPiece of the position
    int initID = game->board[pos];
    if ( initID == 0 ) { return 0; }
    if(game->pieces[game->board[pos]]->getFaction() != game->color) return 0;

    ArmType arm = game->pieces[initID]->getArmType();
    if ( arm == JunQi || arm == DiLei) return 0;

    int delta[] = {5,6,1,-4,-5,-6,-1,4};
    for(auto d : delta) {
        int hasEdge = Constants::hasEdge(pos, pos+d);
        if (hasEdge == 1) {
            int toId = game->board[pos+d];
            if (toId == 0) {AffectedRecord.Approachable.insert(pos+d); continue;}
            if (! game->pieces[toId]->revealed) { continue; }
            if ( game->pieces[initID]->canEat(game->pieces[toId]) == 1) {
                if(game->pieces[toId]->getArmType() == JunQi && !game->gameState[0]) continue;
                AffectedRecord.Eatable.insert(pos+d); continue;
            }
            if ( game->pieces[initID]->canEat(game->pieces[toId]) == 2) {
                AffectedRecord.EatableWhileSelfDestroyed.insert(pos+d); continue;
            }
            continue;
        }
        if(hasEdge == 2) {
            if (arm != GongBing) {
                int target = pos;
                while(Constants::hasEdge(target, target+d) == 2 && game->board[target+d] == 0) {
                    AffectedRecord.Approachable.insert(target+d); target +=d ;
                }
                if(Constants::hasEdge(target, target+d) == 2) {
                    int toId = game->board[target+d];
                    if (! game->pieces[toId]->revealed) { continue; }
                    if ( game->pieces[initID]->canEat(game->pieces[toId]) == 1) {
                        if(game->pieces[toId]->getArmType() == JunQi && !game->gameState[0]) continue;
                        AffectedRecord.Eatable.insert(target+d); continue;
                    }
                    if ( game->pieces[initID]->canEat(game->pieces[toId]) == 2) {
                        AffectedRecord.EatableWhileSelfDestroyed.insert(target+d); continue;
                    }
                    continue;
                }
            } else {
                DFS(game, initID, pos+d);
            }

        }
    }
    return AffectedRecord.Approachable.size() + AffectedRecord.Eatable.size() + AffectedRecord.EatableWhileSelfDestroyed.size();

}

void GameLogic::MovePiece(Game *game, int from, int to) {
    game->board[to] = game->board[from];
    game->board[from] = 0;

    game->pieces[game->board[to]]->isInCamp = false;
    for(auto x: Constants::Camp) {
        if (to == x || to == x+25) {
            game->pieces[game->board[to]]->isInCamp = true;
            break;
        }
    }

    game->updateIcon(from);
    game->updateIcon(to);


}

void GameLogic::EatPiece(Game *game, int from, int to) {
    int toId = game->board[to];
    game->pieces[toId]->setEaten();

    game->board[to] = game->board[from];
    game->board[from] = 0;

    game->pieces[game->board[to]]->isInCamp = false;
    for(auto x: Constants::Camp) {
        if (to == x || to == x+25) {
            game->pieces[game->board[to]]->isInCamp = true;
            break;
        }
    }

    game->updateIcon(from);
    game->updateIcon(to);

}

void GameLogic::EatPieceWhileSelfDestroyed(Game *game, int from, int to) {
    int fromId = game->board[from];
    int toId = game->board[to];
    game->pieces[fromId]->setEaten();
    game->pieces[toId]->setEaten();

    game->board[from] = 0;
    game->board[to] = 0;

    game->updateIcon(from);
    game->updateIcon(to);

}

void GameLogic::clickBoard(Game *game, int pos) {

    bool related = false; // Whether pass this turn
    if (game->getStatus() != PLAYING_THINKING) related = true;
    // Reveal Unknown
    if (!related && pos >= 0 && game->board[pos] != 0 && !game->pieces[game->board[pos]]->revealed) {
        game->pieces[game->board[pos]]->setRevealed();
        game->updateIcon(pos);
        emit game->writeData("300 " + QString::number(pos));
        game->revealedHistory.push_back((Faction)(game->board[pos]%2));
        related = true;
    }

    if (!related) {
        // Test if pos falls in AffectedRecord
        int x;
        foreach(x, AffectedRecord.Approachable) {
            if ( pos == x ) {
                MovePiece(game, OperationRecord, pos);
                emit game->writeData("301 " + QString::number(OperationRecord) + " " + QString::number(pos));
                related = true;
            }
        }
        foreach(x, AffectedRecord.Eatable) {
            if ( pos == x ) {
                EatPiece(game, OperationRecord, pos);
                emit game->writeData("302 " + QString::number(OperationRecord) + " " + QString::number(pos));
                related = true;
            }
        }
        foreach(x, AffectedRecord.EatableWhileSelfDestroyed) {
            if( pos == x ){
                EatPieceWhileSelfDestroyed(game, OperationRecord, pos);
                emit game->writeData("303 " + QString::number(OperationRecord) + " " + QString::number(pos));
                related = true;
            }
        }
    }

    // Clean styles
    if(OperationRecord >= 0) {
        game->icons->at(OperationRecord)->setStyleSheet("");
        int x;
        foreach(x , AffectedRecord.Approachable) game->icons->at(x)->setStyleSheet("");
        foreach(x , AffectedRecord.Eatable) game->icons->at(x)->setStyleSheet("");
        foreach(x , AffectedRecord.EatableWhileSelfDestroyed) game->icons->at(x)->setStyleSheet("");
    }


    // Generate new related positions
    if (related) GenerateRecordForPosition(game, -1);
    else {
        GenerateRecordForPosition(game, pos);
    }

    // Make styles
    if (pos >= 0) {
        if (related) game->icons->at(pos)->setStyleSheet("border-width: 3px; border-style: outset solid; border-color: yellow; border-radius: 5px;");
        else game->icons->at(pos)->setStyleSheet("border-width: 3px; border-style: outset solid; border-color: red; border-radius: 5px;");

        int x;
        foreach(x , AffectedRecord.Approachable) game->icons->at(x)->setStyleSheet("border-width: 3px; border-style: outset solid; border-color: green; border-radius: 5px;");
        foreach(x , AffectedRecord.Eatable) game->icons->at(x)->setStyleSheet("border-width: 3px; border-style: outset solid; border-color: #FF9999; border-radius: 5px;");
        foreach(x , AffectedRecord.EatableWhileSelfDestroyed) game->icons->at(x)->setStyleSheet("border-width: 3px; border-style: outset solid; border-color: #FF99FF; border-radius: 5px;");
    }



    OperationRecord = pos;
    if (related && game->getStatus() == PLAYING_THINKING) {
        endTurn(game);
    }

}

void GameLogic::startTurn(Game *game, const QVector<QString> &args) {
    if (game->color == None && args.length() > 1) {
        if (args[1] == "Blue") game->color = Blue;
        if (args[1] == "Red") game->color = Red;
        game->GameInfo.ColorMe = formatPrompt(args[1] == "Blue" ? "0000ff" : "ff0000", "■");
    }
    game->GameInfo.TurnCount += 1;
    if (game->GameInfo.TurnCount == 21) emit game->enableAdmitDefeatButton();
    game->GameInfo.ColorNow = game->GameInfo.ColorMe;
    game->GameInfo.LeftTime = 20;
    emit game->setPrompt(formatPrompt("6232E3", "It's your turn!"));
    emit game->setInfo(game->GameInfo);
    // Start timer
    game->timer = new QTimer();
    connect(game->timer, &QTimer::timeout, game, &Game::turnTimeout);
    game->timer->start(1000);

}

void GameLogic::turnTimeout(Game *game) {
    game->GameInfo.LeftTime -= 1;
    emit game->setInfo(game->GameInfo);

    if (game->GameInfo.LeftTime == 0) {
        game->GameInfo.TimeOut -= 1;
        emit game->setInfo(game->GameInfo);

        if (game->GameInfo.TimeOut == 0) {            
            clickBoard(game, -1);
            winGame(game, false);
        } else {
            clickBoard(game, -1);
            endTurn(game);
        }
    }
}

void GameLogic::turnTimeoutOpponent(Game *game) {
    if (game->GameInfo.LeftTime > 0) {
        game->GameInfo.LeftTime -= 1;
        emit game->setInfo(game->GameInfo);
    }
}

void GameLogic::endTurn(Game *game) {
    game->status = PLAYING_WAITING;

    // End timer
    if (game->timer) {
        game->timer->stop();
        delete game->timer;
        game->timer = nullptr;
    }

    // If color was not decided, judge.
    QString sendArgs = "";
    if(game->color == None && game->revealedHistory.length() > 1) {
        // Look up revealed history
        int l = game->revealedHistory.length();
        if(game->revealedHistory[l-1] == game->revealedHistory[l-2]) {
            game->color = game->revealedHistory[l-1];
            game->GameInfo.ColorMe = formatPrompt(game->color == Blue ? "0000ff" : "ff0000", "■");
            sendArgs += game->color == Blue ? " Red" : " Blue";
        }
    }

    // Judge if JunQi can be eaten
    // 45 47 49 Red DiLei
    // 46 48 50 Blue DiLei
    if(game->color == Blue){
        if (game->pieces[45]->eaten &&  game->pieces[47]->eaten && game->pieces[49]->eaten) {
            game->gameState[0] = true;
        }
    } else if (game->color == Red) {
        if (game->pieces[46]->eaten &&  game->pieces[48]->eaten && game->pieces[50]->eaten) {
            game->gameState[0] = true;
        }
    }

    // Judge win and lose: JunQi was eaten; no chesspieces to go
    // 1 Red JunQi
    // 2 Blue JunQi
    if (game->pieces[1]->eaten) {winGame(game, game->color == Blue); return;}
    if (game->pieces[2]->eaten) {winGame(game, game->color == Red); return;}
    // todo: No chesspieces to go
    if(game->color != None) {
        bool cannotMove = true;
        for(int i = 1; i <= 50; ++i) {
            if (game->pieces[i]->revealed == false) {cannotMove = false; break;}
        }
        if (cannotMove) {

            // self
            for(int i = 0; i < 60; ++i) {
                if(GenerateRecordForPosition(game, i) > 0){
                    cannotMove = false; break;
                }
            }
            GenerateRecordForPosition(game, -1);
            // if self cannotMove then enemy wins
            if(cannotMove) {winGame(game, false); return;}

            cannotMove = true;
            // enemy
            game->color = game->color == Blue? Red: Blue;
            for(int i = 0; i < 60; ++i) {
                if(GenerateRecordForPosition(game, i) > 0){
                    cannotMove = false; break;
                }
            }
            game->color = game->color == Blue? Red: Blue;
            GenerateRecordForPosition(game, -1);
            // if enemy cannot move then self wins
            if(cannotMove) {winGame(game, true); return;}
        }

    }

    // If game can continue !!
    // Reset game info
    game->GameInfo.TurnCount += 1;
    game->GameInfo.ColorNow = game->GameInfo.ColorOpponent;
    game->GameInfo.LeftTime = 20;
    emit game->setPrompt(formatPrompt("921855", "It's your opponent's turn!"));
    emit game->setInfo(game->GameInfo);
    if (game->GameInfo.TurnCount == 21) emit game->enableAdmitDefeatButton();

    // Sending signals through socket
    emit game->writeData("400" + sendArgs);

    // New timer
    game->timer = new QTimer();
    connect(game->timer, &QTimer::timeout, game, &Game::turnTimeoutOpponent);
    game->timer->start(1000);

}

void GameLogic::winGame(Game *game, bool isMe) {
    if (game->getStatus() == END) return;
    // Kill timer
    if (game->timer) {
        game->timer->stop();
        delete game->timer;
        game->timer = nullptr;
    }

    QString args = "";

    if (isMe) args += " 0";
    else {args += " 1";}


    emit game->writeData("401" + args);
    emit game->setPrompt(formatPrompt("000000", isMe ? "You Win!" : "You Lose!"));

    game->status = END;

    EndGame* eg;
    if(isMe) {
        eg = new EndGame("You Win!");
    } else {
        eg = new EndGame("You Lose!");
    }
    eg->setWindowTitle("Game Ended!");
    eg->show();
}
