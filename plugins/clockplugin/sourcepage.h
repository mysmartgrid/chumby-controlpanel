#ifndef SOURCEPAGE_H
#define SOURCEPAGE_H

#include <QWidget>
#include <QListWidgetItem>

#include "alarm.h"
#include "audioplugin.h"

namespace Ui {
class SourcePage;
}

class SourcePage : public QWidget
{
    Q_OBJECT
    
public:
    explicit SourcePage(Msg::Alarm* alarm, QWidget *parent = 0);
    ~SourcePage();

protected slots:
    void setSource();
    void back();
    void selectSource(QListWidgetItem *item);

private:
    Msg::AudioPlugin* getPlugin();
    QString getPath() const;
    void initSourceList();
    Ui::SourcePage *_ui;
    Msg::Alarm *_alarm;
    Msg::AudioPlugin* _plugin;
    QStringList _source;
};

#endif // SOURCEPAGE_H
