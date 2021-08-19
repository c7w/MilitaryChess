#include "endgame.h"
#include "ui_endgame.h"

EndGame::EndGame(QString message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EndGame)
{
    ui->setupUi(this);
    ui->Prompt->setText(message);
}

EndGame::~EndGame()
{
    delete ui;
}
