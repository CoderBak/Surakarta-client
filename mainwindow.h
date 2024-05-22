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
    void handleEatableQuery(const posType &pos);
    void handleAIQuery(bool isAI);
    void handleMovableQuery(const posType &pos);
    void handleMoveInfo(const QByteArray &);
    void sendTryAgain();
    void getData();
    void handleCheckBoxStateChanged(int state);
    void receiveBoardSizeFromSettings(int size);
    void receivePieceColorFromSettings(char color);
    void receiveDirFromSettings(QString dir);

signals:
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
