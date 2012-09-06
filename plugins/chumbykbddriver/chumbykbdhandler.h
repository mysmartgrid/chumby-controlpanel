#ifndef CHUMBYKBDHANDLER_H
#define CHUMBYKBDHANDLER_H

#include <QWSKeyboardHandler>
#include <QDebug>
#include <QSocketNotifier>

#include <fcntl.h>
#include <errno.h>

struct ExampleInput {
    unsigned int   dummy1;
    unsigned int   dummy2;
    unsigned short type;
    unsigned short code;
    unsigned int   value;
};

class ChumbyKbdHandler : public QObject, public QWSKeyboardHandler
{
    Q_OBJECT
public:
    ChumbyKbdHandler(const QString &device = QString("/dev/input/event0"));
    ~ChumbyKbdHandler();
private:
    QSocketNotifier *m_notify, *m_notify2;
    int kbdFd1, kbdFd2;
    bool shift;
private Q_SLOTS:
    void readKbdData();
    void readWheelData();
};

#endif // CHUMBYKBDHANDLER_H
