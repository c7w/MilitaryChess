#include "connectserverwindow.h"
#include "ui_connectserverwindow.h"
#include <regex>

ConnectServerWindow::ConnectServerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectServerWindow)
{
    ui->setupUi(this);
    connect(ui->p0, &QPushButton::clicked, [=](){emit BtnPressed("0");});
    connect(ui->pushButton_4, &QPushButton::clicked, [=](){emit BtnPressed("1");});
    connect(ui->pushButton_5, &QPushButton::clicked, [=](){emit BtnPressed("2");});
    connect(ui->pushButton_6, &QPushButton::clicked, [=](){emit BtnPressed("3");});
    connect(ui->pushButton_7, &QPushButton::clicked, [=](){emit BtnPressed("4");});
    connect(ui->pushButton_8, &QPushButton::clicked, [=](){emit BtnPressed("5");});
    connect(ui->pushButton_9, &QPushButton::clicked, [=](){emit BtnPressed("6");});
    connect(ui->pushButton_10, &QPushButton::clicked, [=](){emit BtnPressed("7");});
    connect(ui->pushButton_11, &QPushButton::clicked, [=](){emit BtnPressed("8");});
    connect(ui->pushButton_12, &QPushButton::clicked, [=](){emit BtnPressed("9");});
    connect(ui->pushButton_3, &QPushButton::clicked, [=](){emit BtnPressed(".");});
    connect(ui->pushButton_2, &QPushButton::clicked, [=](){emit BtnPressed("Del");});

    connect(this, &ConnectServerWindow::BtnPressed, this, &ConnectServerWindow::IpAppend);
}

ConnectServerWindow::~ConnectServerWindow()
{
    delete ui;
}

void ConnectServerWindow::IpAppend(QString n) {
    if (n == "Del") {
        const QString curr = ui->ip_addr->text();
        const int length = curr.length();
        if (length == 0) return;
        QString newS = "";
        for(int i = 0; i < length-1; ++i) {
            newS += curr[i];
        }
        ui->ip_addr->setText(newS);

    } else {
        ui->ip_addr->setText((ui->ip_addr->text()) + n);
    }

}



void ConnectServerWindow::on_pushButton_13_clicked()
{
    this->close();
}


void ConnectServerWindow::on_pushButton_14_clicked()
{
    // First check if typed in an IP Address
    std::regex re = std::regex("^((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])$");
    const bool match = std::regex_match(ui->ip_addr->text().toStdString(), re);
    // Reference: Regex Expression taken from https://tool.chinaz.com/regex


    if (match) {
        // TODO: Socket
        emit startConnect(ui->ip_addr->text());

        this->close();
    } else {
        // Prompt if not an ip address
        ui->prompt->setText("Please type in a right IP address!");
    }
}

