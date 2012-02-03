#include <QtGui/QApplication>
#include <QWSEvent>
#include "controlpanel.h"

class myApplication : public QApplication
{
    bool qwsEventFilter(QWSEvent* event)
    {
        if (event->type == QWSEvent::Key )
        {
            std::cout << ((QWSKeyEvent*) event)->Key << std::endl;
        }
        return false;
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOverrideCursor( QCursor( Qt::BlankCursor ) );

    Msg::Controlpanel w;
    w.showFullScreen();

    return a.exec();
}

