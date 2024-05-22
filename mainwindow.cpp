#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startmenu.h"
#include <Qpainter>
#include <QMouseEvent>
#include <iostream>
#include <QTextStream>
#include <QString>
#include <QCursor>
#include <QMenuBar>
#include <QStatusBar>
#include <QThread>
#include <QDir>
#include <QFile>

void MainWindow::handleAIQuery(bool isAI) {
    if (isAI) {
        socket->write("$A1");
    } else {
        socket->write("$A0");
    }
}

// Send the pressed information.
void MainWindow::handleMoveInfo(const QByteArray &moveInfo) {
    socket->write(moveInfo);
}

void MainWindow::handleEatableQuery(const posType &pos) {
    QByteArray query = QString("$QC%1;%2").arg(pos.first).arg(pos.second).toUtf8();
    socket->write(query);
}

void MainWindow::handleMovableQuery(const posType &pos) {
    QByteArray query = QString("$QN%1;%2").arg(pos.first).arg(pos.second).toUtf8();
    socket->write(query);
}

void MainWindow::handleCheckBoxStateChanged(int state) {
    bool enable = (state == Qt::Checked);
    emit aiControlChanged(enable);
}

void MainWindow::receiveBoardSizeFromSettings(int size) {
    board->receiveDataFromUser(size, 0);
}

void MainWindow::receivePieceColorFromSettings(char color) {
    board->receiveDataFromUser(0, color);
}

void MainWindow::receiveDirFromSettings(QString dir) {
    qDebug() << "dir = " << dir;
    savePlace = dir;
}

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        socket(new QTcpSocket(this)),
        board(new QtBoard(this)) {
    ui->setupUi(this);

    // Set timer
    titleTotal = new QLabel("Total time", this);
    titleReset = new QLabel("Current Time", this);
    titleTotal->move(10, 30);
    titleReset->move(10, 70);
    labelTotal = new QLabel("00::00::00", this);
    labelTotal->move(10, 50);
    labelReset = new QLabel("00::00::00", this);
    labelReset->move(10, 90);

    // Set the layout of the interface.
    auto *layout = new QHBoxLayout;
    auto *menu = new QVBoxLayout;

    // Add buttons to the menu.
    menu->addWidget(ui->tryAgainButton);
    menu->addWidget(ui->giveUpButton);
    menu->addWidget(ui->openChatroomButton);
    menu->addWidget(ui->aiHandle);
    // // Set the layout.
    layout->addLayout(menu);
    layout->addWidget(board);

    // Set the window size.
    auto *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    centralWidget->setFixedSize(WIDTH, HEIGHT);
    setCentralWidget(centralWidget);
    // setStyleSheet(STYLE);

    // Connect the button to functions.
    // connect(startMenu, &StartMenu::startGame,this,&MainWindow::startGame);
    // qDebug()<<" run before connect";
    // connect(setting, &Settings::settingsApplied, board, &QtBoard::receiveDataFromUser);

    connect(ui->aiHandle, &QCheckBox::stateChanged, this, &MainWindow::handleCheckBoxStateChanged);
    connect(this, &MainWindow::aiControlChanged, board, &QtBoard::setHandledByAI);
    connect(board, &QtBoard::sendMovableQuery, this, &MainWindow::handleMovableQuery);
    connect(board, &QtBoard::sendEatableQuery, this, &MainWindow::handleEatableQuery);
    connect(board, &QtBoard::sendMoveInfo, this, &MainWindow::handleMoveInfo);
    connect(board, &QtBoard::sendAIQuery, this, &MainWindow::handleAIQuery);
    connect(this, &MainWindow::sendEatable, board, &QtBoard::handleEatable);
    connect(this, &MainWindow::sendMovable, board, &QtBoard::handleMovable);
    connect(ui->tryAgainButton, &QPushButton::clicked, this, &MainWindow::sendTryAgain);
    // connect(giveUpButton, &QPushButton::clicked, this, &MainWindow::handleGiveUp);
    // connect(openChatroomButton, &QPushButton::clicked, this, &MainWindow::handleOpenChatroom);

    // Connect the server.
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::getData);
    //connect(socket, &QTcpSocket::readyRead, this, &MainWindow::getTimeData);

    //connect(startMenu->startButton,&QPushButton::clicked,this,&MainWindow::startNow);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::startNow() {
    socket->connectToHost("localhost", PORT);
    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect to remote host, try to connect localhost";
        socket->connectToHost("localhost", PORT);
        if (!socket->waitForConnected()) {
            qDebug() << "Failed to connect to localhost, abort!";
            exit(-1);
        }
    } else {
        qDebug() << "Remote host connected successfully!";
    }
}

void MainWindow::sendTryAgain() {
    socket->write("$G;");
}

// Process data in this function.
void MainWindow::getData() {
    QByteArray data = socket->readAll();
    // qDebug() << "Received message from server: " << data;
    if (data[0] != '$') {
        qDebug() << "Wrong format!";
    } else {
        while (true) {
            const auto endPos = data.indexOf('$', 1);
            if (endPos != -1) {
                dataHandler(data.mid(1, endPos - 1));
                data.remove(0, endPos);
            } else {
                data.remove(0, 1);
                dataHandler(data);
                break;
            }
        }
    }
    socket->read(socket->bytesAvailable());
}

void MainWindow::dataHandler(const QByteArray &info) {
    if (info[0] != 'T') {
        qDebug() << "Processing info:" << info;
    }
    switch (info[0]) {
        case 'S': {
            qDebug() << "I'm player" << info[1];
            switch (info[1]) {
                case 'B':
                    board->setCurrentPlayer(BLACK);
                    break;
                case 'W':
                    board->setCurrentPlayer(WHITE);
                    break;
                default:
                    board->setCurrentPlayer(NONE);
            }
            break;
        }
        case 'R': {
            QByteArray toSend = info.mid(2, info.length());
            if (info[1] == 'C') {
                emit sendEatable(toSend);
            } else {
                emit sendMovable(toSend);
            }
            break;
        }
        case 'T': {
            QByteArray timeData = info.mid(5).trimmed(); // Remove "$TIME:" prefix
            if (timeData[0] == 'R') {
                //qDebug()<<timeData;
                timeData = timeData.mid(1).trimmed();
                QString timeString = QString::fromUtf8(timeData);
                labelReset->setText(timeString);
            } else if (timeData[0] == 'T') {
                //qDebug()<<timeData;
                timeData = timeData.mid(1).trimmed();
                QString timeString = QString::fromUtf8(timeData);
                labelTotal->setText(timeString);
            }
            break;
        }
        case 'E': {
            if (info[1] == 'T') {
                qDebug() << "Time out!";
            } else if (info[1] == 'F') {
                qDebug() << "Opponent time out";
            }
            break;
        }
        case 'B': {
            board->processBoardInfo(info.mid(1));
            break;
        }
        case 'L': {
            QString title = QString::fromUtf8(info.mid(1, 17));
            QString log = QString::fromUtf8(info.mid(18));
            qDebug() << "Received title" << title;
            qDebug() << "Received log" << log;
            QDir dir;
            if (!dir.exists(savePlace)) {
                dir.mkpath(savePlace);
            }
            qDebug() << "Trying to save at " << savePlace + "/" + title + ".log";
            QFile file(savePlace + "/" + title + ".log");
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream stream(&file);
                stream << log;
                qDebug() << "Log saved";
            }
        }
        default:
            return;
    }
}
