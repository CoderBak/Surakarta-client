#include "settings.h"
#include <QFileDialog>

Settings::Settings(QWidget *parent) :
        QWidget(parent) {
    // 创建滑动条
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);
    sizeButtonGroup = new QButtonGroup(this);
    colorButtonGroup = new QButtonGroup(this);
    // Create size radio buttons
    QRadioButton* size6Button = new QRadioButton("6x6", this);
    QRadioButton* size8Button = new QRadioButton("8x8", this);
    QRadioButton* size10Button = new QRadioButton("10x10", this);
    QRadioButton* colorRedButton = new QRadioButton("Red", this);
    QRadioButton* colorYellowButton = new QRadioButton("Yellow", this);
    QRadioButton* colorBlueButton = new QRadioButton("Blue", this);
    QPushButton* selectDir = new QPushButton("Select log dir", this);
    // Add buttons to button group with corresponding ids
    sizeButtonGroup->addButton(size6Button, 6);
    sizeButtonGroup->addButton(size8Button, 8);
    sizeButtonGroup->addButton(size10Button, 10);
    colorButtonGroup->addButton(colorRedButton);
    colorButtonGroup->addButton(colorYellowButton);
    colorButtonGroup->addButton(colorBlueButton);
    size6Button->setChecked(true);
    colorRedButton->setChecked(true);

    // Connect size button group signal to slot
    // connect(sizeButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(applyButtonClicked()));

    // 创建应用按钮
    fileDir = new QLineEdit();
    fileDir->setReadOnly(true);
    applyButton = new QPushButton("Apply", this);

    // 布局
    QVBoxLayout * layout = new QVBoxLayout(this);
    QVBoxLayout * size = new QVBoxLayout();
    QVBoxLayout * color = new QVBoxLayout();
    QHBoxLayout * horizon = new QHBoxLayout();

    size->addWidget(size6Button);
    size->addWidget(size8Button);
    size->addWidget(size10Button);
    color->addWidget(colorRedButton);
    color->addWidget(colorYellowButton);
    color->addWidget(colorBlueButton);
    horizon->addLayout(size);
    horizon->addLayout(color);
    layout->addLayout(horizon);
    layout->addWidget(slider);
    layout->addWidget(selectDir);
    layout->addWidget(fileDir);
    layout->addWidget(applyButton);
    setLayout(layout);

    // 连接信号槽
    connect(applyButton, &QPushButton::clicked, this, &Settings::applyButtonClicked);
    connect(applyButton, &QPushButton::clicked, this, &Settings::handleColorRadioButtonClicked);
    connect(applyButton, &QPushButton::clicked, this, &Settings::handleDirSend);
    connect(applyButton, &QPushButton::clicked, this, [this]() { this->hide(); });
    connect(selectDir, &QPushButton::clicked, this, &Settings::selectDir);
}

void Settings::handleColorRadioButtonClicked() {
    // 发送颜色选中的首字母信号
    QAbstractButton * selectedButton = colorButtonGroup->checkedButton();
    if (selectedButton) {
        QString colorText = selectedButton->text();
        char colorInitial = colorText.at(0).toLatin1();
        qDebug() << "emit value of color && color =" << colorInitial;
        emit colorSelected(colorInitial);
    }
}

void Settings::applyButtonClicked() {
    // 获取当前选中的按钮
    QAbstractButton * selectedButton = sizeButtonGroup->checkedButton();
    if (selectedButton) {
        // 获取选中按钮的文本
        QString buttonText = selectedButton->text();

        // 提取按钮文本中的数字（假设按钮文本中只包含数字）
        int value = buttonText.split("x").first().toInt();

        // 发射应用设置信号
        qDebug() << "emit value of size && value =" << value;
        emit settingsApplied(value);
    }
}

void Settings::selectDir() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder");
    if (!folderPath.isEmpty()) {
        fileDir->setText(folderPath);
        selectedDir = folderPath;
        //qDebug () << folderPath;
    }
}

void Settings::handleDirSend() {
    emit dirSelected(selectedDir);
}