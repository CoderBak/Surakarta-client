#include "startmenu.h"
#include <QVBoxLayout>
#include <QIcon>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent) {
    // Create the layout for the start menu
    QVBoxLayout * layout = new QVBoxLayout(this);

    // Load the image
    QPixmap image(":/image/image/22.jpg");
    imageLabel = new QLabel();
    imageLabel->setPixmap(image);
    layout->addWidget(imageLabel);

    // Create the start button
    startButton = new QPushButton(this);
    humanVsAIButton = new QPushButton("Human vs AI", this);
    settingsButton = new QPushButton("Settings", this);
    replayButton = new QPushButton("Replay", this);

    startButton->setIcon(QIcon(":/image/image/background-removed.png"));
    startButton->setIconSize(QSize(100, 100)); // 调整图标大小
    startButton->setFixedSize(100, 100); // 调整按钮大小

    layout->addWidget(startButton);
    layout->addWidget(humanVsAIButton);
    layout->addWidget(settingsButton);
    layout->addWidget(replayButton);

    setLayout(layout);

    // Connect the start button click signal to the handleStartButtonClicked slot
    connect(startButton, &QPushButton::clicked, this, &StartMenu::handleStartButtonClicked);
    connect(humanVsAIButton, &QPushButton::clicked, this, &StartMenu::humanVsAI);
    connect(settingsButton, &QPushButton::clicked, this, &StartMenu::settings);
    connect(replayButton, &QPushButton::clicked, this, &StartMenu::replay);
}

StartMenu::~StartMenu() {
}

void StartMenu::handleStartButtonClicked() {
    emit startGame();
}
