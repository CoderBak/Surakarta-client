#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qpainter>
#include <QMouseEvent>
#include <iostream>
#include <sstream>
#include <QTextStream>
#include <QString>
#include <QCursor>
#include <QMenuBar>
#include <QStatusBar>
#include <QThread>
#include "startmenu.h"

QtBoard::QtBoard(QWidget *parent) : QWidget(parent) {
    this->installEventFilter(this);
    setMouseTracking(true);
    for (unsigned int i = 0; i < BOARD_SIZE; i += 1) {
        for (unsigned int j = 0; j < BOARD_SIZE; j += 1) {
            chessColor[i][j] = NONE;
        }
    }
    // connect(&settings, &Settings::settingsApplied, this, &QtBoard::receiveDataFromUser);
}

void QtBoard::setHandledByAI(bool enabled) {
    handledByAI = enabled;
    qDebug() << " handledByAI = " << handledByAI;
}

QtBoard::~QtBoard() {
    //delete animationTimer;
}

bool QtBoard::eventFilter(QObject *obj, QEvent *event) {
    // If the event is a mouse press.
    if (event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            // Change the type of the cursor.
            setCursor(Qt::ArrowCursor);
        }
    }
    return QWidget::eventFilter(obj, event);
}

// Set the current player.
void QtBoard::setCurrentPlayer(ChessColor cur) {
    current_player = cur;
}

void QtBoard::receiveDataFromUser(int value, char piece_color) {
    if (value != 0)
        qDebug() << "BOARD_SIZE = " << value;
    if (piece_color != 0)
        qDebug() << "piece_color = " << piece_color;

}

// Send the pressed information.
void MainWindow::handleMoveInfo(const QByteArray &moveInfo) {
    socket->write(moveInfo);
}

void MainWindow::handleEatableQuery(const posType &pos) {
    QByteArray query = QString("$QC%1;%2").arg(pos.first).arg(pos.second).toUtf8();
    socket->write(query);
}

void MainWindow::handleMovableQuery(const posType &pos) {
    QByteArray query = QString("$QN%1;%2").arg(pos.first).arg(pos.second).toUtf8();
    socket->write(query);
}

void MainWindow::handleCheckBoxStateChanged(int state) {
    bool enable = (state == Qt::Checked);
    emit aiControlChanged(enable);
}

void MainWindow::receiveBoardSizeFromSettings(int size) {
    board->receiveDataFromUser(size, 0);
}

