#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    StartMenu menu;
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
       });

       QObject::connect(&menu, &StartMenu::replay, [&]() {
           // TODO:
           qDebug() << "Replay button clicked";
       });
    menu.show();
   // w.show();
    return QApplication::exec();
}
