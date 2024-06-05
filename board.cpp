#include "board.h"
#include <Qpainter>
#include <QMouseEvent>
#include <iostream>
#include <QTextStream>
#include <QString>
#include <QCursor>
#include <QMenuBar>
#include <QStatusBar>
#include <QThread>
#include <QDir>
#include <QTimer>
#include <QApplication>

// initialize the board
QtBoard::QtBoard(QWidget *parent) : QWidget(parent) {
    this->installEventFilter(this);
    setMouseTracking(true);
    for (unsigned int i = 0; i < BOARD_SIZE; i += 1) {
        for (unsigned int j = 0; j < BOARD_SIZE; j += 1) {
            chessColor[i][j] = NONE;
        }
    }
}

// give the control to AI
void QtBoard::setHandledByAI(bool enabled) {
    handledByAI = enabled;
    emit sendAIQuery(handledByAI);
    qDebug() << "handledByAI = " << handledByAI;
}

// Filter mouse to fit the UI
bool QtBoard::eventFilter(QObject *obj, QEvent *event) {
    // If the event is a mouse press.
    if (event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            // Change the type of the cursor.
            setCursor(Qt::ArrowCursor);
        }
    }
    return QWidget::eventFilter(obj, event);
}

// Set the current player.
void QtBoard::setCurrentPlayer(ChessColor cur) {
    current_player = cur;
}

// Receive data from user.
void QtBoard::receiveDataFromUser(int value, char piece_color) {
    if (value != 0) {
        qDebug() << "BOARD_SIZE = " << value;
    }
    if (piece_color != 0) {
        qDebug() << "piece_color = " << piece_color;
    }
}