void MainWindow::receivePieceColorFromSettings(char color) {
    board->receiveDataFromUser(0, color);
}

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        socket(new QTcpSocket(this)),
        board(new QtBoard(this)),
        setting(new Settings(this)) {

    ui->setupUi(this);

    // Set timer

    titleTotal = new QLabel("Total time", this);
    titleReset = new QLabel("Current Time", this);
    titleTotal->move(10, 30);
    titleReset->move(10, 70);
    labelTotal = new QLabel("00::00::00", this);
    labelTotal->move(10, 50);
    labelReset = new QLabel("00::00::00", this);
    labelReset->move(10, 90);

    // Set the layout of the interface.
    auto *layout = new QHBoxLayout;
    auto *menu = new QVBoxLayout;

    // Add buttons to the menu.
    menu->addWidget(ui->tryAgainButton);
    menu->addWidget(ui->giveUpButton);
    menu->addWidget(ui->openChatroomButton);
    menu->addWidget(ui->Altrusteeship);
    // // Set the layout.
    layout->addLayout(menu);
    layout->addWidget(board);

    // Set the window size.
    auto *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    centralWidget->setFixedSize(WIDTH, HEIGHT);
    setCentralWidget(centralWidget);
    // setStyleSheet(STYLE);

    //initial connect ui

    //connect ui
    /*
    ip_edit = new QLineEdit(this);
    ip_edit->setObjectName(QString::fromUtf8("ip_edit"));
    ip_edit->setGeometry(QRect(10, 230, 111, 20));
    ip_edit->setAlignment(Qt::AlignCenter);
    menu->addWidget(ip_edit);

    port_edit = new QLineEdit(this);
    port_edit->setObjectName(QString::fromUtf8("port_edit"));
    port_edit->setGeometry(QRect(140, 230, 51, 20));
    port_edit->setAlignment(Qt::AlignCenter);
    menu->addWidget(port_edit);

    connect_button = new QPushButton(this);
    connect_button->setObjectName(QString::fromUtf8("connect_button"));
    connect_button->setGeometry(QRect(210, 230, 80, 20));
    menu->addWidget(connect_button);

    send_edit = new QLineEdit(this);
    send_edit->setObjectName(QString::fromUtf8("send_edit"));
    send_edit->setGeometry(QRect(10, 260, 181, 20));
    send_edit->setAlignment(Qt::AlignCenter);
    menu->addWidget(send_edit);

    send_button = new QPushButton(this);
    send_button->setObjectName(QString::fromUtf8("send_button"));
    send_button->setGeometry(QRect(210, 260, 81, 21));
    menu->addWidget(send_button);

    receive_edit = new QLineEdit(this);
    receive_edit->setObjectName(QString::fromUtf8("receive_edit"));
    receive_edit->setGeometry(QRect(10, 290, 181, 20));
    receive_edit->setAlignment(Qt::AlignCenter);
    menu->addWidget(receive_edit);

    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(210, 290, 81, 16));
    label->setAlignment(Qt::AlignCenter);
    menu->addWidget(label);

    disconnect_button = new QPushButton(this);
    disconnect_button->setObjectName(QString::fromUtf8("disconnect_button"));
    disconnect_button->setGeometry(QRect(10, 320, 281, 21));
    menu->addWidget(disconnect_button);*/


    //menubar = new QMenuBar(this);
    //setMenuBar(menubar);
    //menu->addWidget(menubar);

    //statusbar = new QStatusBar(this);
    //setStatusBar(statusbar);
    //menu->addWidget(statusbar);
/*
    connect_button->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
    send_button->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
    label->setText(QCoreApplication::translate("MainWindow", "Receive", nullptr));
    disconnect_button->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));


    //initial ui
    this->ip_edit->setText(ip);
    this->port_edit->setText(QString::number(port));
    this->send_button->setEnabled(false);
    this->disconnect_button->setEnabled(false);
    this->receive_edit->setReadOnly(true);

    //connect ui
    socket1 = new NetworkSocket(new QTcpSocket(this), this);
    connect(socket1->base(), &QTcpSocket::connected, this,
            &MainWindow::connectedSuccessfully);  // connected 是客户端连接成功后发出的信号
    connect(this->connect_button, &QPushButton::clicked, this, &MainWindow::connectToServer); // 连接服务器
    connect(this->disconnect_button, &QPushButton::clicked, this, &MainWindow::disconnectFromServer); // 断开连接
    connect(this->send_button, &QPushButton::clicked, this, &MainWindow::sendMessage); // 发送消息
    connect(socket1, &NetworkSocket::receive, this, &MainWindow::receiveMessage);*/

    // Connect the button to functions.
    // connect(startMenu, &StartMenu::startGame,this,&MainWindow::startGame);
    //qDebug()<<" run before connect";
    //connect(setting, &Settings::settingsApplied, board, &QtBoard::receiveDataFromUser);

    connect(ui->Altrusteeship, &QCheckBox::stateChanged, this, &MainWindow::handleCheckBoxStateChanged);
    connect(this, &MainWindow::aiControlChanged, board, &QtBoard::setHandledByAI);
    connect(board, &QtBoard::sendMovableQuery, this, &MainWindow::handleMovableQuery);
    connect(board, &QtBoard::sendEatableQuery, this, &MainWindow::handleEatableQuery);
    connect(board, &QtBoard::sendMoveInfo, this, &MainWindow::handleMoveInfo);
    connect(this, &MainWindow::sendEatable, board, &QtBoard::handleEatable);
    connect(this, &MainWindow::sendMovable, board, &QtBoard::handleMovable);
    connect(ui->tryAgainButton, &QPushButton::clicked, this, &MainWindow::sendTryAgain);
    // connect(giveUpButton, &QPushButton::clicked, this, &MainWindow::handleGiveUp);
    // connect(openChatroomButton, &QPushButton::clicked, this, &MainWindow::handleOpenChatroom);

    // Connect the server.
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::getData);
    //connect(socket, &QTcpSocket::readyRead, this, &MainWindow::getTimeData);

    socket->connectToHost("localhost", PORT);
    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect to remote host, try to connect localhost";
        socket->connectToHost("localhost", PORT);
        if (!socket->waitForConnected()) {
            qDebug() << "Failed to connect to localhost, abort!";
            exit(-1);
        }
    } else {
        qDebug() << "Remote host connected successfully!";
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::sendTryAgain() {
    socket->write("$G;");
}

// Process data in this function.
void MainWindow::getData() {
    QByteArray data = socket->readAll();
    // qDebug() << "Received message from server: " << data;
    if (data[0] != '$') {
        qDebug() << "Wrong format!";
    } else {
        while (true) {
            const auto endPos = data.indexOf('$', 1);
            if (endPos != -1) {
                dataHandler(data.mid(1, endPos - 1));
                data.remove(0, endPos);
            } else {
                data.remove(0, 1);
                dataHandler(data);
                break;
            }
        }
    }
    socket->read(socket->bytesAvailable());
}

void MainWindow::connectToServer() {
    this->ip = this->ip_edit->text();
    this->port = this->port_edit->text().toInt();
    socket1->hello(ip, port);
    this->socket1->base()->waitForConnected(2000);
}

void MainWindow::connectedSuccessfully() {
    this->connect_button->setEnabled(false);
    this->disconnect_button->setEnabled(true);
    this->send_button->setEnabled(true);
    this->port_edit->setReadOnly(true);
    this->ip_edit->setText("Connected");
    this->ip_edit->setReadOnly(true);
    socket1->send(NetworkData(OPCODE::READY_OP, "", "", ""));
}

void MainWindow::disconnectFromServer() {
    socket1->send(NetworkData(OPCODE::LEAVE_OP, "", "", ""));
    socket1->bye();
    this->connect_button->setEnabled(true);
    this->disconnect_button->setEnabled(false);
    this->send_button->setEnabled(false);
    this->port_edit->setReadOnly(false);
    this->ip_edit->setReadOnly(false);
    this->ip_edit->setText(ip);
}

void MainWindow::sendMessage() {
    QString message = this->send_edit->text();
    socket1->send(NetworkData(OPCODE::CHAT_OP, "", message, "")); // 发送消息给服务端，是不是很简单
    this->send_edit->clear();
}

void MainWindow::receiveMessage(NetworkData data) {
    this->receive_edit->setText(data.data2);
}

// void MainWindow::updateTimeSlot(QString time) {
//     // labelTotal->setText(time);
//     // labelReset->setText(time);
// }

void MainWindow::dataHandler(const QByteArray &info) {
    if (info[0] != 'T') {
        qDebug() << "Processing info:" << info;
    }
    switch (info[0]) {
        case 'S': {
            qDebug() << "I'm player" << info[1];
            switch (info[1]) {
                case 'B':
                    board->setCurrentPlayer(BLACK);
                    break;
                case 'W':
                    board->setCurrentPlayer(WHITE);
                    break;
                default:
                    board->setCurrentPlayer(NONE);
            }
            break;
        }
        case 'R': {
            QByteArray toSend = info.mid(2, info.length());
            if (info[1] == 'C') {
                emit sendEatable(toSend);
            } else {
                emit sendMovable(toSend);
            }
            break;
        }
        case 'T': {
            QByteArray timeData = info.mid(5).trimmed(); // Remove "$TIME:" prefix
            if (timeData[0] == 'R') {
                //qDebug()<<timeData;
                timeData = timeData.mid(1).trimmed();
                QString timeString = QString::fromUtf8(timeData);
                labelReset->setText(timeString);
            } else if (timeData[0] == 'T') {
                //qDebug()<<timeData;
                timeData = timeData.mid(1).trimmed();
                QString timeString = QString::fromUtf8(timeData);
                labelTotal->setText(timeString);
            }
            break;
        }
        case 'E': {
            if (info[1] == 'T') {
                qDebug() << "Time out!";
            } else if (info[1] == 'F') {
                qDebug() << "Opponent time out";
            }
            break;
        }
        case 'B': {
            board->processBoardInfo(info.mid(1));
            break;
        }
        default:
            return;
    }
}

// Process boardInfo in this function.
void QtBoard::processBoardInfo(const QByteArray &boardInfo) {
    qDebug() << "PROCESSING BOARD";
    QString boardInfoStr = QString::fromUtf8(boardInfo);
    QStringList rows = boardInfoStr.split('\n');
    for (int row = 0; row < BOARD_SIZE; row += 1) {
        QString rowString = rows.value(row).trimmed();
        rowString.remove(' ');
        QTextStream stream(&rowString);
        for (int col = 0; col < BOARD_SIZE; col += 1) {
            QChar pieceChar;
            stream >> pieceChar;
            // qDebug() << pieceChar;
            switch (pieceChar.toLatin1()) {
                case 'B':
                    chessColor[row][col] = BLACK;
                    break;
                case 'W':
                    chessColor[row][col] = WHITE;
                    break;
                case '.':
                    chessColor[row][col] = NONE;
                    break;
                default:
                    continue;
            }
        }
    }
    // Animation & repaint
    if (shouldCheckAnimation) {
        animateMove();
        shouldCheckAnimation = false;
    } else {
        repaint();
    }
}

void QtBoard::animateMove() {
    bool isEatableFound = false;
    currentPath = nullptr;
    if (eatable.empty()) {
        repaint();
        return;
    }
    if (currentPath == nullptr) {
        for (const auto &elem: eatable) {
            const auto &target = elem.first;
            const auto pieceColor = chessColor[target.first][target.second];
            if (pieceColor == current_player) {
                lastRow = fromRow;
                lastCol = fromCol;
                currentPath = &(elem.second);
                currentPathIndex = 0;
                isEatableFound = true;
                chessColor[target.first][target.second] = (pieceColor == WHITE) ? BLACK : WHITE;
                break;
            }
        }
    }
    if (currentPath != nullptr || isEatableFound) {
        inAnimation = true;
        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, &QtBoard::animationStep);
        animationTimer->setInterval(ANIMATION_PER_TIME / ANIMATION_STEP);
        animationTimer->start();
    } else {
        repaint();
    }
}

