#ifndef CONNECTSERVERWINDOW_H
#define CONNECTSERVERWINDOW_H

#include <QDialog>

namespace Ui {
class ConnectServerWindow;
}

class ConnectServerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectServerWindow(QWidget *parent = nullptr);
    ~ConnectServerWindow();

signals:
    void BtnPressed(QString n);
    void startConnect(QString ip_addr);

public slots:
    void IpAppend(QString n);

private slots:
    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::ConnectServerWindow *ui;
};

#endif // CONNECTSERVERWINDOW_H