// Process boardInfo in this function.
void QtBoard::processBoardInfo(const QByteArray &boardInfo) {
    gameCount += 1;
    QString boardInfoStr = QString::fromUtf8(boardInfo);
    QStringList rows = boardInfoStr.split('|');
    for (int row = 0; row < BOARD_SIZE; row += 1) {
        // process data
        QString rowString = rows.value(row).trimmed();
        rowString.remove(' ');
        QTextStream stream(&rowString);
        for (int col = 0; col < BOARD_SIZE; col += 1) {
            QChar pieceChar;
            stream >> pieceChar;
            // Set the color of the chess.
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
    // Animation & repaint
    if (shouldCheckAnimation) {
        animateMove();
        shouldCheckAnimation = false;
    } else {
        repaint();
    }
}

// show the animation of move
void QtBoard::animateMove() {
    bool isEatableFound = false;
    currentPath = nullptr;
    if (eatable.empty()) {
        repaint();
        return;
    }
    // cannot find the path
    if (currentPath == nullptr) {
        for (const auto &elem: eatable) {
            const auto &target = elem.first;
            const auto pieceColor = chessColor[target.first][target.second];
            // check piece
            if (pieceColor == current_player) {
                lastRow = fromRow;
                lastCol = fromCol;
                currentPath = &(elem.second);
                currentPathIndex = 0;
                isEatableFound = true;
                chessColor[target.first][target.second] = (pieceColor == WHITE) ? BLACK : WHITE;
                break;
            }
        }
    }
    // if found, record available path
    if (currentPath != nullptr || isEatableFound) {
        inAnimation = true;
        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, &QtBoard::animationStep);
        animationTimer->setInterval(ANIMATION_PER_TIME / ANIMATION_STEP);
        animationTimer->start();
    } else {
        repaint();
    }
}

// Translate the coordinates.
std::pair<int, int> translateIdx(const unsigned int x, const unsigned int y) {
    return std::make_pair(DELTA_X + x * cellSize + cellSize / 2, DELTA_Y + y * cellSize + cellSize / 2);
}

// calculate the rotation
auto calculateRotation = [](auto step, auto row, auto col) {
    int centerX, centerY, layer;
    double angle = step * 1.0 / ANIMATION_STEP * 270;
    const int x = std::abs(row - col), n = BOARD_SIZE;
    // 1 = anticlockwise
    if (row == n - 1 && col < n / 2 || col == 0 && row >= n / 2) {
        centerX = DELTA_X;
        centerY = DELTA_Y + BOARD_HEIGHT;
        layer = n - x;
        // anticlockwise
        if (row != n - 1) {
            angle = 90 + angle;
        } else {
            angle = -angle;
        }
    }
    if (row == n - 1 && col >= n / 2 || col == n - 1 && row >= n / 2) {
        centerX = DELTA_X + BOARD_HEIGHT;
        centerY = DELTA_Y + BOARD_HEIGHT;
        layer = x + 1;
        // anticlockwise
        if (row == n - 1) {
            angle = 180 + angle;
        } else {
            angle = 90 - angle;
        }
    }
    if (row == 0 && col < n / 2 || col == 0 && row < n / 2) {
        centerX = DELTA_X;
        centerY = DELTA_Y;
        layer = x + 1;
        // anticlockwise
        if (row == 0) {
            angle = angle;
        } else {
            angle = 270 - angle;
        }
    }
    if (row == 0 && col >= n / 2 || col == n - 1 && row < n / 2) {
        centerX = DELTA_X + BOARD_HEIGHT;
        centerY = DELTA_Y;
        layer = n - x;
        // anticlockwise
        if (row != 0) {
            angle = 270 + angle;
        } else {
            angle = 180 - angle;
        }
    }
    const double radius = cellSize * layer - cellSize * 1.0 / 2;
    centerX = centerX + static_cast<int>(radius * std::cos(angle * M_PI / 180));
    centerY = centerY - static_cast<int>(radius * std::sin(angle * M_PI / 180));
    return std::make_pair(centerX, centerY);
};

// calculate animation step
void QtBoard::animationStep() {
    if (currentPathIndex < currentPath->size()) {
        if (lastRow != -1 && lastCol != -1) {
            chessColor[lastRow][lastCol] = NONE;
        }
        const auto [row, col] = (*currentPath)[currentPathIndex];
        // calculate border conditions
        if ((row - lastRow) * (col - lastCol) == 0) {
            const auto [u, v] = translateIdx(lastCol, lastRow);
            const auto [s, t] = translateIdx(col, row);
            extraX = u + static_cast<int>((s - u) * currentStep * 1.0 / ANIMATION_STEP);
            extraY = v + static_cast<int>((t - v) * currentStep * 1.0 / ANIMATION_STEP);
            repaint();
            extraX = -1;
            extraY = -1;
            currentStep += 10;
        } else {
            const auto [centerX, centerY] = calculateRotation(currentStep, lastRow, lastCol);
            extraX = centerX;
            extraY = centerY;
            repaint();
            extraX = -1;
            extraY = -1;
            currentStep += 1;
        }
        if (currentStep > ANIMATION_STEP) {
            chessColor[row][col] = current_player;
            lastRow = row;
            lastCol = col;
            currentPathIndex += 1;
            currentStep = 1;
        }
    } else {
        animationTimer->stop();
        disconnect(animationTimer, &QTimer::timeout, this, &QtBoard::animationStep);
        currentPathIndex = 0;
        currentPath = nullptr;
        eatable.clear();
        inAnimation = false;
        currentStep = 1;
        repaint();
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

void QtBoard::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), BACK_COLOR);
    // highlight the path
    if (inAnimation) {
        highlightPath(painter);
    }

    // Draw k = size / 2 layers of arc, centered at centerX, centerY.
    painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH));
    for (int i = 0; i < BOARD_SIZE; i += 1) {
        for (int j = 0; j < BOARD_SIZE; j += 1) {
            painter.save();
            painter.drawRect(DELTA_X + i * cellSize, DELTA_Y + j * cellSize, cellSize, cellSize);
            painter.restore();
        }
    }
    drawArcs(painter, DELTA_X, DELTA_Y, 0, BOARD_SIZE / 2);
    drawArcs(painter, DELTA_X + BOARD_HEIGHT, DELTA_Y, 270 * 16, BOARD_SIZE / 2);
    drawArcs(painter, DELTA_X, DELTA_Y + BOARD_HEIGHT, 90 * 16, BOARD_SIZE / 2);
    drawArcs(painter, DELTA_X + BOARD_HEIGHT, DELTA_Y + BOARD_HEIGHT, 180 * 16, BOARD_SIZE / 2);

    // define emphasize function
    auto emphasize = [&](const int col, const int row, auto color, auto line, auto size) {
        const auto [centerX, centerY] = translateIdx(col, row);
        const QRect rect(centerX - chessRadius, centerY - chessRadius, 2 * chessRadius, 2 * chessRadius);
        painter.setPen(QPen(color, size, line));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(rect);
    };

    // Show the chess at clicked.
    if (selectedPieceRow != -1 && selectedPieceCol != -1) {
        emphasize(selectedPieceCol, selectedPieceRow, SELECTED_COLOR, selectedLine, selectedSize);
        // Show the eatable
        for (const auto &elem: eatable) {
            emphasize(elem.first.second, elem.first.first, EATABLE_COLOR, selectedLine, selectedSize);
        }
        // Show the movable
        for (const auto &elem: movable) {
            emphasize(elem.second, elem.first, MOVABLE_COLOR, emphasizeLine, emphasizeSize);
        }
    }

    drawChess();
}

