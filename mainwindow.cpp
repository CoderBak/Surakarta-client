#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "board_utils.h"
#include <QDebug>
#include <Qpainter>
#include <QMouseEvent>
#include <iostream>
#include <sstream>
#include <QTextStream>
#include <QString>
#include <QCursor>

// #include<QSoundEffect>
QtBoard::QtBoard(QWidget *parent) : QWidget(parent) {
    this->installEventFilter(this);
}

QtBoard::~QtBoard() {
}

bool QtBoard::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            // 在此处更改鼠标指针形状
            QCursor cursor(Qt::PointingHandCursor); // 更改为您希望的指针形状
            setCursor(cursor);
        }
    }
    // 将事件传递给父类
    return QWidget::eventFilter(obj, event);
}


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        socket(new QTcpSocket(this)),
        board(new QtBoard(this)) {
    ui->setupUi(this);
    //setCentralWidget(board);
    setFixedSize(1000, 900);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(board);
    // 创建三个按钮
    QPushButton *tryAgainButton = new QPushButton("Try Again", this);
    QPushButton *giveUpButton = new QPushButton("Give Up", this);
    QPushButton *openChatroomButton = new QPushButton("Open Chatroom", this);

    // 添加三个按钮到布局中
    layout->addWidget(tryAgainButton);
    layout->addWidget(giveUpButton);
    layout->addWidget(openChatroomButton);

    // 添加编辑框和发送按钮到布局中
    layout->addWidget(ui->lineEdit1);
    layout->addWidget(ui->lineEdit2);
    layout->addWidget(ui->pushButton);

    // 设置布局到中央部件
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(board, &QtBoard::moveInfoReady, this, &MainWindow::sendData_mousepress);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::getData);
    //  connect(tryAgainButton, &QPushButton::clicked, this, &MainWindow::handleTryAgain);
    //  connect(giveUpButton, &QPushButton::clicked, this, &MainWindow::handleGiveUp);
    // connect(openChatroomButton, &QPushButton::clicked, this, &MainWindow::handleOpenChatroom);
    socket->connectToHost("localhost", 1234);
    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect to host.";
        exit(-1);
    } else {
        qDebug() << "Connected successfully!";
    }
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::sendData);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::sendData_mousepress(const QString &moveInfo) {
    // When perform move, the info format is : M4;5;3;4
    QByteArray data;
    // data.append("M");
    // data.append(ui->lineEdit1->text().toUtf8());
    // data.append(";");
    // data.append(ui->lineEdit2->text().toUtf8());
    data.append(moveInfo.toUtf8());
    socket->write(data);
}

void MainWindow::sendData() {
    QByteArray data;
    data.append("M");
    data.append(ui->lineEdit1->text().toUtf8());
    data.append(";");
    data.append(ui->lineEdit2->text().toUtf8());
    socket->write(data);
}

void MainWindow::getData() {
    QByteArray data = socket->readAll();
    qDebug() << "Received message from server: " << data;
    board->processBoardInfo(data);
    socket->read(socket->bytesAvailable());
}

// void QtBoard::processBoardInfo(const QByteArray &boardInfo) {
//     // 将字节数组转换为字符串
//     QString boardInfoStr = QString::fromUtf8(boardInfo);

//     // 去除字符串两端的空白字符和引号
//     QString sanitizedInfo = boardInfoStr.trimmed();
//     sanitizedInfo.remove("\"");

//     // 分割字符串为行
//     QStringList rows = sanitizedInfo.split('\n');

//     // 遍历每一行数据
//     for (int row = 0; row < 6; ++row) {
//         QString rowString = rows.value(row).trimmed();
//     qDebug()<<rows.value(row).trimmed();
//         // 遍历每一列数据
//         for (int col = 0; col < 6; ++col) {
//             QChar pieceChar = rowString.at(col);
//             qDebug()<<pieceChar.toLatin1();
//             switch (pieceChar.toLatin1()) {
//                 case 'B':
//                     chessColor[col][row] = BLACK;
//                     break;
//                 case 'W':
//                     chessColor[col][row] = WHITE;
//                     break;
//                 case '.':
//                     chessColor[col][row] = NONE;
//                     break;
//                 default:
//                    // qDebug() << "Error: Invalid piece character!";
//                // qDebug()<<pieceChar.toLatin1();
//                 break;
//             }
//         }
//     }

//     // 打印二维数组，以便调试
//     for (int row = 0; row < 6; ++row) {
//         for (int col = 0; col < 6; ++col) {
//             std::cout << (chessColor[col][row] == BLACK ? "B" : (chessColor[col][row] == WHITE ? "W" : "."));
//         }
//      std::cout << std::endl;
//    }

//     // 重新绘制棋盘
//     repaint();
// }

