#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionConnect_to_server_triggered();

    void on_actionCreate_the_connection_triggered();

    void on_start_host(std::string ip_addr);

private:
    Ui::MainWindow *ui;
    QDialog* childWindow = nullptr;


protected:
    Game* game;
};
#endif // MAINWINDOW_H
