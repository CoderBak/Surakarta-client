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

private slots:

    void applyButtonClicked();

    void handleColorRadioButtonClicked();

    void selectDir();

private:
    QSlider *slider;
    QPushButton *applyButton;
    QButtonGroup *sizeButtonGroup;
    QButtonGroup *colorButtonGroup;
    QLineEdit *fileDir;
};

#endif // SETTINGS_H
