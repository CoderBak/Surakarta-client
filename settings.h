#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include<QSlider>
#include<QPushButton>
#include<QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);

signals:
    void settingsApplied(int value);

private slots:
    void applyButtonClicked(int value);

private:
    QSlider *slider;
    QPushButton *applyButton;
    QButtonGroup *sizeButtonGroup;

};

#endif // SETTINGS_H
