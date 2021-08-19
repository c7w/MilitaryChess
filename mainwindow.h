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
    void pressedBoard(int pos);
    void playerGetReady();
    void playerAdmitDefeat();

private slots:
    void on_actionConnect_to_server_triggered();

    void on_actionCreate_the_connection_triggered();

    void on_start_host(QString ip_addr);
    void on_start_connection(QString ip_addr);


    void on_actionCancel_the_connection_triggered();

    void setPrompt(const QString& message);

    void on_actionStart_triggered();

    void on_actionAdmit_Defeat_triggered();

private:
    Ui::MainWindow *ui;
    QDialog* childWindow = nullptr;
    QVector<QLabel*>* icons;
    Game* game;
    bool eventFilter(QObject *obj, QEvent *event);
    void initGame();
    void clearGame();
};
#endif // MAINWINDOW_H
