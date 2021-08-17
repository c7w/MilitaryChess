#include "createconnectionwindow.h"
#include "ui_createconnectionwindow.h"
#include <QtNetwork>

CreateConnectionWindow::CreateConnectionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateConnectionWindow)
{
    ui->setupUi(this);

    // Fetch local IP addresses
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    for (auto& hostAddr : info.addresses()) {
        if (hostAddr.protocol() == QAbstractSocket::IPv4Protocol) {
            ui->ip_addr->addItem(hostAddr.toString());
        }
    }

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
    emit startHost(ui->ip_addr->currentText());
    this->close();
}

