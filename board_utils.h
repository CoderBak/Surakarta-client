#ifndef QTBOARD_H
#define QTBOARD_H

#include <QWidget>
#include <QHostAddress>

using posType = std::pair<int, int>;
constexpr unsigned int BOARD_SIZE = 6;
constexpr unsigned int maxNoCapture = 40;
constexpr int WIDTH = 1180;
constexpr int HEIGHT = 820;
constexpr int BOARD_HEIGHT = 720 / 2;
constexpr int DELTA_Y = HEIGHT / 2 - BOARD_HEIGHT / 2 - 10;
constexpr int DELTA_X = 400;
constexpr int cellSize = BOARD_HEIGHT / BOARD_SIZE;
constexpr int PEN_WIDTH = 2;
constexpr auto PATH_COLOR = QColor(173, 216, 230);
constexpr auto DEFAULT_COLOR = Qt::black;
constexpr auto BACK_COLOR = QColor(240, 255, 240);
constexpr int chessRadius = static_cast<int>(0.35 * cellSize);
constexpr auto BLACK_COLOR = Qt::black;
constexpr auto WHITE_COLOR = Qt::white;
constexpr auto CHESS_BORDER = Qt::black;
constexpr auto SELECTED_COLOR = Qt::red;
constexpr auto EATABLE_COLOR = Qt::blue;
constexpr auto MOVABLE_COLOR = Qt::green;
constexpr auto EATABLE_PATH_COLOR = Qt::yellow;
constexpr int selectedSize = 5;
constexpr int maxSecond = 1000;
const QHostAddress serverIP = QHostAddress("20.212.82.172");
constexpr int PORT = 1233;
// const QString STYLE = "background-color: white;";
enum ChessColor
{
    WHITE,
    BLACK,
    NONE
};

class QtBoard : public QWidget
{
    Q_OBJECT

public:
    void setCurrentPlayer(ChessColor cur);

    explicit QtBoard(QWidget *parent) : QWidget(parent)
    {
        this->installEventFilter(this);
        setMouseTracking(true);
        for (int i = 0; i < BOARD_SIZE; i += 1)
        {
            for (int j = 0; j < BOARD_SIZE; j += 1)
            {
                chessColor[i][j] = ChessColor::NONE;
            }
        }
    }

    ~QtBoard() override = default;

    void paintEvent(QPaintEvent *) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void processBoardInfo(const QByteArray &boardInfo);

    void drawChess();

    bool eventFilter(QObject *obj, QEvent *event) override;

    bool rotated;

signals:

    void sendMoveInfo(const QByteArray &moveInfo);

    void sendEatableQuery(const posType &pos);

    void sendMovableQuery(const posType &pos);

public slots:

    void handleEatable(const QByteArray &);

    void handleMovable(const QByteArray &);

private:
    ChessColor chessColor[BOARD_SIZE][BOARD_SIZE];
    ChessColor current_player;
    int selectedPieceRow = -1;
    int selectedPieceCol = -1;
    std::vector<posType> movable;
    std::vector<std::pair<posType, std::vector<posType>>> eatable;
};

#endif // QTBOARD_H
