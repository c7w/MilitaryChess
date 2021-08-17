#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createconnectionwindow.h"
#include "connectserverwindow.h"
#include "game.h"
#include <QtDebug>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage img(":/assets/image/chessboard_background.jpg");
    ui->Map->setPixmap(QPixmap::fromImage(img).scaled(ui->Map->size(), Qt::KeepAspectRatio));
    ui->Map->raise();

    this->game = new Game();

    // Store icons into the Vector
    icons = new QVector<QLabel*>();
    icons->push_back(ui->icon_0);icons->push_back(ui->icon_1);icons->push_back(ui->icon_2);icons->push_back(ui->icon_3);icons->push_back(ui->icon_4);icons->push_back(ui->icon_5);icons->push_back(ui->icon_6);icons->push_back(ui->icon_7);icons->push_back(ui->icon_8);icons->push_back(ui->icon_9);icons->push_back(ui->icon_10);icons->push_back(ui->icon_11);icons->push_back(ui->icon_12);icons->push_back(ui->icon_13);icons->push_back(ui->icon_14);icons->push_back(ui->icon_15);icons->push_back(ui->icon_16);icons->push_back(ui->icon_17);icons->push_back(ui->icon_18);icons->push_back(ui->icon_19);icons->push_back(ui->icon_20);icons->push_back(ui->icon_21);icons->push_back(ui->icon_22);icons->push_back(ui->icon_23);icons->push_back(ui->icon_24);icons->push_back(ui->icon_25);icons->push_back(ui->icon_26);icons->push_back(ui->icon_27);icons->push_back(ui->icon_28);icons->push_back(ui->icon_29);icons->push_back(ui->icon_30);icons->push_back(ui->icon_31);icons->push_back(ui->icon_32);icons->push_back(ui->icon_33);icons->push_back(ui->icon_34);icons->push_back(ui->icon_35);icons->push_back(ui->icon_36);icons->push_back(ui->icon_37);icons->push_back(ui->icon_38);icons->push_back(ui->icon_39);icons->push_back(ui->icon_40);icons->push_back(ui->icon_41);icons->push_back(ui->icon_42);icons->push_back(ui->icon_43);icons->push_back(ui->icon_44);icons->push_back(ui->icon_45);icons->push_back(ui->icon_46);icons->push_back(ui->icon_47);icons->push_back(ui->icon_48);icons->push_back(ui->icon_49);icons->push_back(ui->icon_50);icons->push_back(ui->icon_51);icons->push_back(ui->icon_52);icons->push_back(ui->icon_53);icons->push_back(ui->icon_54);icons->push_back(ui->icon_55);icons->push_back(ui->icon_56);icons->push_back(ui->icon_57);icons->push_back(ui->icon_58);icons->push_back(ui->icon_59);
    this->game->setIcons(icons);

    // Install the event filter
    ui->widget->installEventFilter(this);
    for(int i = 0; i < 60; ++i){
        icons->at(i)->installEventFilter(this);
    }
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
    ui->Map->lower();
    game->startConnection("Server");

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

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->widget) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            qDebug() << "Ate key press" << mouseEvent;
            if (mouseEvent->button() == Qt::LeftButton) emit pressedPiece(-1);
            return true;
        } else {
            return false;
        }
    }

    for(int i = 0; i<60; ++i) {
        if (obj == icons->at(i)) {
            if (event->type() == QEvent::MouseButtonRelease) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                qDebug() << "Ate key press" << mouseEvent;
                if (mouseEvent->button() == Qt::LeftButton) emit pressedPiece(i);
                return true;
            } else {
                return false;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