// Translate the coordinates.
std::pair<int, int> translateIdx(const unsigned int x, const unsigned int y) {
    return std::make_pair(DELTA_X + x * cellSize + cellSize / 2, DELTA_Y + y * cellSize + cellSize / 2);
}

auto calculateRotation = [](auto step, auto row, auto col) {
    int centerX, centerY, layer;
    double angle = step * 1.0 / ANIMATION_STEP * 270;
    const int x = std::abs(row - col), n = BOARD_SIZE;
    // 1 = 逆时针
    if (row == n - 1 && col < n / 2 || col == 0 && row >= n / 2) {
        centerX = DELTA_X;
        centerY = DELTA_Y + BOARD_HEIGHT;
        layer = n - x;
        // 逆时针
        if (row != n - 1) {
            angle = 90 + angle;
        } else {
            angle = -angle;
        }
    }
    if (row == n - 1 && col >= n / 2 || col == n - 1 && row >= n / 2) {
        centerX = DELTA_X + BOARD_HEIGHT;
        centerY = DELTA_Y + BOARD_HEIGHT;
        layer = x + 1;
        // 逆时针
        if (row == n - 1) {
            angle = 180 + angle;
        } else {
            angle = 90 - angle;
        }
    }
    if (row == 0 && col < n / 2 || col == 0 && row < n / 2) {
        centerX = DELTA_X;
        centerY = DELTA_Y;
        layer = x + 1;
        // 逆时针
        if (row == 0) {
            angle = angle;
        } else {
            angle = 270 - angle;
        }
    }
    if (row == 0 && col >= n / 2 || col == n - 1 && row < n / 2) {
        centerX = DELTA_X + BOARD_HEIGHT;
        centerY = DELTA_Y;
        layer = n - x;
        // 逆时针
        if (row != 0) {
            angle = 270 + angle;
        } else {
            angle = 180 - angle;
        }
    }
    const double radius = cellSize * layer - cellSize * 1.0 / 2;
    centerX = centerX + static_cast<int>(radius * std::cos(angle * M_PI / 180));
    centerY = centerY - static_cast<int>(radius * std::sin(angle * M_PI / 180));
    return std::make_pair(centerX, centerY);
};

