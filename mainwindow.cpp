#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createconnectionwindow.h"
#include "connectserverwindow.h"
#include "game.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Render UI

    ui->setupUi(this);

    QImage img(":/assets/image/chessboard_background.jpg");
    ui->Map->setPixmap(QPixmap::fromImage(img).scaled(ui->Map->size(), Qt::KeepAspectRatio));

    QImage unknown(":/assets/image/chess_unknown.png");
    ui->icon_0->setPixmap(QPixmap::fromImage(unknown).scaled(ui->icon_0->size(), Qt::KeepAspectRatio));

    this->game = new Game();




}

MainWindow::~MainWindow()
{
    delete ui;
    delete game;
}

void MainWindow::on_actionConnect_to_server_triggered()
{
    delete childWindow;
    ConnectServerWindow* ccw = new ConnectServerWindow(this);
    ccw->setWindowTitle("Connect to Host");
    ccw->show();

    childWindow = ccw;

}


void MainWindow::on_actionCreate_the_connection_triggered()
{
    delete childWindow;
    CreateConnectionWindow* ccw = new CreateConnectionWindow(this);
    ccw->setWindowTitle("Create Host");
    ccw->show();

    childWindow = ccw;
    connect(ccw, &CreateConnectionWindow::startHost, this, &MainWindow::on_start_host);
}

void MainWindow::on_start_host(std::string ip_addr) {
    // Set status
    // role = "Server";
    // server = new QTcpServer();
    // server->listen( ip_addr, 23333);
    // Render UI
    // map = new QPixmap("/assets/images/chessboard.png");
    // ui->Map->setPixmap(*map);
    QImage img(":/assets/image/chessboard.png");

    ui->Map->setPixmap(QPixmap::fromImage(img).scaled(ui->Map->size(), Qt::KeepAspectRatio));


}