void QtBoard::processBoardInfo(const QByteArray &boardInfo) {
    // 将字节数组转换为字符串
    QString boardInfoStr = QString::fromUtf8(boardInfo);

    // 分割字符串为行
    QStringList rows = boardInfoStr.split('\n');

    // 遍历每一行数据
    for (int row = 0; row < 6; ++row) {
        QString rowString = rows.value(row).trimmed();
        rowString.remove(' ');
        // 使用输入输出流读取数据并存储到二维数组中
        QTextStream stream(&rowString);
        for (int col = 0; col < 6; ++col) {
            QChar pieceChar;
            stream >> pieceChar;
            qDebug() << pieceChar;
            char piece = pieceChar.toLatin1();
            std::cout << "attention:" << piece << std::endl;
            // 检查当前字符是否是需要的字符类型，如果不是则跳过
            if (piece == 'B' || piece == 'W' || piece == '.') {
                switch (piece) {
                    case 'B':
                        chessColor[row][col] = BLACK;
                        break;
                    case 'W':
                        chessColor[row][col] = WHITE;
                        break;
                    case '.':
                        chessColor[row][col] = NONE;
                        break;
                }
            }
        }
    }

    // 打印二维数组，以便调试
    // for (int row = 0; row < 6; ++row) {
    //     for (int col = 0; col < 6; ++col) {
    //         std::cout << (chessColor[row][col] == BLACK ? "B" : (chessColor[row][col] == WHITE ? "W" : "."));
    //     }
    //     std::cout << std::endl;
    // }

    // 重新绘制棋盘
    repaint();
}

inline std::pair<int, int> translateIdx(const unsigned int x, const unsigned int y)
{
    return std::make_pair(DELTA_X + x * cellSize + cellSize / 2, DELTA_Y + y * cellSize + cellSize / 2);
}

inline auto drawArcs = [](QPainter &painter, auto centerX, auto centerY, auto startAngle, auto k) {
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
    // draw k = size / 2 layers of arc, centered at centerX, centerY.
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


    //最开始初始页面
    if (begin++ == 0)
        InitBoard();
    //点击后提供特效
    if (selectedPieceRow != -1 && selectedPieceCol != -1) {
        //std::cout<<"Clicked!"<<std::endl;
        const auto [centerX, centerY] = translateIdx(selectedPieceCol,selectedPieceRow);
        const QRect rect(centerX - chessRadius, centerY - chessRadius, 2 * chessRadius, 2 * chessRadius);

        // 绘制半透明边框
        painter.setPen(QPen(Qt::red, selectedSize)); // 设置边框颜色和宽度
        painter.setBrush(Qt::NoBrush); // 不填充颜色，保持边框透明
        painter.drawEllipse(rect);
    }

    drawChess();
}

void QtBoard::InitBoard() {
    for (unsigned int i = 0; i < BOARD_SIZE; i++) {
        for (unsigned int j = 0; j < BOARD_SIZE; j++) {
            if (i < 2)
                chessColor[i][j] = BLACK;
            else if (i > BOARD_SIZE-3)
                chessColor[i][j] = WHITE;
            else
                chessColor[i][j] = NONE;

        }
    }
}

void QtBoard::drawChess() {
    QPainter painter(this);

    for (unsigned int i = 0; i< BOARD_SIZE; i++) {
        for (unsigned int j = 0; j < BOARD_SIZE; j++) {
            if (chessColor[i][j] == BLACK) {
                painter.setPen(QPen(CHESS_BORDER, PEN_WIDTH));
                painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));     
                const auto [centerX, centerY] = translateIdx(j,i);
                const QRect rect(centerX - chessRadius, centerY - chessRadius, 2 * chessRadius, 2 * chessRadius);
                painter.drawEllipse(rect);
            } else if (chessColor[i][j] == WHITE) {
                painter.setPen(QPen(CHESS_BORDER, PEN_WIDTH));
                painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
                const auto [centerX, centerY] = translateIdx(j,i);
                const QRect rect(centerX - chessRadius, centerY - chessRadius, 2 * chessRadius, 2 * chessRadius);
                painter.drawEllipse(rect);
            }
        }
    }
}

void QtBoard::mousePressEvent(QMouseEvent *event) {
    event->pos();
    if (event->button() == Qt::LeftButton) {
        int row = (event->y()-DELTA_Y)/cellSize;
        int col = (event->x()-DELTA_X)/cellSize;

        std::cout << row << "," << col << std::endl;
        // 检查点击的位置是否在棋盘范围内
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {

            // 检查点击的位置是否有棋子
            if (chessColor[row][col] != NONE) {
                    selectedPieceRow = row;
                    selectedPieceCol = col;
                    repaint();

            } else {
                // 如果已经选择了棋子，则发送移动信息
                if (selectedPieceRow != -1 && selectedPieceCol != -1) {
                    // 发送移动信息给服务器
                    QString moveInfo = QString("M%1;%2;%3;%4")
                            .arg(selectedPieceCol)
                            .arg(selectedPieceRow)
                            .arg(col)
                            .arg(row);
                    //QByteArray data = moveInfo.toUtf8();
                    //socket->write(data);
                    std::cout << selectedPieceRow << "," << selectedPieceCol << ";" << row << "," << col << std::endl;
                    emit moveInfoReady(moveInfo);

                    // 清除已选择的棋子位置
                    selectedPieceRow = -1;
                    selectedPieceCol = -1;
                }
            }
        }
    }
}

