#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include "board_utils.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

    void handleEatableQuery(const posType &pos);

    void handleMovableQuery(const posType &pos);

    void handleMoveInfo(const QByteArray &);

    void sendTryAgain();

    void getData();

    // void handleTryAgain();
    // void handleGiveUp();
    // void handleOpenChatroom();

signals:

    void sendEatable(const QByteArray &info);

    void sendMovable(const QByteArray &info);

private:
    void dataHandler(const QByteArray &info);

    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QtBoard *board;
    QByteArray msg;
};

#endif //MAINWINDOW_H
