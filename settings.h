#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include<QSlider>
#include<QPushButton>
#include<QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>

class Settings : public QWidget {
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);

    signals:

    void settingsApplied(int value);

    void colorSelected(char color);

    void dirSelected(QString dir);

private slots:

    void applyButtonClicked();

    void handleColorRadioButtonClicked();

    void selectDir();

    void handleDirSend();

private:
    QSlider *slider;
    QPushButton *applyButton;
    QButtonGroup *sizeButtonGroup;
    QButtonGroup *colorButtonGroup;
    QLineEdit *fileDir;
    QString selectedDir = "./log";
};

#endif // SETTINGS_H