void QtBoard::animationStep() {
    if (currentPathIndex < currentPath->size()) {
        if (lastRow != -1 && lastCol != -1) {
            chessColor[lastRow][lastCol] = NONE;
        }
        const auto [row, col] = (*currentPath)[currentPathIndex];
        if ((row - lastRow) * (col - lastCol) == 0) {
            const auto [u, v] = translateIdx(lastCol, lastRow);
            const auto [s, t] = translateIdx(col, row);
            extraX = u + static_cast<int>((s - u) * currentStep * 1.0 / ANIMATION_STEP);
            extraY = v + static_cast<int>((t - v) * currentStep * 1.0 / ANIMATION_STEP);
            repaint();
            extraX = -1;
            extraY = -1;
            currentStep += 10;

        } else {
            const auto [centerX, centerY] = calculateRotation(currentStep, lastRow, lastCol);
            extraX = centerX;
            extraY = centerY;
            repaint();
            extraX = -1;
            extraY = -1;
            currentStep += 1;
        }
        if (currentStep > ANIMATION_STEP) {
            chessColor[row][col] = current_player;
            lastRow = row;
            lastCol = col;
            currentPathIndex += 1;
            currentStep = 1;
        }
    } else {
        animationTimer->stop();
        disconnect(animationTimer, &QTimer::timeout, this, &QtBoard::animationStep);
        currentPathIndex = 0;
        currentPath = nullptr;
        eatable.clear();
        inAnimation = false;
        currentStep = 1;
        repaint();
    }
}

