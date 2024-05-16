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

    void paintEvent(QPaintEvent *) override;

    ~StartMenu();

    signals:

            void startGame();

    void humanVsAI();

    void settings();

    void reshow();


private
    slots:

            void handleStartButtonClicked();

    void handleSettings();

private:
    QPushButton *startButton;
    QPushButton *humanVsAIButton;
    QPushButton *settingsButton;
    QPushButton *replayButton;
    QLabel *imageLabel;
    Ui::StartMenu *ui;
};

#endif // STARTMENU_H
