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
    QObject::connect(&setting, &Settings::settingsApplied, &w, &MainWindow::receiveBoardSizeFromSettings);
    QObject::connect(&setting, &Settings::colorSelected, &w, &MainWindow::receivePieceColorFromSettings);
    QObject::connect(&setting, &Settings::dirSelected, &w, &MainWindow::receiveDirFromSettings);
    menu.show();
    return QApplication::exec();
}
