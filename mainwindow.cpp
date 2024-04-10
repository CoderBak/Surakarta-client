#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
<<<<<<< HEAD
#include <Qpainter>
#include <QMouseEvent>
#include<iostream>
#include<sstream>
#include<QTextStream>
#include<QString>
#include<QCursor>
// #include<QSoundEffect>
QtBoard::QtBoard(QWidget *parent):QWidget(parent){
    this->installEventFilter(this);
}
QtBoard::~QtBoard(){
}
bool QtBoard::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::MouseButtonPress) {
           QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
           if (mouseEvent->button() == Qt::LeftButton) {
               // 在此处更改鼠标指针形状
               QCursor cursor(Qt::PointingHandCursor); // 更改为您希望的指针形状
               setCursor(cursor);
           }
       }
       // 将事件传递给父类
       return QWidget::eventFilter(obj, event);
}
ChessColor QtBoard::checkTurn(){
    if(turn%2==0)return BLACK;
    else return WHITE;
}
=======

>>>>>>> a1e8519d8ffa131e5e3542ef31a0c239707fe97a
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        socket(new QTcpSocket(this))
{
    ui->setupUi(this);
<<<<<<< HEAD
    //setCentralWidget(board);
    setFixedSize(800,600);
    QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(board);
        QPushButton *tryAgainButton = new QPushButton("Try Again", this);
        QPushButton *giveUpButton = new QPushButton("Give Up", this);
        QPushButton *openChatroomButton = new QPushButton("Open Chatroom", this);

        layout->addWidget(tryAgainButton);
        layout->addWidget(giveUpButton);
        layout->addWidget(openChatroomButton);

        QWidget *centralWidget = new QWidget;
        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);

    connect(board,&QtBoard::moveInfoReady,this,&MainWindow::sendData);
=======
>>>>>>> a1e8519d8ffa131e5e3542ef31a0c239707fe97a
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendData() {
    // When perform move, the info format is : M4;5;3;4
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
    socket->read(socket->bytesAvailable());
<<<<<<< HEAD
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
            qDebug()<<pieceChar;
            char piece = pieceChar.toLatin1();
           std:: cout<<"attention:"<<piece<<std::endl;
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


void QtBoard::paintEvent(QPaintEvent *){
    QPainter painter(this);

    int dir_x[4]={0*16,90*16,270*16,180*16};
    for(int i=0;i<6;i++){
        painter.drawLine(QPoint(x,i*d*k+y),QPoint(x+5*d*k,i*d*k+y));
    }
    for(int i=0;i<6;i++){
        painter.drawLine(QPoint(x+i*d*k,y),QPoint(x+i*d*k,5*d*k+y));
    }
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            int m=x+i*5*d*k;
            int n=y+j*5*d*k;
            painter.drawArc(m-d*k,n-d*k,d*2*k,d*2*k,dir_x[i*2+j],270*16);
            painter.drawArc(m-2*d*k,n-2*d*k,d*4*k,d*4*k,dir_x[i*2+j],270*16);
        }
    }
    //最开始初始页面
    if(begin++==0)
    InitBoard();
    //点击后提供特效
    if (selectedPieceRow != -1 && selectedPieceCol != -1) {
           int centerX = stPos.x() + selectedPieceCol * d * k+x;
           int centerY = stPos.y() + selectedPieceRow * d * k+y;

           // 计算边框矩形的位置
           QRect borderRect(centerX - d / 2 - borderSize, centerY - d / 2 - borderSize, d + 2 * borderSize, d + 2 * borderSize);

           // 绘制半透明边框
           painter.setPen(QPen(Qt::red, borderWidth)); // 设置边框颜色和宽度
           painter.setBrush(Qt::NoBrush); // 不填充颜色，保持边框透明
           painter.drawEllipse(borderRect);
       }
           drawChess();
}

void QtBoard::InitBoard(){
   for(int i=0;i<6;i++){
       for(int j=0;j<6;j++){
           if(i<2)
           chessColor[i][j]=BLACK;
           else if(i>3)
           chessColor[i][j]=WHITE;
           else
           chessColor[i][j]=NONE;

       }
   }
}
void QtBoard::drawChess(){
    QPainter painter(this);

    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            if(chessColor[i][j]==BLACK){
                 painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
                 painter.drawEllipse(x+j*d*k-d/2,y+i*d*k-d/2,d,d);
            }
            else if(chessColor[i][j]==WHITE){
                painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
                painter.drawEllipse(x+j*d*k-d/2,y+i*d*k-d/2,d,d);
            }
        }
    }
}
void QtBoard::mousePressEvent(QMouseEvent* event){
    event->pos();
    if(event->button() == Qt::LeftButton){
        int row = (event->y() - stPos.y()+0.5*d-y) / (d * k);
        int col = (event->x() - stPos.x()+0.5*d-x) / (d * k);
        std::cout<<row<<","<<col<<std::endl;
        // 检查点击的位置是否在棋盘范围内
        if(row >= 0 && row < 6 && col >= 0 && col < 6){
            // 检查点击的位置是否有棋子
            if(chessColor[row][col] != NONE){
                // 存储点击的棋子位置
                selectedPieceRow = row;
                selectedPieceCol = col;
                repaint();

            } else {
                // 如果已经选择了棋子，则发送移动信息
                if(selectedPieceRow != -1 && selectedPieceCol != -1){
                    // 发送移动信息给服务器
                    QString moveInfo = QString("M%1;%2;%3;%4")
                                            .arg(selectedPieceCol)
                                            .arg(selectedPieceRow)
                                            .arg(col)
                                            .arg(row);
                    //QByteArray data = moveInfo.toUtf8();
                    //socket->write(data);
                  std::cout<<selectedPieceRow<<","<<selectedPieceCol<<";"<<row<<","<<col<<std::endl;
                    emit moveInfoReady(moveInfo);

                    // 清除已选择的棋子位置
                    selectedPieceRow = -1;
                    selectedPieceCol = -1;
                }
            }
        }
    }
}

=======
}
>>>>>>> a1e8519d8ffa131e5e3542ef31a0c239707fe97a
