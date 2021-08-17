#include "createconnectionwindow.h"
#include "ui_createconnectionwindow.h"
#include <QtNetwork>

CreateConnectionWindow::CreateConnectionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateConnectionWindow)
{
    ui->setupUi(this);
}

CreateConnectionWindow::~CreateConnectionWindow()
{
    delete ui;
}

void CreateConnectionWindow::on_pushButton_2_clicked()
{
    this->close();
}

void CreateConnectionWindow::on_pushButton_clicked()
{
    emit startHost(ui->ip_addr->text().toStdString());
    this->close();
}