// Draw arcs at corner.
auto drawArcs = [](QPainter &painter, auto centerX, auto centerY, auto startAngle, auto k) {
    constexpr int endAngle = 270 * 16;
    painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH, Qt::DashLine));
    for (int i = 1; i <= k; i += 1) {
        const int radius = i * cellSize;
        QRectF rect(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
        painter.drawArc(rect, startAngle, endAngle);
    }
    painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH));
};

void QtBoard::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), BACK_COLOR);

    if (inAnimation) {
        highlightPath(painter);
    }

    // Draw k = size / 2 layers of arc, centered at centerX, centerY.
    painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH));
    for (int i = 0; i < BOARD_SIZE; i += 1) {
        for (int j = 0; j < BOARD_SIZE; j += 1) {
            painter.save();
            painter.drawRect(DELTA_X + i * cellSize, DELTA_Y + j * cellSize, cellSize, cellSize);
            painter.restore();
        }
    }
    drawArcs(painter, DELTA_X, DELTA_Y, 0, BOARD_SIZE / 2);
    drawArcs(painter, DELTA_X + BOARD_HEIGHT, DELTA_Y, 270 * 16, BOARD_SIZE / 2);
    drawArcs(painter, DELTA_X, DELTA_Y + BOARD_HEIGHT, 90 * 16, BOARD_SIZE / 2);
    drawArcs(painter, DELTA_X + BOARD_HEIGHT, DELTA_Y + BOARD_HEIGHT, 180 * 16, BOARD_SIZE / 2);

    auto emphasize = [&](const int col, const int row, auto color, auto line, auto size) {
        const auto [centerX, centerY] = translateIdx(col, row);
        const QRect rect(centerX - chessRadius, centerY - chessRadius, 2 * chessRadius, 2 * chessRadius);
        painter.setPen(QPen(color, size, line));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(rect);
    };

    // Show the chess at clicked.
    if (selectedPieceRow != -1 && selectedPieceCol != -1) {
        emphasize(selectedPieceCol, selectedPieceRow, SELECTED_COLOR, selectedLine, selectedSize);
        // Show the eatable
        for (const auto &elem: eatable) {
            emphasize(elem.first.second, elem.first.first, EATABLE_COLOR, selectedLine, selectedSize);
        }
        // Show the movable
        for (const auto &elem: movable) {
            emphasize(elem.second, elem.first, MOVABLE_COLOR, emphasizeLine, emphasizeSize);
        }
    }

    drawChess();
}

