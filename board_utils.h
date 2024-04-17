#ifndef QTBOARD_H
#define QTBOARD_H

#include<QWidget>

constexpr unsigned int BOARD_SIZE = 6;
constexpr unsigned int maxNoCapture = 40;
constexpr int WIDTH = 1180;
constexpr int HEIGHT = 820;
constexpr int BOARD_HEIGHT = 720 / 2;
constexpr int DELTA_Y = HEIGHT / 2 - BOARD_HEIGHT / 2 - 13 + BOARD_SIZE / 2;
constexpr int DELTA_X = 400;
constexpr int cellSize = BOARD_HEIGHT / BOARD_SIZE;
constexpr int PEN_WIDTH = 2;
constexpr auto PATH_COLOR = QColor(173, 216, 230);
constexpr auto DEFAULT_COLOR = Qt::black;
constexpr auto BACK_COLOR = Qt::yellow;
constexpr int chessRadius = static_cast<int>(0.35 * cellSize);
constexpr auto BLACK_COLOR = Qt::black;
constexpr auto WHITE_COLOR = Qt::white;
constexpr auto CHESS_BORDER = Qt::black;
constexpr int selectedSize = 5;
using posType = std::pair<unsigned int, unsigned int>;
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

signals:

    void moveInfoReady(const QString &moveInfo);

private:
    ChessColor chessColor[BOARD_SIZE][BOARD_SIZE];
    int begin = 0;
    int selectedPieceRow = -1;
    int selectedPieceCol = -1;
private slots:

};

#endif // QTBOARD_H
