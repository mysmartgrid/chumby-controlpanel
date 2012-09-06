#include "chumbykbddriverplugin.h"

ChumbyKbdDriverPlugin::ChumbyKbdDriverPlugin(QObject *parent) :
    QKbdDriverPlugin(parent)
{
}

ChumbyKbdDriverPlugin::~ChumbyKbdDriverPlugin()
{

}

QStringList ChumbyKbdDriverPlugin::keys() const
{
    return QStringList() << "chumbykbdhandler";
}

QWSKeyboardHandler* ChumbyKbdDriverPlugin::create(const QString &driver, const QString &device)
{
    Q_UNUSED(device)
    if ( driver.toLower() == "chumbykbdhandler")
    {
        return new ChumbyKbdHandler(device);
    }
    return 0;
}

QWSKeyboardHandler* ChumbyKbdDriverPlugin::create(const QString &driver)
{
    if ( driver.toLower() == "chumbykbdhandler")
    {
        return new ChumbyKbdHandler();
    }
    return 0;
}

Q_EXPORT_PLUGIN2(chumbykbddriverplugin, ChumbyKbdDriverPlugin)
