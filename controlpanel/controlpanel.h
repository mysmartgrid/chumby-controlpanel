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
#include "audioplugin.h"
#include "musiccontrol.h"
#include "alarmdaemon.h"

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

    signals:
        void keyPressed();

    private:
        QVBoxLayout* layout;
        QHBoxLayout* header;
        QLabel* clock;
        QLabel* wifi;
        QListWidget* list;
        QPushButton* button;
        QStackedWidget* stack;
        QMap< QString, QPair<QIcon*, DLLFactory<PluginFactory>* > > plugins;
        Plugin* currentPlugin;

        int sock_iwconfig;

        MusicControl* mc;
        AlarmDaemon* alarm;
    };
}

#endif // CONTROLPANEL_H
