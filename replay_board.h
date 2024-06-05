// In this file, we define the board used in replay.
// Only special functions will we give explanations.
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

    // this function is used to facilitate the drawing of board and chess
    std::pair<int, int> translateIdx(const unsigned int x, const unsigned int y) {
        return std::make_pair(delta_x + x * cellSize + cellSize / 2, delta_y + y * cellSize + cellSize / 2);
    }

    // paint the board and chess
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.fillRect(rect(), BACK_COLOR);
        // Draw k = size / 2 layers of arc, centered at centerX, centerY.
        painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH));
        for (int i = 0; i < BOARD_SIZE; i += 1) {
            for (int j = 0; j < BOARD_SIZE; j += 1) {
                painter.save();
                painter.drawRect(delta_x + i * cellSize, delta_y + j * cellSize, cellSize, cellSize);
                painter.restore();
            }
        }
        drawChess();
    }

    // process the board information
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

    // draw the chess
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
    ChessColor chessColor[MAX_BOARD_SIZE][MAX_BOARD_SIZE]{};
    const int delta_x = 20, delta_y = 20;
};

#endif // REPLAY_BOARD_H
