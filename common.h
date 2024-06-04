// In this file, we define const.
// These const value are defined for both server and client.
// We use it to intialize some basic or unchanged things.
// We believe the name of const has already explained its meaning.
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <QWidget>
#include <QHostAddress>

using posType = std::pair<int, int>;

constexpr unsigned int BOARD_SIZE = 10;
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
constexpr auto BACK_COLOR = QColor(255, 255, 240);
constexpr int chessRadius = static_cast<int>(0.35 * cellSize);
constexpr auto CHESS_BORDER = Qt::black;
constexpr auto SELECTED_COLOR = Qt::red;
constexpr Qt::GlobalColor EATABLE_COLOR = Qt::green;
constexpr auto MOVABLE_COLOR = Qt::green;
constexpr auto ANIMATION_PER_TIME = 500;
constexpr auto ANIMATION_STEP = 100;
constexpr int selectedSize = 5;
constexpr int emphasizeSize = 3;
constexpr Qt::PenStyle selectedLine = Qt::SolidLine;
constexpr Qt::PenStyle emphasizeLine = Qt::DotLine;

constexpr int maxSecond = 1000;
const QHostAddress serverIP = QHostAddress("20.212.82.172");
constexpr int PORT = 1233;
constexpr int replaySpeed = 600;

enum ChessColor
{
    WHITE,
    BLACK,
    NONE
};

#endif // COMMON_H
