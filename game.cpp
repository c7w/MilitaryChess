#include "game.h"

Game::Game()
    : status(OFFLINE)
{

}

void Game::startConnection(QString role, QString ip_addr) {
    connection = new GameConnection(role, ip_addr);
    connection->moveToThread(&connectionThread);

    // Generate Chess
    QImage img(":/assets/image/chess_unknown.png");
    for (int i = 0; i < 60; ++i) {
        icons->at(i)->setPixmap(QPixmap::fromImage(img.scaled(icons->at(i)->size(), Qt::KeepAspectRatio)));
    }

    // Establish multi-thread connection
    connect(this, &Game::initConnection, connection, &GameConnection::start);
    connect(connection, &GameConnection::MessageToGame, this, &Game::getData);
    connect(this, &Game::writeData, connection, &GameConnection::GameToMessage);

    // Create the connection
    emit initConnection();

    // TODO: Multi-thread
    if (role=="Server") {
        status = HOSTING;
    } else {
        status = CONNECTING;
    }
}

Game::~Game() {
    connectionThread.quit();
    connectionThread.wait();
    delete icons;
}

void Game::getData(const QString & data) {
    // TODO Parse Data
    qDebug() << data;
}
