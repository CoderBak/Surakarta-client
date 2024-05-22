#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    StartMenu menu;
    Settings setting;
    QObject::connect(&menu, &StartMenu::startGame, [&]() {
        menu.hide();
        w.show();
        w.startNow();
    });
    QObject::connect(&menu, &StartMenu::humanVsAI, [&]() {
        menu.hide();
        qDebug() << "Human vs AI button clicked";
        w.show();
    });
    QObject::connect(&menu, &StartMenu::settings, [&]() {
        setting.show();
    });
    QObject::connect(&menu, &StartMenu::reshow, [&]() {

    });
    QObject::connect(&setting, &Settings::settingsApplied, &w, &MainWindow::receiveBoardSizeFromSettings);
    QObject::connect(&setting, &Settings::colorSelected, &w, &MainWindow::receivePieceColorFromSettings);
    QObject::connect(&setting, &Settings::dirSelected, &w, &MainWindow::receiveDirFromSettings);
    menu.show();
    return QApplication::exec();
}
