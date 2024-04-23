#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QTimer>

class TimerThread : public QThread {
    Q_OBJECT

public:
    TimerThread(QTcpSocket *socket,QObject *parent) : QThread(parent),m_socket(socket), m_running(true) {}
    void stop();

signals:
    void timeReceived(QString time);

protected:
    void run() override;

private:
    QTcpSocket *m_socket;
    bool m_running;
};

#endif // TIMERTHREAD_H
