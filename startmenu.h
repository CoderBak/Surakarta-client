#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>
#include <QPushButton>
#include<QLabel>
#include "ui_startmenu.h"

namespace Ui {
    class StartMenu;
}
class StartMenu : public QWidget {
Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);

    ~StartMenu();

signals:

    void startGame();

    void humanVsAI();

    void settings();

    void replay();


private slots:

    void handleStartButtonClicked();

private:
    QPushButton *startButton;
    QPushButton *humanVsAIButton;
    QPushButton *settingsButton;
    QPushButton *replayButton;
    QLabel *imageLabel;
    Ui::StartMenu *ui;
};

#endif // STARTMENU_H
