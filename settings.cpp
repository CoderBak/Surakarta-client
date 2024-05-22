#include "settings.h"
#include <QFileDialog>

Settings::Settings(QWidget *parent) :
        QWidget(parent) {
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);
    sizeButtonGroup = new QButtonGroup(this);
    colorButtonGroup = new QButtonGroup(this);

    auto size6Button = new QRadioButton("6x6", this);
    auto size8Button = new QRadioButton("8x8", this);
    auto size10Button = new QRadioButton("10x10", this);
    auto colorRedButton = new QRadioButton("Red", this);
    auto colorYellowButton = new QRadioButton("Yellow", this);
    auto colorBlueButton = new QRadioButton("Blue", this);
    auto selectDir = new QPushButton("Select log dir", this);
    sizeButtonGroup->addButton(size6Button, 6);
    sizeButtonGroup->addButton(size8Button, 8);
    sizeButtonGroup->addButton(size10Button, 10);
    colorButtonGroup->addButton(colorRedButton);
    colorButtonGroup->addButton(colorYellowButton);
    colorButtonGroup->addButton(colorBlueButton);
    size6Button->setChecked(true);
    colorRedButton->setChecked(true);

    fileDir = new QLineEdit();
    fileDir->setReadOnly(true);
    applyButton = new QPushButton("Apply", this);

    auto layout = new QVBoxLayout(this);
    auto size = new QVBoxLayout();
    auto color = new QVBoxLayout();
    auto horizon = new QHBoxLayout();

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

    connect(applyButton, &QPushButton::clicked, this, &Settings::applyButtonClicked);
    connect(applyButton, &QPushButton::clicked, this, &Settings::handleColorRadioButtonClicked);
    connect(applyButton, &QPushButton::clicked, this, &Settings::handleDirSend);
    connect(applyButton, &QPushButton::clicked, this, [this]() { this->hide(); });
    connect(selectDir, &QPushButton::clicked, this, &Settings::selectDir);
}

void Settings::handleColorRadioButtonClicked() {
    QAbstractButton * selectedButton = colorButtonGroup->checkedButton();
    if (selectedButton) {
        QString colorText = selectedButton->text();
        char colorInitial = colorText.at(0).toLatin1();
        qDebug() << "emit value of color && color =" << colorInitial;
        emit colorSelected(colorInitial);
    }
}

void Settings::applyButtonClicked() {
    QAbstractButton * selectedButton = sizeButtonGroup->checkedButton();
    if (selectedButton) {
        QString buttonText = selectedButton->text();
        int value = buttonText.split("x").first().toInt();
        qDebug() << "emit value of size && value =" << value;
        emit settingsApplied(value);
    }
}

void Settings::selectDir() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder");
    if (!folderPath.isEmpty()) {
        fileDir->setText(folderPath);
        selectedDir = folderPath;
    }
}

void Settings::handleDirSend() {
    emit dirSelected(selectedDir);
}