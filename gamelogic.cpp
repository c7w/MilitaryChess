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

void GameLogic::GenerateRecordForPosition(Game *game, int pos) {
    AffectedRecord.Approachable.clear();
    AffectedRecord.Eatable.clear();
    AffectedRecord.EatableWhileSelfDestroyed.clear();

    if (pos == -1) {
        return;
    }
    // First retrieve the ChessPiece of the position
    int initID = game->board[pos];
    if ( initID == 0 ) { return; }



}

void GameLogic::MovePiece(Game *game, int from, int to) {
    game->board[to] = game->board[from];
    game->board[from] = 0;
    game->updateIcon(from);
    game->updateIcon(to);
}

void GameLogic::EatPiece(Game *game, int from, int to) {
    int toId = game->board[to];
    game->pieces[toId]->setEaten();

    game->board[to] = game->board[from];
    game->board[from] = 0;
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
        game->updateIcon(pos);
        related = true;
    }

    if (!related) {
        // Test if pos falls in AffectedRecord
        for(auto x : AffectedRecord.Approachable) {
            if ( pos == x ) {
                MovePiece(game, OperationRecord, pos);
                related = true;
            }
        }
        for(auto x : AffectedRecord.Eatable) {
            if ( pos == x ) {
                EatPiece(game, OperationRecord, pos);
                related = true;
            }
        }
        for(auto x : AffectedRecord.EatableWhileSelfDestroyed) {
            if( pos == x ){
                EatPieceWhileSelfDestroyed(game, OperationRecord, pos);
                related = true;
            }
        }
    }

    // Clean styles
    if(OperationRecord > 0) {
        game->icons->at(OperationRecord)->setStyleSheet("");
        for(auto x : AffectedRecord.Approachable) game->icons->at(x)->setStyleSheet("");
        for(auto x : AffectedRecord.Eatable) game->icons->at(x)->setStyleSheet("");
        for(auto x : AffectedRecord.EatableWhileSelfDestroyed) game->icons->at(x)->setStyleSheet("");
    }


    // Generate new related positions
    if (related) GenerateRecordForPosition(game, -1);
    else GenerateRecordForPosition(game, pos);

    // Make styles
    if (pos > 0) {
        if (related) game->icons->at(pos)->setStyleSheet("border-width: 2px; border-style: outset solid; border-color: yellow; border-radius: 5px;");
        else game->icons->at(pos)->setStyleSheet("border-width: 2px; border-style: outset solid; border-color: red; border-radius: 5px;");

        for(auto x : AffectedRecord.Approachable) game->icons->at(x)->setStyleSheet("border-width: 2px; border-style: outset solid; border-color: green; border-radius: 5px;");
        for(auto x : AffectedRecord.Eatable) game->icons->at(x)->setStyleSheet("border-width: 2px; border-style: outset solid; border-color: blue; border-radius: 5px;");
        for(auto x : AffectedRecord.EatableWhileSelfDestroyed) game->icons->at(x)->setStyleSheet("border-width: 2px; border-style: outset solid; border-color: #FF99FF; border-radius: 5px;");
    }



    OperationRecord = pos;
    if (related) {
        // Pass turn
    }


}
