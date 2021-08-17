#include "gamelogic.h"
#include <iostream>
#include <sstream>

GameLogic::GameLogic(QObject *parent) : QObject(parent)
{

}


QString formatPrompt(QString color, QString message) {
    return "<p style=\"color: #"+ color +"\">" + message + "</p>";
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

    // 200: set GAME.STATUS to READY, resend a 200 if changed
    if(args[0] == "200") {
        if(game->getStatus() == HOSTING) {
            // TODO: Create chessboard

            // TODO: Send chessboard

            emit game->setPrompt(formatPrompt("872657", "Click 'Play' to get ready!"));
        } else if (game->getStatus() == CONNECTING) {
           emit game->setPrompt(formatPrompt("872657", "Click 'Play' to get ready!"));
            game->writeData("200");
        }
        game->status = WAIT_PLAY_CONFIRMATION;
    }
}
