#ifndef QTBOARD_H
#define QTBOARD_H
#include<QWidget>
enum ChessColor{
    WHITE,
    BLACK,
    NONE
};
class QtBoard: public QWidget
{
    Q_OBJECT
 public:
   explicit QtBoard(QWidget *parent=nullptr);
    ~QtBoard();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * event);
    bool IsBlackChess(ChessColor color);
    void IsDead();
    void InitBoard();
    void processBoardInfo(const QByteArray &boardInfo);
    void drawChess();

 private:
    ChessColor chessColor[6][6];
    int d=40;//棋子直径 or 9/10の棋盘直径
    int x=280;//增大整个图形向右动
    int y=120;//增大整个图形向下动
    int turn=0;
    double k=(double)10/9;
    //注意，保证了棋盘间距比棋子略大
    QPoint stPos,edPos;
private slots:

};
#endif // QTBOARD_H
