#include "chumbykbdhandler.h"

ChumbyKbdHandler::ChumbyKbdHandler(const QString &device)
{
    qDebug() << "Loaded Chumby keyboard plugin!";
    setObjectName("Chumby Keyboard Handler");
    kbdFd1 = ::open("/dev/input/event0", O_RDONLY, 0);
    if (kbdFd1 >= 0 )
    {
        qDebug() << "Opened " << device << "(" << kbdFd1 << ") as keyboard input";
        m_notify = new QSocketNotifier(kbdFd1, QSocketNotifier::Read, this);
        m_notify->setEnabled(true);
        connect(m_notify, SIGNAL(activated(int)), this, SLOT(readKbdData()));
    } else {
        qWarning("Cannot open %s for keyboard input (%s)",
                 device.toLocal8Bit().constData(), strerror(errno));
        return;
    }
    kbdFd2 = ::open("/dev/input/event2", O_RDONLY, 0);
    if (kbdFd2 >= 0 )
    {
        qDebug() << "Opened " << device << "(" << kbdFd2 << ") as keyboard input";
        m_notify2 = new QSocketNotifier(kbdFd2, QSocketNotifier::Read, this);
        m_notify2->setEnabled(true);
        connect(m_notify2, SIGNAL(activated(int)), this, SLOT(readWheelData()));
    } else {
        qWarning("Cannot open %s for keyboard input (%s)",
                 device.toLocal8Bit().constData(), strerror(errno));
        return;
    }
    shift = false;
}

ChumbyKbdHandler::~ChumbyKbdHandler()
{
    qDebug() << "Destroying Chumby keyboard plugin!";
    m_notify->setEnabled(false);
    if (kbdFd1 >= 0)
        ::close(kbdFd1);
}

void ChumbyKbdHandler::readKbdData()
{
    ExampleInput event;
    int n = read(kbdFd1, &event, sizeof(ExampleInput));
    if (n != 16) {
        qDebug() << "keypressed: n=" << n;
        return;
    }
    qDebug() << "keypressed: type=" << event.type
              << "code=" << event.code
              << "value=" << event.value
              << ((event.value != 0) ? "(Down)" : "(Up)");
    int unicode = 0xffff;
    int key_code = event.code;
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;

    processKeyEvent(unicode, key_code, modifiers, event.value!=0, false);
}

void ChumbyKbdHandler::readWheelData()
{
    ExampleInput event;
    int n = read(kbdFd2, &event, sizeof(ExampleInput));
    if (n != 16) {
        qDebug() << "keypressed: n=" << n;
        return;
    }
    qDebug() << "keypressed: type=" << event.type
              << "code=" << event.code
              << "value=" << event.value
              << ((event.value != 0) ? "(Down)" : "(Up)");
    int unicode = 0xffff;
    unsigned int rep = 1;
    if ( event.code == 8 )
    {
        if ( event.value > 2147483648UL )
        {
            event.code = 9;
            rep = 4294967295UL - event.value;
        }
        else
        {
            rep = event.value - 1;
        }
    }
    int key_code = event.code;
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;


    for ( unsigned int i = 0; i <= rep; i++ )
        processKeyEvent(unicode, key_code, modifiers, event.value!=0, false);
}
