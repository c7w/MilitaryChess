#include "game.h"

Game::Game()
    : status(OFFLINE)
{

}

void Game::startConnection(QString role) {
    connection = new GameConnection("0.0.0.0");
    connection->moveToThread(&connectionThread);
    if (role=="Server") {
        status = HOSTING;
        connection->startServer();
    } else {
        status = CONNECTING;
        connection->startClient();
    }
}

Game::~Game() {
    connectionThread.quit();
    connectionThread.wait();
}
