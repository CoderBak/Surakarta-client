#ifndef PIECES_H
#define PIECES_H

#include <QPoint>
#include <QColor>

class ChessPiece {
public:
    ChessPiece(const QPoint &position, const QColor &color);

    // 获取棋子位置
    QPoint getPosition() const;

    // 获取棋子颜色
    QColor getColor() const;

    // 设置棋子位置
    void setPosition(const QPoint &position);

    // 设置棋子颜色
    void setColor(const QColor &color);

private:
    QPoint piece_position; // 棋子位置
    QColor piece_color;    // 棋子颜色
};


#endif // PIECES_H
