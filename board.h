// In this file, we define the QtBoard class.
#ifndef BOARD_H
#define BOARD_H

#include "common.h"

class QtBoard : public QWidget {
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
    void sendMoveInfo(const QByteArray &moveInfo);
    void sendEatableQuery(const posType &pos);
    void sendMovableQuery(const posType &pos);
    void sendAIQuery(bool isAI);

public slots:
    void receiveDataFromUser(int value, char piece_color);
    void setHandledByAI(bool enabled);
    void animateMove();
    void handleEatable(const QByteArray &);
    void handleMovable(const QByteArray &);

private:
    ChessColor chessColor[BOARD_SIZE][BOARD_SIZE];
    ChessColor current_player;
    std::vector<posType> movable;
    std::vector<std::pair<posType, std::vector<posType>>> eatable;
    QTimer *animationTimer;
    int selectedPieceRow = -1;
    int selectedPieceCol = -1;
    int currentPathIndex;
    int lastRow, lastCol, fromRow, fromCol;
    const std::vector<posType> *currentPath;
    bool handledByAI = false, inAnimation = false;
    bool shouldCheckAnimation = false;
    int extraX = -1, extraY = -1;
    int currentStep, gameCount = 0;
};

#endif //BOARD_H
