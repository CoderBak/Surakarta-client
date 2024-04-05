#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), socket(new QTcpSocket(this)) {
    ui->setupUi(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readData);
    socket->connectToHost("localhost", 1234);
    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect to host.";
        exit(1);
    } else {
        qDebug() << "Connected successfully!";
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    QString number = ui->lineEdit->text();
    qDebug() << number;
    socket->write(number.toUtf8());
}

void MainWindow::readData() {
    QByteArray data = socket->readAll();
    ui->label->setText(data);
}
