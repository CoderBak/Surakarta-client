#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include<QSlider>
#include<QPushButton>
#include<QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>

class Settings : public QWidget {
Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);

signals:

    void settingsApplied(int value);
    void colorSelected(char color);

private slots:

    void applyButtonClicked(int value);
    void handleColorRadioButtonClicked(int val);
private:
    QSlider *slider;
    QPushButton *applyButton;
    QButtonGroup *sizeButtonGroup;
    QButtonGroup *colorButtonGroup;




};

#endif // SETTINGS_H
