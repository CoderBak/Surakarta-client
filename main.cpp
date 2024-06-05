// In this file, we implement the main function.
// We connect the start menu, settings, replay and window.
// We will first demonstrate the start menu.
// Users will have different choices on the start menu.
// Then they will get into the window.
// Other functions, competing, and UI for board and timer will be shown there.
#include "mainwindow.h"
#include "replay.h"
#include <QApplication>
#include <QFileDialog>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    StartMenu menu;
    Settings setting;
    Replay replay;

    // connect menu
    QObject::connect(&menu, &StartMenu::startGame, [&]() {
        menu.hide();
        w.show();
        w.startNow();
    });
    QObject::connect(&menu, &StartMenu::settings, [&]() { setting.show(); });
    // initialize menu
    QObject::connect(&menu, &StartMenu::reshow, [&]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "选择回放文件", "", "");
        // Didn't check if the log is valid.
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                bool flag = false;
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    if (line[0] == 'B') {
                        if (flag) {
                            replay.history.push_back(line.mid(1));
                        } else {
                            flag = true;
                        }
                    }
                }
                file.close();
                menu.hide();
                replay.show();
                replay.print();
            } else {
                qDebug() << "Unable to open file." << fileName;
            }
        } else {
            qDebug() << "Failed to select the file.";
        }
    });

    // connect settings
    QObject::connect(&setting, &Settings::settingsApplied, &w, &MainWindow::receiveBoardSizeFromSettings);
    QObject::connect(&setting, &Settings::colorSelected, &w, &MainWindow::receivePieceColorFromSettings);
    QObject::connect(&setting, &Settings::dirSelected, &w, &MainWindow::receiveDirFromSettings);
    menu.show();
    return QApplication::exec();
}
