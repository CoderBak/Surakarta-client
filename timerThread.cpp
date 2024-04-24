// #include "TimerThread.h"

// void TimerThread::run() {
//     int seconds = 0;
//     while (!isInterruptionRequested()) {
//         int hours = seconds / 3600;
//         int minutes = (seconds % 3600) / 60;
//         int secs = seconds % 60;

//         QString timeString = QString("%1:%2:%3")
//                                  .arg(hours, 2, 10, QChar('0'))
//                                  .arg(minutes, 2, 10, QChar('0'))
//                                  .arg(secs, 2, 10, QChar('0'));

//         emit updateTime(timeString);

//         sleep(1);
//         seconds++;
//     }
// }

#include "timerThread.h"
#include <QDateTime>

void TimerThread::run() {
    while (m_running) {
        if (m_socket->waitForReadyRead()) {
            QByteArray data = m_socket->readAll();
            if (data.startsWith("TIME:")) {
                QString time = QString::fromUtf8(data.mid(6)); // 解析时间信息
                // 发出时间信息的信号
                emit timeReceived(time);
            }
        }
    }
}

void TimerThread::stop() {
    m_running = false;
}
