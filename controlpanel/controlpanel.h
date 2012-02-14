#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QtGui/QDialog>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>

#include <QtGui/QVBoxLayout>
#include <QtGui/QStackedWidget>

#include <QKeyEvent>

#include "dllmanager.h"
#include "plugin.h"
#include "musiccontrol.h"

namespace Msg
{
    class Controlpanel : public QDialog
    {
        Q_OBJECT

    public:
        Controlpanel(QWidget *parent = 0);
        ~Controlpanel();

    public slots:
        void startPlugin();
        void stopPlugin();
        void updateClock();

    protected:
        void getPlugins();
        void keyPressEvent(QKeyEvent *event);

    private:
        QVBoxLayout* layout;
        QHBoxLayout* header;
        QLabel* clock;
        QLabel* wifi;
        QListWidget* list;
        QPushButton* button;
        QStackedWidget* stack;
        QMap< QString, QPair<DLLFactory<PluginFactory>*, Plugin*> > plugins;

        int sock_iwconfig;

        MusicControl mc; //implicit construction here
    };
}

#endif // CONTROLPANEL_H
