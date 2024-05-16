#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    StartMenu menu;
    Settings setting;
    //QObject::connect(&menu,&StartMenu::startGame,&w,&MainWindow::startGame);
    QObject::connect(&menu, &StartMenu::startGame, [&]() {
        menu.hide();
        w.show();
        // w.startGame();
    });
    QObject::connect(&menu, &StartMenu::humanVsAI, [&]() {
        menu.hide();
        // TODO:
        qDebug() << "Human vs AI button clicked";
        w.show();
    });

    QObject::connect(&menu, &StartMenu::settings, [&]() {
        // TODO:
        qDebug() << "Settings button clicked";
        setting.show();
    });

    QObject::connect(&menu, &StartMenu::reshow, [&]() {
        // TODO:
        qDebug() << "Replay button clicked";
    });
    // QObject::connect(&setting, &Settings::settingsApplied, board, &QtBoard::receiveDataFromUser);
    QObject::connect(&setting, &Settings::settingsApplied, &w, &MainWindow::receiveBoardSizeFromSettings);
    QObject::connect(&setting, &Settings::colorSelected, &w, &MainWindow::receivePieceColorFromSettings);
    menu.show();
    // w.show();
    return QApplication::exec();
}