void QtBoard::highlightPath(QPainter &painter) {
    // function to paint path
    auto fillPath = [&](auto centerX, auto centerY, auto startAngle, auto layer) {
        int radius = layer * cellSize;
        constexpr int endAngle = 270 * 16;
        painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH, Qt::DashLine));
        painter.setBrush(PATH_COLOR);
        QRectF rect(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
        painter.drawPie(rect, startAngle, endAngle);
        radius -= cellSize;
        painter.setBrush(BACK_COLOR);
        rect = QRectF(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
        painter.drawPie(rect, startAngle, endAngle);
        drawArcs(painter, centerX, centerY, startAngle, layer - 1);
        painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(DELTA_X, DELTA_Y, BOARD_HEIGHT, BOARD_HEIGHT);
    };

    // function to detect neighbour
    auto isNeighbour = [](const int x1, const int y1, const int x2, const int y2) {
        return (x1 - x2) * (y1 - y2) == 0;
    };

    // function to highlight path
    auto emphasize = [&](const int col, const int row, auto color) {
        const auto [centerX, centerY] = translateIdx(col, row);
        const QRect rect(centerX - cellSize / 2, centerY - cellSize / 2, cellSize, cellSize);
        painter.fillRect(rect, PATH_COLOR);
    };

    // main function for path
    auto transfer = [&](const int col, const int row, const int futureCol, const int futureRow) {
        if (!isNeighbour(row, col, futureRow, futureCol)) {
            const int x = std::abs(row - col), n = BOARD_SIZE;
            if (row == n - 1 && col < n / 2 || col == 0 && row >= n / 2) {
                fillPath(DELTA_X, DELTA_Y + BOARD_HEIGHT, 90 * 16, n - x);
            }
            if (row == n - 1 && col >= n / 2 || col == n - 1 && row >= n / 2) {
                fillPath(DELTA_X + BOARD_HEIGHT, DELTA_Y + BOARD_HEIGHT, 180 * 16, x + 1);
            }
            if (row == 0 && col < n / 2 || col == 0 && row < n / 2) {
                fillPath(DELTA_X, DELTA_Y, 0, x + 1);
            }
            if (row == 0 && col >= n / 2 || col == n - 1 && row < n / 2) {
                fillPath(DELTA_X + BOARD_HEIGHT, DELTA_Y, 270 * 16, n - x);
            }
        }
    };

    const auto size = currentPath->size();
    emphasize(fromCol, fromRow, PATH_COLOR);
    emphasize((*currentPath)[size - 1].second, (*currentPath)[size - 1].first, PATH_COLOR);
    transfer(fromCol, fromRow, (*currentPath)[0].second, (*currentPath)[0].first);

    // paint
    for (int i = 0; i < size - 1; i += 1) {
        const auto [row, col] = (*currentPath)[i];
        const auto [futureRow, futureCol] = (*currentPath)[i + 1];
        transfer(col, row, futureCol, futureRow);
        emphasize(col, row, PATH_COLOR);
    }
}

// Draw all the chess.
void QtBoard::drawChess() {
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
    if (extraX != -1 && extraY != -1) {
        const auto currentColor = (current_player == BLACK) ? Qt::black : Qt::white;
        painter.setPen(QPen(CHESS_BORDER, PEN_WIDTH));
        painter.setBrush(QBrush(currentColor, Qt::SolidPattern));
        const QRect rect(extraX - chessRadius, extraY - chessRadius, 2 * chessRadius, 2 * chessRadius);
        painter.drawEllipse(rect);
    }
}

