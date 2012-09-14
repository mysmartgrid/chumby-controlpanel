#include <QtGui/QApplication>
#ifdef Q_WS_QWS
#  include <QtGui/QWSServer>
#  include <QWSEvent>
#endif /* Q_WS_QWS */
#include "controlpanel.h"

class myApplication : public QApplication
{
#ifdef Q_WS_QWS
	bool qwsEventFilter(QWSEvent* event)
    {
			if (event->type == QWSEvent::Key )
			{
				std::cout << ((QWSKeyEvent*) event)->Key << std::endl;
			}
			return false;
    }
#endif /* Q_WS_QWS */
};

int main(int argc, char *argv[])
{
#ifdef Q_WS_QWS
    QWSServer::setBackground(QBrush(Qt::white));
    QWSServer::setCursorVisible(false);
#endif /* Q_WS_QWS */
    QApplication a(argc, argv);
    a.setOverrideCursor( QCursor( Qt::BlankCursor ) );

    //QFont font("Arial", 14);
    //QApplication::setFont(font);

    Msg::Controlpanel w;
    w.showFullScreen();

    return a.exec();
}

