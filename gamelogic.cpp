#include "gamelogic.h"
#include <iostream>
#include <sstream>

GameLogic::GameLogic(QObject *parent) : QObject(parent)
{

}

QString formatPrompt(QString color, QString message) {
    return "<p style=\"color: #"+ color +"\">" + message + "</p>";
}

int GameLogic::OperationRecord = {-1,};
GameLogic::AR GameLogic::AffectedRecord = {{}, {}, {}};

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

    // 200: set GAME.STATUS to READY, resend a 200 if changed
    if(args[0] == "200") {
        if(game->getStatus() == HOSTING) {
            // TODO: Create chessboard
            game->initChessboard();
            game->initIcon();
            // TODO: Send chessboard

            emit game->setPrompt(formatPrompt("872657", "Click 'Play' to get ready!"));
        } else if (game->getStatus() == CONNECTING) {
            emit game->setPrompt(formatPrompt("872657", "Click 'Play' to get ready!"));
            emit game->writeData("200");
        }
        game->status = WAIT_PLAY_CONFIRMATION;
    }
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
        if ( game->pieces[initId]->canEat(game->pieces[toId]) == 1) { AffectedRecord.Eatable.insert(next); }
        if ( game->pieces[initId]->canEat(game->pieces[toId]) == 2) { AffectedRecord.EatableWhileSelfDestroyed.insert(next); }
    }
}

void GameLogic::GenerateRecordForPosition(Game *game, int pos) {
    AffectedRecord.Approachable.clear();
    AffectedRecord.Eatable.clear();
    AffectedRecord.EatableWhileSelfDestroyed.clear();

    if (pos == -1) {
        return;
    }
    qDebug() << "GenerateRecordForPosition " << pos;
    // First retrieve the ChessPiece of the position
    int initID = game->board[pos];
    if ( initID == 0 ) { return; }

    ArmType arm = game->pieces[initID]->getArmType();
    if ( arm == JunQi || arm == DiLei) return;

    int delta[] = {5,6,1,-4,-5,-6,-1,4};
    for(auto d : delta) {
        int hasEdge = Constants::hasEdge(pos, pos+d);
        if (hasEdge == 1) {
            int toId = game->board[pos+d];
            if (toId == 0) {AffectedRecord.Approachable.insert(pos+d); continue;}
            if (! game->pieces[toId]->revealed) { continue; }
            if ( game->pieces[initID]->canEat(game->pieces[toId]) == 1) { AffectedRecord.Eatable.insert(pos+d); continue; }
            if ( game->pieces[initID]->canEat(game->pieces[toId]) == 2) { AffectedRecord.EatableWhileSelfDestroyed.insert(pos+d); continue; }
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
                    if ( game->pieces[initID]->canEat(game->pieces[toId]) == 1) { AffectedRecord.Eatable.insert(target+d); continue; }
                    if ( game->pieces[initID]->canEat(game->pieces[toId]) == 2) { AffectedRecord.EatableWhileSelfDestroyed.insert(target+d); continue; }
                    continue;
                }
            } else {
                DFS(game, initID, pos+d);
            }

        }
    }

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
    qDebug() << "Last: " << OperationRecord << " This: " << pos ;

    bool related = false; // Whether pass this turn

    // Reveal Unknown
    if (pos > 0 && game->board[pos] != 0 && !game->pieces[game->board[pos]]->revealed) {
        game->pieces[game->board[pos]]->setRevealed();
        qDebug() << "SetRevealed " << pos;
        game->updateIcon(pos);
        related = true;
    }

    if (!related) {
        // Test if pos falls in AffectedRecord
        int x;
        foreach(x, AffectedRecord.Approachable) {
            if ( pos == x ) {
                MovePiece(game, OperationRecord, pos);
                related = true;
            }
        }
        foreach(x, AffectedRecord.Eatable) {
            if ( pos == x ) {
                EatPiece(game, OperationRecord, pos);
                related = true;
            }
        }
        foreach(x, AffectedRecord.EatableWhileSelfDestroyed) {
            if( pos == x ){
                EatPieceWhileSelfDestroyed(game, OperationRecord, pos);
                related = true;
            }
        }
    }

    // Clean styles
    if(OperationRecord >= 0) {
        game->icons->at(OperationRecord)->setStyleSheet("");
        int x;
        foreach(x , qAsConst(AffectedRecord.Approachable)) game->icons->at(x)->setStyleSheet("");
        foreach(x , AffectedRecord.Eatable) game->icons->at(x)->setStyleSheet("");
        foreach(x , AffectedRecord.EatableWhileSelfDestroyed) game->icons->at(x)->setStyleSheet("");
    }


    // Generate new related positions
    if (related) GenerateRecordForPosition(game, -1);
    else GenerateRecordForPosition(game, pos);

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
    if (related) {
        // Pass turn
    }


}
