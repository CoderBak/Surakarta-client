// SettingsWidget.cpp
#include "settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent)
{
    // 创建滑动条
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);
    sizeButtonGroup = new QButtonGroup(this);

        // Create size radio buttons
        QRadioButton *size6Button = new QRadioButton("6x6", this);
        QRadioButton *size10Button = new QRadioButton("10x10", this);

        // Add buttons to button group with corresponding ids
        sizeButtonGroup->addButton(size6Button, 6);
        sizeButtonGroup->addButton(size10Button, 10);

        // Connect size button group signal to slot
       // connect(sizeButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(applyButtonClicked()));



    // 创建应用按钮
    applyButton = new QPushButton("Apply", this);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
       layout->addWidget(size6Button);
       layout->addWidget(size10Button);
       layout->addWidget(slider);
       layout->addWidget(applyButton);
       setLayout(layout);

    // 连接信号槽
    connect(applyButton, &QPushButton::clicked, this, &Settings::applyButtonClicked);
}

void Settings::applyButtonClicked(int value)
{
     // 获取当前选中的按钮
        QAbstractButton *selectedButton = sizeButtonGroup->checkedButton();
        if (selectedButton) {
            // 获取选中按钮的文本
            QString buttonText = selectedButton->text();

            // 提取按钮文本中的数字（假设按钮文本中只包含数字）
            int value = buttonText.split("x").first().toInt();

            // 发射应用设置信号
            qDebug()<<"emit value of size && value ="<<value;
            emit settingsApplied(value);
        }
}