// Handle mouse press event.
void QtBoard::mousePressEvent(QMouseEvent *event) {
    QPoint mousePos = event->pos();
    if ((gameCount % 2 == 0) ^ (current_player != BLACK)) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        bool canClick = false;
        const int x = mousePos.x(), y = mousePos.y();
        const int dx = x - DELTA_X, dy = y - DELTA_Y;

        // calculate neighbour for moving and eating
        if (0 <= dx && dx <= BOARD_HEIGHT && 0 <= dy && dy <= BOARD_HEIGHT) {
            const int row = dy / cellSize, col = dx / cellSize;
            const auto [centerX, centerY] = translateIdx(col, row);
            if (chessColor[row][col] == current_player) {
                if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                    canClick = true;
                }
            }
            for (const auto &elem: eatable) {
                if (elem.first.first == row && elem.first.second == col) {
                    if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                        canClick = true;
                        break;
                    }
                }
            }
            for (const auto &elem: movable) {
                if (elem.first == row && elem.second == col) {
                    if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                        canClick = true;
                        break;
                    }
                }
            }
        }

        // calculate next state
        if (canClick) {
            const int row = (mousePos.y() - DELTA_Y) / cellSize;
            const int col = (mousePos.x() - DELTA_X) / cellSize;
            if (chessColor[row][col] == current_player) {
                selectedPieceRow = row;
                selectedPieceCol = col;
                eatable.clear();
                movable.clear();
                emit sendEatableQuery(std::make_pair(col, row));
                emit sendMovableQuery(std::make_pair(col, row));
                repaint();
            } else {
                if (selectedPieceRow != -1 && selectedPieceCol != -1) {
                    QString moveInfo = QString("$M%1;%2;%3;%4")
                            .arg(selectedPieceCol)
                            .arg(selectedPieceRow)
                            .arg(col)
                            .arg(row);
                    fromRow = selectedPieceRow;
                    fromCol = selectedPieceCol;
                    selectedPieceRow = selectedPieceCol = -1;
                    shouldCheckAnimation = true;
                    emit sendMoveInfo(moveInfo.toUtf8());
                    repaint();
                }
            }
        }
    }
}

// process data for movable
void QtBoard::handleMovable(const QByteArray &info) {
    movable.clear();
    auto startIdx = info.indexOf("|");
    int n = info.left(startIdx).toInt();
    if (n != 0) {
        QString coords = info.mid(startIdx + 1);
        QStringList pairs = coords.split("|");
        for (const QString &pair: pairs) {
            QStringList values = pair.split(";");
            int x = values[0].toInt(), y = values[1].toInt();
            movable.emplace_back(x, y);
        }
    }
    repaint();
}

// process data for eatable
void QtBoard::handleEatable(const QByteArray &info) {
    // example:
    // 2|6;7@2;1@2;2@2;3@2;4@2;5@2;6@2;7@2;8@2;9@0;7@1;7@2;7@3;7@4;7@5;7@6;7|8;2@0;2@1;2@2;2@3;2@4;2@5;2@6;2@7;2@8;2
    eatable.clear();
    auto startIdx = info.indexOf("|");
    int n = info.left(startIdx).toInt();

    // record the path
    if (n != 0) {
        QString coords = info.mid(startIdx + 1);
        QStringList paths = coords.split("|");
        for (const QString &path: paths) {
            QStringList pairs = path.split("@");
            bool started = false;
            posType target;
            std::vector<posType> generatedPath;
            for (const QString &pair: pairs) {
                auto unzip = [&]() {
                    QStringList values = pair.split(";");
                    return std::make_pair(values[0].toInt(), values[1].toInt());
                };
                if (!started) {
                    target = unzip();
                    started = true;
                } else {
                    generatedPath.push_back(unzip());
                }
            }
            eatable.emplace_back(target, generatedPath);
        }
    }
    repaint();
}

// Handle move mouse event.
// override the cursor to make UI suitable
void QtBoard::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    QPoint mousePos = event->pos();
    if ((gameCount % 2 == 0) ^ (current_player != BLACK)) {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        return;
    }
    // Translate the coordinates.
    const int x = mousePos.x(), y = mousePos.y();
    const int dx = x - DELTA_X, dy = y - DELTA_Y;
    if (0 <= dx && dx <= BOARD_HEIGHT && 0 <= dy && dy <= BOARD_HEIGHT) {
        const int row = dy / cellSize, col = dx / cellSize;
        const auto [centerX, centerY] = translateIdx(col, row);
        if (chessColor[row][col] == current_player) {
            if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                QApplication::setOverrideCursor(Qt::PointingHandCursor);
                return;
            }
        }
        for (const auto &elem: eatable) {
            if (elem.first.first == row && elem.first.second == col) {
                if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
                    return;
                }
            }
        }
        for (const auto &elem: movable) {
            if (elem.first == row && elem.second == col) {
                if (std::pow(centerX - x, 2) + std::pow(centerY - y, 2) <= std::pow(chessRadius, 2)) {
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
                    return;
                }
            }
        }
    }
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}
