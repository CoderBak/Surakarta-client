// In this file, we implement the start menu for users.
// Users will first see the start menu and then click button to get into window and compete.
// Only special functions will we give explanations.
#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "ui_startmenu.h"

namespace Ui {
    class StartMenu;
}

class StartMenu : public QWidget {
Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;
    ~StartMenu() override;

signals:
    //  We support these functions
    void startGame();
    void settings();
    void reshow();

private slots:
    // slots for processing
    void handleStartButtonClicked();
    void handleSettings();

private:
    Ui::StartMenu *ui;
};

#endif // STARTMENU_H
