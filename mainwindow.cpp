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

// handle ai query
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

// process eatable query
void MainWindow::handleEatableQuery(const posType &pos) {
    QByteArray query = QString("$QC%1;%2").arg(pos.first).arg(pos.second).toUtf8();
    socket->write(query);
}

// process movable query
void MainWindow::handleMovableQuery(const posType &pos) {
    QByteArray query = QString("$QN%1;%2").arg(pos.first).arg(pos.second).toUtf8();
    socket->write(query);
}

// handle checkbox state changed
void MainWindow::handleCheckBoxStateChanged(int state) {
    bool enable = (state == Qt::Checked);
    emit aiControlChanged(enable);
}

// receive boardSize from settings
void MainWindow::receiveBoardSizeFromSettings(int size) {
    board->receiveDataFromUser(size, 0);
}

// receive piece color from settings
void MainWindow::receivePieceColorFromSettings(char color) {
    board->receiveDataFromUser(0, color);
}

// receive dir from settings
void MainWindow::receiveDirFromSettings(QString dir) {
    qDebug() << "dir = " << dir;
    savePlace = dir;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
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
    // Set the layout.
    layout->addLayout(menu);
    layout->addWidget(board);

    // Set the window size.
    auto *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    centralWidget->setFixedSize(WIDTH, HEIGHT);
    setCentralWidget(centralWidget);
    // setStyleSheet(STYLE);

    // functions to connect
    connect(ui->aiHandle, &QCheckBox::stateChanged, this, &MainWindow::handleCheckBoxStateChanged);
    connect(this, &MainWindow::aiControlChanged, board, &QtBoard::setHandledByAI);
    connect(board, &QtBoard::sendMovableQuery, this, &MainWindow::handleMovableQuery);
    connect(board, &QtBoard::sendEatableQuery, this, &MainWindow::handleEatableQuery);
    connect(board, &QtBoard::sendMoveInfo, this, &MainWindow::handleMoveInfo);
    connect(board, &QtBoard::sendAIQuery, this, &MainWindow::handleAIQuery);
    connect(this, &MainWindow::sendEatable, board, &QtBoard::handleEatable);
    connect(this, &MainWindow::sendMovable, board, &QtBoard::handleMovable);
    connect(ui->tryAgainButton, &QPushButton::clicked, this, &MainWindow::sendTryAgain);

    // Connect the server.
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::getData);
}

// destructor
MainWindow::~MainWindow() {
    delete ui;
}

// Start the game.
void MainWindow::startNow() {
    socket->connectToHost(serverIP, PORT); // Here we simply use localhost, feel free to change.
    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect to remote host, abort!";
        exit(-1);
    } else {
        qDebug() << "Remote host connected successfully!";
    }
}

// Send try again message.
void MainWindow::sendTryAgain() {
    socket->write("$G;");
}

// Process data in this function.
void MainWindow::getData() {
    QByteArray data = socket->readAll();

    // judge the format first
    if (data[0] != '$') {
        qDebug() << "Wrong format!";
    } else {
        // process the data
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

    // process the rest of the data
    socket->read(socket->bytesAvailable());
}

void MainWindow::dataHandler(const QByteArray &info) {
    // first judge the type of the info
    if (info[0] != 'T') {
        qDebug() << "Processing info:" << info;
    }
    switch (info[0]) {
        case 'S': {
            qDebug() << "I'm player" << info[1];
            switch (info[1]) {
                // process chess color
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
            // process move or eat
            QByteArray toSend = info.mid(2, info.length());
            if (info[1] == 'C') {
                emit sendEatable(toSend);
            } else {
                emit sendMovable(toSend);
            }
            break;
        }
        case 'T': {
            // process time
            // Remove "$TIME:" prefix
            QByteArray timeData = info.mid(5).trimmed();

            if (timeData[0] == 'R') {
                // for reset timer
                timeData = timeData.mid(1).trimmed();
                QString timeString = QString::fromUtf8(timeData);
                labelReset->setText(timeString);
            } else if (timeData[0] == 'T') {
                // for total timer
                timeData = timeData.mid(1).trimmed();
                QString timeString = QString::fromUtf8(timeData);
                labelTotal->setText(timeString);
            }
            break;
        }
        case 'E': {
            // process timeout
            if (info[1] == 'T') {
                qDebug() << "Time out!";
            } else if (info[1] == 'F') {
                qDebug() << "Opponent time out";
            }
            break;
        }
        case 'B': {
            // process board info
            board->processBoardInfo(info.mid(1));
            break;
        }
        case 'L': {
            // process log
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
