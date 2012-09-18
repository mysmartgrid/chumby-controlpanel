#include <QtGui/QApplication>
#include <QtGui/QWSServer>
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
#ifdef Q_WS_QWS
    QWSServer::setBackground(QBrush(Qt::white));
    QWSServer::setCursorVisible(false);
    a.setOverrideCursor( QCursor( Qt::BlankCursor ) );
#else
		a.setOverrideCursor( QCursor( Qt::CrossCursor ) );
#endif

    //QFont font("Arial", 14);
    //QApplication::setFont(font);

    Msg::Controlpanel w;
    w.showFullScreen();

    return a.exec();
}

