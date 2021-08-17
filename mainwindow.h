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

signals:
    void pressedPiece(int pos);

private slots:
    void on_actionConnect_to_server_triggered();

    void on_actionCreate_the_connection_triggered();

    void on_start_host(std::string ip_addr);



private:
    Ui::MainWindow *ui;
    QDialog* childWindow = nullptr;
    QVector<QLabel*>* icons;

protected:
    Game* game;
    bool eventFilter(QObject *obj, QEvent *event);
};
#endif // MAINWINDOW_H
