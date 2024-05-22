// In this file, we define the board used in replay.
#ifndef REPLAY_BOARD_H
#define REPLAY_BOARD_H

#include <QPainter>
#include "common.h"

class ReplayBoard : public QWidget {
Q_OBJECT

public:
    explicit ReplayBoard(QWidget *parent) {
        for (unsigned int i = 0; i < BOARD_SIZE; i += 1) {
            for (unsigned int j = 0; j < BOARD_SIZE; j += 1) {
                chessColor[i][j] = NONE;
            }
        }
    }

    std::pair<int, int> translateIdx(const unsigned int x, const unsigned int y) {
        return std::make_pair(DELTA_X + x * cellSize + cellSize / 2, DELTA_Y + y * cellSize + cellSize / 2);
    }

    void paintEvent(QPaintEvent *) override {
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
        drawArcs(painter, DELTA_X, DELTA_Y, 0, BOARD_SIZE / 2);
        drawArcs(painter, DELTA_X + BOARD_HEIGHT, DELTA_Y, 270 * 16, BOARD_SIZE / 2);
        drawArcs(painter, DELTA_X, DELTA_Y + BOARD_HEIGHT, 90 * 16, BOARD_SIZE / 2);
        drawArcs(painter, DELTA_X + BOARD_HEIGHT, DELTA_Y + BOARD_HEIGHT, 180 * 16, BOARD_SIZE / 2);
        drawChess();
    }

    inline void processBoardInfo(const QByteArray &boardInfo) {
        QString boardInfoStr = QString::fromUtf8(boardInfo);
        QStringList rows = boardInfoStr.split('|');
        for (int row = 0; row < BOARD_SIZE; row += 1) {
            QString rowString = rows.value(row).trimmed();
            rowString.remove(' ');
            QTextStream stream(&rowString);
            for (int col = 0; col < BOARD_SIZE; col += 1) {
                QChar pieceChar;
                stream >> pieceChar;
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
        repaint();
    }

    void drawChess() {
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

private:
    ChessColor chessColor[BOARD_SIZE][BOARD_SIZE]{};
};

#endif //REPLAY_BOARD_H
