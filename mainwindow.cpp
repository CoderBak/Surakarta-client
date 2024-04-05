#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        socket(new QTcpSocket(this))
{
    ui->setupUi(this);
    //connect(socket, &QTcpSocket::readyRead, this, &MainWindow::sendData);
    socket->connectToHost("localhost", 1234);
    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect to host.";
        exit(-1);
    } else {
        qDebug() << "Connected successfully!";
    }
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::sendData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendData() {
    QByteArray data;
    data.append(ui->lineEdit1->text().toUtf8());
    data.append(";");
    data.append(ui->lineEdit2->text().toUtf8());
    socket->write(data);
}
