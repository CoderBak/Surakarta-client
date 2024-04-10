#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include "qtboard.h"
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void sendData(const QString&);
    void getData();
   // void handleTryAgain();
   // void handleGiveUp();
   // void handleOpenChatroom();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QtBoard *board;
};

#endif //MAINWINDOW_H
