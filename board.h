// In this file, we define the QtBoard class.
// All the fuctions are named according to their functionalities.
// Only special functions will we give explanations.
#ifndef BOARD_H
#define BOARD_H

#include "common.h"

class QtBoard : public QWidget
{
    Q_OBJECT

public:
    explicit QtBoard(QWidget *parent);
    void setCurrentPlayer(ChessColor cur);
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void processBoardInfo(const QByteArray &boardInfo);
    void drawChess();
    bool eventFilter(QObject *obj, QEvent *event) override;
    void animationStep();
    void highlightPath(QPainter &painter);

signals:
    // These signals are used to send data
    void sendMoveInfo(const QByteArray &moveInfo);
    void sendEatableQuery(const posType &pos);
    void sendMovableQuery(const posType &pos);
    void sendAIQuery(bool isAI);

public slots:
    // These functions are used to receive data and process data
    void receiveDataFromUser(int value, char piece_color);
    void setHandledByAI(bool enabled);
    void animateMove();
    void handleEatable(const QByteArray &);
    void handleMovable(const QByteArray &);

private:
    // these are parameters for the board and boardinfo
    ChessColor chessColor[BOARD_SIZE][BOARD_SIZE];
    ChessColor current_player;

    // movable is a vector of positions
    std::vector<posType> movable;

    // eatable is a vector of pairs
    // each pair contains a position and a vector recording the eatable path
    std::vector<std::pair<posType, std::vector<posType>>> eatable;

    // We use a timer to control the animation painting
    QTimer *animationTimer;

    // Use to determine the selected piece and path
    int selectedPieceRow = -1;
    int selectedPieceCol = -1;
    int currentPathIndex;
    int lastRow, lastCol, fromRow, fromCol;

    // We use a vector to store the path of positions
    const std::vector<posType> *currentPath;

    // Bools for judgement
    // These are for replay animation and AI control
    bool handledByAI = false, inAnimation = false;
    bool shouldCheckAnimation = false;
    int extraX = -1, extraY = -1;
    int currentStep, gameCount = 0;
};

#endif // BOARD_H
