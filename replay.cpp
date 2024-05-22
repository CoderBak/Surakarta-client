#include "replay.h"
#include "ui_replay.h"
#include "common.h"
#include <Qpainter>
#include <QMouseEvent>
#include <iostream>
#include <QString>

Replay::Replay(QWidget *parent) :
        QWidget(parent), ui(new Ui::Replay),
        board(new ReplayBoard(this)) {
    ui->setupUi(this);
    this->layout()->addWidget(board);
    this->setFixedSize(WIDTH, HEIGHT);
    connect(ui->nextButton, &QPushButton::clicked, this, &Replay::stepNext);
    connect(ui->backButton, &QPushButton::clicked, this, &Replay::stepBack);
    connect(ui->startButton, &QPushButton::clicked, this, &Replay::handleStart);
    connect(ui->stopButton, &QPushButton::clicked, this, &Replay::handleStop);
    maxStep = static_cast<int>(history.size());
}

Replay::~Replay() {
    delete ui;
}

void Replay::handleStart() {
    qDebug() << "start!";
}

void Replay::handleStop() {
    qDebug() << "stop!";
}

void Replay::stepNext() {
    step += 1;
    print();
}

void Replay::stepBack() {
    step -= 1;
    print();
}

void Replay::print() {
    // TODO: add bound
    board->processBoardInfo(history[step].toUtf8());
}