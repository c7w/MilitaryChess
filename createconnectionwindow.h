#ifndef CREATECONNECTIONWINDOW_H
#define CREATECONNECTIONWINDOW_H

#include <QDialog>

namespace Ui {
class CreateConnectionWindow;
}

class CreateConnectionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateConnectionWindow(QWidget *parent = nullptr);
    ~CreateConnectionWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

signals:
    void startHost(QString IP_ADDR);

private:
    Ui::CreateConnectionWindow *ui;
};

#endif // CREATECONNECTIONWINDOW_H
