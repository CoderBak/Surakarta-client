#include "replay.h"
#include "ui_replay.h"
#include "common.h"
#include <QMouseEvent>
#include <iostream>
#include <QPainter>

Replay::Replay(QWidget *parent) :
        QWidget(parent), ui(new Ui::Replay),
        board(new ReplayBoard(this)), timer(new QTimer(this)) {
    ui->setupUi(this);
    this->layout()->addWidget(board);
    board->setFixedSize(BOARD_HEIGHT + 40, BOARD_HEIGHT + 40);
    connect(ui->nextButton, &QPushButton::clicked, this, &Replay::stepNext);
    connect(ui->backButton, &QPushButton::clicked, this, &Replay::stepBack);
    connect(ui->startButton, &QPushButton::clicked, this, &Replay::handleStart);
    connect(ui->stopButton, &QPushButton::clicked, this, &Replay::handleStop);
    connect(timer, &QTimer::timeout, this, &Replay::stepNext);
    this->setFixedSize(this->size());
}

Replay::~Replay() {
    delete ui;
}

void Replay::handleStart() {
    timer->start(replaySpeed);
}

void Replay::handleStop() {
    timer->stop();
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
    maxStep = static_cast<int>(history.size());
    step = (step % maxStep + maxStep) % maxStep;
    board->processBoardInfo(history[step].toUtf8());
}