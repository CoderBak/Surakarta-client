#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>

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
    void sendData();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
};

#endif //MAINWINDOW_H