void QtBoard::highlightPath(QPainter &painter) {
    auto fillPath = [&](auto centerX, auto centerY, auto startAngle, auto layer) {
        int radius = layer * cellSize;
        constexpr int endAngle = 270 * 16;
        painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH, Qt::DashLine));
        painter.setBrush(PATH_COLOR);
        QRectF rect(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
        painter.drawPie(rect, startAngle, endAngle);
        radius -= cellSize;
        painter.setBrush(BACK_COLOR);
        rect = QRectF(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
        painter.drawPie(rect, startAngle, endAngle);
        drawArcs(painter, centerX, centerY, startAngle, layer - 1);
        painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(DELTA_X, DELTA_Y, BOARD_HEIGHT, BOARD_HEIGHT);
    };

    auto isNeighbour = [](const int x1, const int y1, const int x2, const int y2) {
        return (x1 - x2) * (y1 - y2) == 0;
    };

    auto emphasize = [&](const int col, const int row, auto color) {
        const auto [centerX, centerY] = translateIdx(col, row);
        const QRect rect(centerX - cellSize / 2, centerY - cellSize / 2, cellSize, cellSize);
        painter.fillRect(rect, PATH_COLOR);
    };

    auto transfer = [&](const int col, const int row, const int futureCol, const int futureRow) {
        if (!isNeighbour(row, col, futureRow, futureCol)) {
            const int x = std::abs(row - col), n = BOARD_SIZE;
            if (row == n - 1 && col < n / 2 || col == 0 && row >= n / 2) {
                fillPath(DELTA_X, DELTA_Y + BOARD_HEIGHT, 90 * 16, n - x);
            }
            if (row == n - 1 && col >= n / 2 || col == n - 1 && row >= n / 2) {
                fillPath(DELTA_X + BOARD_HEIGHT, DELTA_Y + BOARD_HEIGHT, 180 * 16, x + 1);
            }
            if (row == 0 && col < n / 2 || col == 0 && row < n / 2) {
                fillPath(DELTA_X, DELTA_Y, 0, x + 1);
            }
            if (row == 0 && col >= n / 2 || col == n - 1 && row < n / 2) {
                fillPath(DELTA_X + BOARD_HEIGHT, DELTA_Y, 270 * 16, n - x);
            }
        }
    };

    const auto size = currentPath->size();
    emphasize(fromCol, fromRow, PATH_COLOR);
    emphasize((*currentPath)[size - 1].second, (*currentPath)[size - 1].first, PATH_COLOR);
    transfer(fromCol, fromRow, (*currentPath)[0].second, (*currentPath)[0].first);

    for (int i = 0; i < size - 1; i += 1) {
        const auto [row, col] = (*currentPath)[i];
        const auto [futureRow, futureCol] = (*currentPath)[i + 1];
        transfer(col, row, futureCol, futureRow);
        emphasize(col, row, PATH_COLOR);
    }
}

// Draw all the chess.
void QtBoard::drawChess() {
    QPainter painter(this);
    for (unsigned int i = 0; i < BOARD_SIZE; i++) {
        for (unsigned int j = 0; j < BOARD_SIZE; j++) {
            if (chessColor[i][j] != NONE) {
                const auto currentColor = (chessColor[i][j] == BLACK) ? Qt::black : Qt::white;
                painter.setPen(QPen(CHESS_BORDER, PEN_WIDTH));
                painter.setBrush(QBrush(currentColor, Qt::SolidPattern));
                const auto [centerX, centerY] = translateIdx(j, i);
                const QRect rect(centerX - chessRadius, centerY - chessRadius, 2 * chessRadius, 2 * chessRadius);
                painter.drawEllipse(rect);
            }
        }
    }
    if (extraX != -1 && extraY != -1) {
        const auto currentColor = (current_player == BLACK) ? Qt::black : Qt::white;
        painter.setPen(QPen(CHESS_BORDER, PEN_WIDTH));
        painter.setBrush(QBrush(currentColor, Qt::SolidPattern));
        const QRect rect(extraX - chessRadius, extraY - chessRadius, 2 * chessRadius, 2 * chessRadius);
        painter.drawEllipse(rect);
    }
}

