#ifndef QTBOARD_H
#define QTBOARD_H

#include<QWidget>

enum ChessColor {
    WHITE,
    BLACK,
    NONE
};

class QtBoard : public QWidget {
Q_OBJECT

public:
    explicit QtBoard(QWidget *parent = nullptr);

    ~QtBoard();

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);

    bool IsBlackChess(ChessColor color);

    void IsDead();

    void InitBoard();

    void processBoardInfo(const QByteArray &boardInfo);

    void drawChess();

    bool eventFilter(QObject *obj, QEvent *event) override;

    ChessColor checkTurn();

signals:

    void moveInfoReady(const QString &moveInfo);

private:
    ChessColor chessColor[6][6];
    int d = 60;//棋子直径 or 9/10の棋盘直径
    int x = 280;//增大整个图形向右动
    int y = 140;//增大整个图形向下动
    int turn = 0;
    int begin = 0;
    double k = (double) 10 / 9;
    //注意，保证了棋盘间距比棋子略大
    int selectedPieceRow = -1;
    int selectedPieceCol = -1;
    int borderSize = 2;
    int borderWidth = 5;
    QPoint stPos, edPos;
private slots:

};

#endif // QTBOARD_H
