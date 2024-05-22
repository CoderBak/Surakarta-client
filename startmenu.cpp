#include "startmenu.h"
#include <QVBoxLayout>
#include <QIcon>
#include <QStyleOption>
#include <QPainter>

StartMenu::StartMenu(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::StartMenu) {
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &StartMenu::handleStartButtonClicked);
    connect(ui->humanVsAIButton, &QPushButton::clicked, this, &StartMenu::humanVsAI);
    connect(ui->settingsButton, &QPushButton::clicked, this, &StartMenu::settings);
    connect(ui->reshowButton, &QPushButton::clicked, this, &StartMenu::reshow);
}

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
    emit startGame();
}

void StartMenu::handleSettings() {
    emit settings();
}
