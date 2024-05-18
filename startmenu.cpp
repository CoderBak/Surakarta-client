#include "startmenu.h"
#include <QVBoxLayout>
#include <QIcon>
#include <QStyleOption>
#include <QPainter>

StartMenu::StartMenu(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::StartMenu)
        {
    ui->setupUi(this);
    // Create the layout for the start menu
    // QVBoxLayout * layout = new QVBoxLayout(this);

    // // Load the image
    // QPixmap image(":/image/image/22.jpg");
    // imageLabel = new QLabel();
    // imageLabel->setPixmap(image);
    // layout->addWidget(imageLabel);

    // Create the start button
    // startButton = new QPushButton(this);
    // humanVsAIButton = new QPushButton("Human vs AI", this);
    // settingsButton = new QPushButton("Settings", this);
    // replayButton = new QPushButton("Replay", this);

    // ui->startButton->setIcon(QIcon(":/image/image/background-removed.png"));
    //  startButton->setIconSize(QSize(100, 100)); // 调整图标大小
    //  startButton->setFixedSize(100, 100); // 调整按钮大小

    /* layout->addWidget(startButton);
     layout->addWidget(humanVsAIButton);
     layout->addWidget(settingsButton);
     layout->addWidget(replayButton);

     setLayout(layout);*/

    // Connect the start button click signal to the handleStartButtonClicked slot
    connect(ui->startButton, &QPushButton::clicked, this, &StartMenu::handleStartButtonClicked);
    connect(ui->humanVsAIButton, &QPushButton::clicked, this, &StartMenu::humanVsAI);
    connect(ui->settingsButton, &QPushButton::clicked, this, &StartMenu::settings);
    connect(ui->reshowButton, &QPushButton::clicked, this, &StartMenu::reshow);
}

//mind that you have to override paintevnt to show the picture in qt designer
void StartMenu::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

StartMenu::~StartMenu() {
    delete ui;
}

void StartMenu::handleStartButtonClicked() {
    //isStart=true;
    emit startGame();
}

void StartMenu::handleSettings() {
    emit settings();
}
