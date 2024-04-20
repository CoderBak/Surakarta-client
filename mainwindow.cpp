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
void MainWindow::sendData_mousePress(const QString &moveInfo) {
    QByteArray data;
    data.append(moveInfo.toUtf8());
    socket->write(data);
}

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        socket(new QTcpSocket(this)),
        board(new QtBoard(this)) {
    ui->setupUi(this);

    // Set the layout of the interface.
    auto *layout = new QHBoxLayout;
    auto *menu = new QVBoxLayout;

    // Add buttons to the menu.
    menu->addWidget(ui->lineEdit1);
    menu->addWidget(ui->lineEdit2);
    menu->addWidget(ui->sendButton);
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
    setStyleSheet(STYLE);

    // Connect the button to functions.
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendData);
    connect(board, &QtBoard::moveInfoReady, this, &MainWindow::sendData_mousePress);
    // connect(tryAgainButton, &QPushButton::clicked, this, &MainWindow::handleTryAgain);
    // connect(giveUpButton, &QPushButton::clicked, this, &MainWindow::handleGiveUp);
    // connect(openChatroomButton, &QPushButton::clicked, this, &MainWindow::handleOpenChatroom);

    // Connect the server.
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::getData);
    socket->connectToHost(serverIP, PORT);
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

// send data in this format: M3;2;2;3
void MainWindow::sendData() {
    QByteArray data;
    data.append("M");
    data.append(ui->lineEdit1->text().toUtf8());
    data.append(";");
    data.append(ui->lineEdit2->text().toUtf8());
    socket->write(data);
}

// Process data in this function.
void MainWindow::getData() {
    QByteArray data = socket->readAll();
    qDebug() << "Received message from server: " << data;
    switch (data[0]) {
        case 'S':
            qDebug() << "I'm player" << data[1];
            switch (data[1]) {
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
        default:
            board->processBoardInfo(data);
    }
    socket->read(socket->bytesAvailable());
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

    if (!started) {
        initBoard();
        started = true;
    }

    // Show the chess at clicked.
    if (selectedPieceRow != -1 && selectedPieceCol != -1) {
        const auto [centerX, centerY] = translateIdx(selectedPieceCol, selectedPieceRow);
        const QRect rect(centerX - chessRadius, centerY - chessRadius, 2 * chessRadius, 2 * chessRadius);
        painter.setPen(QPen(Qt::red, selectedSize));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(rect);
    }
    drawChess();
}

void QtBoard::initBoard() {
    for (unsigned int i = 0; i < BOARD_SIZE; i++) {
        for (unsigned int j = 0; j < BOARD_SIZE; j++) {
            if (i < 2) {
                chessColor[i][j] = BLACK;
            } else if (i > BOARD_SIZE - 3) {
                chessColor[i][j] = WHITE;
            } else {
                chessColor[i][j] = NONE;
            }
        }
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
                repaint();
            } else {
                if (selectedPieceRow != -1 && selectedPieceCol != -1) {
                    QString moveInfo = QString("M%1;%2;%3;%4")
                            .arg(selectedPieceCol).arg(selectedPieceRow)
                            .arg(col).arg(row);
                    std::cout << selectedPieceRow << "," << selectedPieceCol << ";" << row << "," << col << std::endl;
                    emit moveInfoReady(moveInfo);
                    selectedPieceRow = selectedPieceCol = -1;
                }
            }
        }
    }
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
