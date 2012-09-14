#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QtGui/QDialog>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>

#include <QtGui/QVBoxLayout>
#include <QtGui/QStackedWidget>

#include <QKeyEvent>

#include "ui_controlpanel.h"
#include "dllmanager.h"
#include "plugin.h"
#include "audioplugin.h"
#include "musiccontrol.h"
#include "alarmdaemon.h"

namespace Msg
{
class Controlpanel : public QWidget
{
	Q_OBJECT

public:
	Controlpanel(QWidget *parent = 0);
	~Controlpanel();

public slots:
	void startPlugin();
	void stopPlugin();
	void updateClock();
	void showWidget();

protected:
	void getPlugins();
	void keyPressEvent(QKeyEvent *event);

signals:
	void keyPressed();

private:
    Ui::controlWidget* _ui;
    QMap< QString, QPair<QIcon*, DLLFactory<PluginFactory>* > > _plugins;
    Plugin* _currentPlugin;
    QWidget* _pluginWidget;

    int _sock_iwconfig;

    MusicControl* _mc;
    AlarmDaemon* _alarm;
};

class InitThread : public QThread
{
	Q_OBJECT
public:
	InitThread(Plugin* plugin);
	void run();

signals:
	void ready();

private:
    Plugin* _plugin;
};
}

#endif // CONTROLPANEL_H
