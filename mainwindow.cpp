#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qpainter>
#include <QMouseEvent>
#include <iostream>
#include <sstream>
#include <QTextStream>
#include <QString>
#include <QCursor>

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

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        socket(new QTcpSocket(this)),
        board(new QtBoard(this)) {
    ui->setupUi(this);

    socket1=new NetworkSocket(new QTcpSocket(this),this);
    //if with bottom
    // connect(socket->base(), &QTcpSocket::connected, this, &MainWindow::connectedSuccessfully);
    // connect(ui->connect_button, &QPushButton::clicked, this, &MainWindow::connectToServer);
    // connect(ui->disconnect_button, &QPushButton::clicked, this, &MainWindow::disconnectFromServer);

    // Set timer
    titleTotal = new QLabel("Total time",this);
    titleReset = new QLabel("Current Time",this);
    titleTotal->move(10,30);
    titleReset->move(10,70);
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

    // Set the layout.
    layout->addLayout(menu);
    layout->addWidget(board);

    // Set the window size.
    auto *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    centralWidget->setFixedSize(WIDTH, HEIGHT);
    setCentralWidget(centralWidget);
    // setStyleSheet(STYLE);

    // Connect the button to functions.
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
    qDebug() << "Received message from server: " << data;
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

void MainWindow::connectToServer()
{
    //this->ip = ui->ip_edit->text();
    //this->port = ui->port_edit->text().toInt();
    socket1->hello(ip, port);
    this->socket1->base()->waitForConnected(2000);
}

void MainWindow::connectedSuccessfully()
{
    socket1->send(NetworkData(OPCODE::READY_OP,"","",""));
}

void MainWindow::disconnectFromServer()
{
    socket1->send(NetworkData(OPCODE::LEAVE_OP,"","",""));
    socket1->bye();
}

// void MainWindow::sendMessage(const QString message)
// {
//     socket1->send(NetworkData(OPCODE::CHAT_OP,"",message,""));
// }

// void MainWindow::updateTimeSlot(QString time) {
//     // labelTotal->setText(time);
//     // labelReset->setText(time);
// }

void MainWindow::dataHandler(const QByteArray &info) {
    qDebug() << "Processing info:" << info;
    switch (info[0]) {
        case 'S':
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
            if(timeData[0]=='R'){
                //qDebug()<<timeData;
                timeData=timeData.mid(1).trimmed();
                QString timeString = QString::fromUtf8(timeData);
                labelReset->setText(timeString);
            }
            else if(timeData[0]=='T'){
                //qDebug()<<timeData;
                timeData=timeData.mid(1).trimmed();
                QString timeString = QString::fromUtf8(timeData);
                labelTotal->setText(timeString);
            }
        }
        case 'E':{
            if(info[1]=='T'){
                qDebug()<<"Time out!";
            }
            else if(info[1]=='F'){
                qDebug()<<"Opponent time out";
            }
        }
        default:
            board->processBoardInfo(info);
    }
}

// Process boardInfo in this function.
void QtBoard::processBoardInfo(const QByteArray &boardInfo) {
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
    // Repaint the board.
    repaint();
}

// Translate the coordinates.
std::pair<int, int> translateIdx(const unsigned int x, const unsigned int y) {
    return std::make_pair(DELTA_X + x * cellSize + cellSize / 2, DELTA_Y + y * cellSize + cellSize / 2);
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

    auto emphasize = [&](const int col, const int row, auto color) {
        const auto [centerX, centerY] = translateIdx(col, row);
        const QRect rect(centerX - chessRadius, centerY - chessRadius, 2 * chessRadius, 2 * chessRadius);
        painter.setPen(QPen(color, selectedSize));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(rect);
    };

    // Show the chess at clicked.
    if (selectedPieceRow != -1 && selectedPieceCol != -1) {
        emphasize(selectedPieceCol, selectedPieceRow, SELECTED_COLOR);
    }

    // Show the eatable
    for (const auto &elem: eatable) {
        emphasize(elem.first.second, elem.first.first, EATABLE_COLOR);
        for (const auto &item: elem.second) {
            emphasize(item.second, item.first, EATABLE_PATH_COLOR);
        }
    }

    // Show the movable
    for (const auto &elem: movable) {
        emphasize(elem.second, elem.first, MOVABLE_COLOR);
    }

    drawChess();
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
                emit sendEatableQuery(std::make_pair(col, row));
                emit sendMovableQuery(std::make_pair(col, row));
                repaint();
            } else {
                if (selectedPieceRow != -1 && selectedPieceCol != -1) {
                    QString moveInfo = QString("$M%1;%2;%3;%4")
                            .arg(selectedPieceCol).arg(selectedPieceRow).arg(col).arg(row);
                    emit sendMoveInfo(moveInfo.toUtf8());
                    qDebug() << moveInfo;
                    selectedPieceRow = selectedPieceCol = -1;
                    movable.clear();
                    eatable.clear();
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
            generatedPath.pop_back();
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
        if (chessColor[row][col] == current_player) {
            const auto [centerX, centerY] = translateIdx(col, row);
            if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                QApplication::setOverrideCursor(Qt::PointingHandCursor);
                return;
            }
        }
    }
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}
