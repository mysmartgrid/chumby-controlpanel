#ifndef SNOOZEDALARMSPAGE_H
#define SNOOZEDALARMSPAGE_H

#include <QWidget>

namespace Ui {
class SnoozedAlarmsPage;
}

class SnoozedAlarmsPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit SnoozedAlarmsPage(QWidget *parent = 0);
    ~SnoozedAlarmsPage();
    
private:
    Ui::SnoozedAlarmsPage *ui;
};

#endif // SNOOZEDALARMSPAGE_H
