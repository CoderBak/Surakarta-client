#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "board_utils.h"
#include <QtWidgets/QLineEdit>
#include "NetworkLibrary/networksocket.h"
#include "NetworkLibrary/networkdata.h"
#include "startmenu.h"
#include "settings.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    QLineEdit *ip_edit;
    QLineEdit *port_edit;
    QPushButton *connect_button;
    QLineEdit *send_edit;
    QPushButton *send_button;
    QLineEdit *receive_edit;
    QLabel *label;
    QPushButton *disconnect_button;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

    void handleEatableQuery(const posType &pos);

    void handleMovableQuery(const posType &pos);

    void handleMoveInfo(const QByteArray &);

    void sendTryAgain();

    // void processData();

    //void animateMove();

    void getData();

    void connectToServer();

    void connectedSuccessfully();

    void disconnectFromServer();

    void sendMessage();

    void receiveMessage(NetworkData data);

public slots:


    void handleCheckBoxStateChanged(int state);
    //  void showStartMenu();

    //  void startGame();

    // void updateTimeSlot(QString time);

    // void resetTime();

    // void handleTryAgain();
    // void handleGiveUp();
    // void handleOpenChatroom();

signals:

    void aiControlChanged(bool enabled);

    void sendEatable(const QByteArray &info);

    void sendMovable(const QByteArray &info);

private:
    void dataHandler(const QByteArray &info);

    int port = 1233;
    QString ip = "127.0.0.1";
    Ui::MainWindow *ui;
    NetworkSocket *socket1;
    Settings *settings;
    QTcpSocket *socket;
    QtBoard *board;
    QByteArray msg;
    QLabel *titleTotal = nullptr;
    QLabel *titleReset = nullptr;
    QLabel *labelTotal = nullptr;
    QLabel *labelReset = nullptr;

    StartMenu *startMenu;
};

#endif // MAINWINDOW_H
