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

    void sendMousePress(const QString &);

    void sendData();

    void sendTryAgain();

    void getData();

    // void handleTryAgain();
    // void handleGiveUp();
    // void handleOpenChatroom();

private:
    void dataHandler(const QByteArray &info);

    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QtBoard *board;
};

#endif //MAINWINDOW_H
