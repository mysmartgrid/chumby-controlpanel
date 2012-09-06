#ifndef CHUMBYKBDDRIVERPLUGIN_H
#define CHUMBYKBDDRIVERPLUGIN_H

#include <QKbdDriverPlugin>
#include <QtGui/QWSKeyboardHandlerFactoryInterface>
#include <chumbykbdhandler.h>

class ChumbyKbdDriverPlugin : public QKbdDriverPlugin
{
    Q_OBJECT
public:
    explicit ChumbyKbdDriverPlugin(QObject *parent = 0);
    ~ChumbyKbdDriverPlugin();
    QWSKeyboardHandler* create(const QString& driver, const QString& device);
    QWSKeyboardHandler* create(const QString& driver);
    QStringList keys() const;
};

#endif // CHUMBYKBDDRIVERPLUGIN_H
