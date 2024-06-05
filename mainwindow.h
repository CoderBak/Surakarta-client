// This is our MainWindow.
// The window is designed for users to see when chess competing.
// We support the UI for board, a timer, and other buttons for users to control the game.
// Only special functions will we give explanations.
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "common.h"
#include <QtWidgets/QLineEdit>
#include "startmenu.h"
#include "settings.h"
#include "board.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void startNow();
    ~MainWindow() override;

public slots:
    // handle functions are used to process data
    void handleEatableQuery(const posType &pos);
    void handleAIQuery(bool isAI);
    void handleMovableQuery(const posType &pos);
    void handleMoveInfo(const QByteArray &);
    void handleCheckBoxStateChanged(int state);
    void sendTryAgain();

    // These functions are used to receive data
    void getData();
    void receiveBoardSizeFromSettings(int size);
    void receivePieceColorFromSettings(char color);
    void receiveDirFromSettings(QString dir);

signals:
    // these signals are to send data
    void aiControlChanged(bool enabled);
    void sendEatable(const QByteArray &info);
    void sendMovable(const QByteArray &info);

private:
    void dataHandler(const QByteArray &info);
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QtBoard *board;
    QByteArray msg;
    QLabel *titleTotal = nullptr;
    QLabel *titleReset = nullptr;
    QLabel *labelTotal = nullptr;
    QLabel *labelReset = nullptr;
    QString savePlace = "./log";
};

#endif // MAINWINDOW_H
