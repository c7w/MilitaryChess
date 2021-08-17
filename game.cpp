#include "game.h"

Game::Game()
    : status(OFFLINE)
{

}

void Game::startConnection(QString role, QString ip_addr) {
    connection = new GameConnection(role, ip_addr);
    connection->moveToThread(&connectionThread);

    // Generate Chess // Remove later
    QImage img(":/assets/image/chess_unknown.png");
    for (int i = 0; i < 60; ++i) {
        icons->at(i)->setPixmap(QPixmap::fromImage(img.scaled(icons->at(i)->size(), Qt::KeepAspectRatio)));
    }

    // Establish multi-thread connection
    connect(this, &Game::initConnection, connection, &GameConnection::start);
    connect(connection, &GameConnection::MessageToGame, this, &Game::getData);
    connect(this, &Game::writeData, connection, &GameConnection::GameToMessage);
    connectionThread.start();

    // Create the connection
    emit initConnection();
    // Change game status
    if (role=="Server") {
        status = HOSTING;
    } else {
        status = CONNECTING;
    }
}

bool Game::cancelConnection() {
    if(status == HOSTING || status == CONNECTING || status == READY) {
        connection->deleteLater();
        connectionThread.quit();
        connectionThread.wait();
        status = OFFLINE;
        return true;
    }
    return false;

}

Game::~Game() {
    connectionThread.quit();
    connectionThread.wait();
    delete icons;
}

void Game::getData(const QString& data) {
    GameLogic::MessageProcess(this, data);
}
