#include "game.h"

Game::Game()
    : status(OFFLINE)
{

}

void Game::startConnection(QString role) {
    connection = new GameConnection("0.0.0.0");
    connection->moveToThread(&connectionThread);

    // Generate Chess
    QImage img(":/assets/image/chess_unknown.png");
    for (int i = 0; i < 60; ++i) {
        qDebug() << i << " " << icons->at(i) << "\n";
        icons->at(i)->setPixmap(QPixmap::fromImage(img.scaled(icons->at(i)->size(), Qt::KeepAspectRatio)));
    }

    // TODO: Multi-thread
    if (role=="Server") {
        status = HOSTING;
        // connection->startServer();
    } else {
        status = CONNECTING;
        // connection->startClient();
    }
}

Game::~Game() {
    connectionThread.quit();
    connectionThread.wait();
    delete icons;
}