// Handle mouse press event.
void QtBoard::mousePressEvent(QMouseEvent *event) {
    QPoint mousePos = event->pos();
    if (event->button() == Qt::LeftButton) {
        const int row = (mousePos.y() - DELTA_Y) / cellSize;
        const int col = (mousePos.x() - DELTA_X) / cellSize;
        if (0 <= row && row < BOARD_SIZE && 0 <= col && col < BOARD_SIZE) {
            if (chessColor[row][col] == current_player) {
                selectedPieceRow = row;
                selectedPieceCol = col;
                eatable.clear();
                movable.clear();
                emit sendEatableQuery(std::make_pair(col, row));
                emit sendMovableQuery(std::make_pair(col, row));
                repaint();
            } else {
                if (selectedPieceRow != -1 && selectedPieceCol != -1) {
                    QString moveInfo = QString("$M%1;%2;%3;%4")
                            .arg(selectedPieceCol).arg(selectedPieceRow).arg(col).arg(row);
                    fromRow = selectedPieceRow;
                    fromCol = selectedPieceCol;
                    selectedPieceRow = selectedPieceCol = -1;
                    shouldCheckAnimation = true;
                    emit sendMoveInfo(moveInfo.toUtf8());
                    repaint();
                }
            }
        }
    }
}

void QtBoard::handleMovable(const QByteArray &info) {
    movable.clear();
    auto startIdx = info.indexOf("|");
    int n = info.left(startIdx).toInt();
    if (n != 0) {
        QString coords = info.mid(startIdx + 1);
        QStringList pairs = coords.split("|");
        for (const QString &pair: pairs) {
            QStringList values = pair.split(";");
            int x = values[0].toInt(), y = values[1].toInt();
            movable.emplace_back(x, y);
        }
    }
    repaint();
}

void QtBoard::handleEatable(const QByteArray &info) {
    // example:
    // 2|6;7@2;1@2;2@2;3@2;4@2;5@2;6@2;7@2;8@2;9@0;7@1;7@2;7@3;7@4;7@5;7@6;7|8;2@0;2@1;2@2;2@3;2@4;2@5;2@6;2@7;2@8;2
    eatable.clear();
    auto startIdx = info.indexOf("|");
    int n = info.left(startIdx).toInt();
    if (n != 0) {
        QString coords = info.mid(startIdx + 1);
        QStringList paths = coords.split("|");
        for (const QString &path: paths) {
            QStringList pairs = path.split("@");
            bool started = false;
            posType target;
            std::vector<posType> generatedPath;
            for (const QString &pair: pairs) {
                auto unzip = [&]() {
                    QStringList values = pair.split(";");
                    return std::make_pair(values[0].toInt(), values[1].toInt());
                };
                if (!started) {
                    target = unzip();
                    started = true;
                } else {
                    generatedPath.push_back(unzip());
                }
            }
            eatable.emplace_back(target, generatedPath);
        }
    }
    repaint();
}

// Handle move mouse event.
void QtBoard::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    QPoint mousePos = event->pos();
    // qDebug() << mousePos;
    // Translate the coordinates.
    const int x = mousePos.x(), y = mousePos.y();
    const int dx = x - DELTA_X, dy = y - DELTA_Y;
    if (0 <= dx && dx <= BOARD_HEIGHT && 0 <= dy && dy <= BOARD_HEIGHT) {
        const int row = dy / cellSize, col = dx / cellSize;
        const auto [centerX, centerY] = translateIdx(col, row);
        if (chessColor[row][col] == current_player) {
            if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                QApplication::setOverrideCursor(Qt::PointingHandCursor);
                return;
            }
        }
        for (const auto &elem: eatable) {
            if (elem.first.first == row && elem.first.second == col) {
                if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
                    return;
                }
            }
        }
        for (const auto &elem: movable) {
            if (elem.first == row && elem.second == col) {
                if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
                    return;
                }
            }
        }
    }
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}
