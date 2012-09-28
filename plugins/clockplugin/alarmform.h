#ifndef ALARMFORM_H
#define ALARMFORM_H

#include <QtGui/QListWidgetItem>
#include <QtGui/QWidget>

namespace Ui {
class AlarmForm;
}

class AlarmForm : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmForm(QWidget *parent = 0);
    ~AlarmForm();

protected slots:
    void editAlarm(Msg::Alarm *alarm = NULL);
    void alarmDetails();
    void refresh();

private:
    Ui::AlarmForm *_ui;
};

#endif // ALARMFORM_H
