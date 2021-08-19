#include "game.h"
#include <random>

Game::Game()
    : status(OFFLINE)
{
    Constants::GenerateEdge();
}

void Game::startConnection(QString role, QString ip_addr) {
    connection = new GameConnection(role, ip_addr);
    connection->moveToThread(&connectionThread);

    // Establish multi-thread connection
    connect(this, &Game::initConnection, connection, &GameConnection::start);
    connect(connection, &GameConnection::MessageToGame, this, &Game::getData);
    connect(this, &Game::writeData, connection, &GameConnection::GameToMessage);
    connectionThread.start();

    // Create the connection
    emit initConnection();
    // Change game status
    this->role = role;
    if (role=="Server") {
        status = HOSTING;
    } else {
        status = CONNECTING;
    }
}

void Game::cancelConnection() {
    connectionThread.quit();
    connectionThread.wait();
    if (connection) {
        delete connection;
        connection = nullptr;
    }
    status = OFFLINE;

    color = None;
    gameState = {false, false};
    readyState = {false, false};
    board.clear();
    for (auto x : pieces) delete x;
    for (auto x : *icons) x->setPixmap(QPixmap::fromImage(Constants::getImage(Empty).scaled(x->size(), Qt::KeepAspectRatio)));
    pieces = {nullptr,}; // [1, 50], takes control of 50 chess pieces in initID order.

    if(timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    revealedHistory.clear();
}

Game::~Game() {
    cancelConnection();
    delete icons;

    for (auto x : this->pieces) delete x;
}

void Game::onGetReady() {GameLogic::onGetReady(this);}
void Game::onAdmitDefeat() { GameLogic::winGame(this, false); }

void Game::getData(const QString& data) {
    GameLogic::MessageProcess(this, data);
}

void Game::setIcon(int pos, EnumChessPiece piece) {
    this->icons->at(pos)->setPixmap(QPixmap::fromImage(Constants::getImage(piece).scaled(this->icons->at(pos)->size(), Qt::KeepAspectRatio)));
}

void Game::initIcon() {
    for(int i = 0; i < 60; ++i) {
        updateIcon(i);
    }
}

void Game::updateIcon(int pos) {
    int pieceInitID = this->board[pos];
    if(pieceInitID == 0) {
        this->setIcon(pos, Empty);
    } else {
        this->setIcon(pos, this->pieces[pieceInitID]->getEnumChessPieceIcon());
    }
}

// Generate 50 new Chess pieces and shuffle chess pieces across chess board;
void Game::initChessboard() {
    // Generate
    for (int i = 1; i <= 50; ++i) {
        this->pieces.push_back(ChessPiece::getChesePiece(i));
    }

    // Shuffle, using std::random_shuffle
    std::vector<int> tempVector;
    for(int i = 1; i <=50; ++i) tempVector.push_back(i);
    srand((unsigned)time(NULL));
    std::random_shuffle(tempVector.begin(), tempVector.end());

    // Fill the shuffled result into the board
    int tempPos = 0;
    for(int i = 0; i < 60; ++i) {
        bool notSkip = true;
        for(auto x: Constants::Camp) {
            if(i == x || i == x+25) {
                // Just leave it blank!
                this->board.push_back(0);
                notSkip = false;
                break;
            }
        }

        if (notSkip) {
            this->board.push_back(tempVector[tempPos]);
            tempPos++;
        }
    }
}

// Slots function for onPressedBoard
void Game::onPressedBoard(int pos) {
    GameLogic::clickBoard(this, pos);
}

void Game::turnTimeout() {
    GameLogic::turnTimeout(this);
}

void Game::turnTimeoutOpponent() {
    GameLogic::turnTimeoutOpponent(this);
}
