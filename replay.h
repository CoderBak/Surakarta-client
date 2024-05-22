// In this file, we define our replay engine.
#ifndef REPLAY_H
#define REPLAY_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QMainWindow>
#include <QTimer>
#include "replay_board.h"

namespace Ui { class Replay; }

class Replay : public QWidget {
Q_OBJECT

public:
    explicit Replay(QWidget *parent = nullptr);
    ~Replay() override;
    std::vector<QString> history;
    void print();

public slots:
    void stepBack();
    void stepNext();
    void handleStart();
    void handleStop();

private:
    Ui::Replay *ui;
    ReplayBoard *board;
    int step = 0;
    int maxStep;
    QTimer* timer;
};

#endif //REPLAY_H
