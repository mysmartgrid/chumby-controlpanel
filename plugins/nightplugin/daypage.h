#ifndef DAYPAGE_H
#define DAYPAGE_H

#include <QWizardPage>

namespace Ui {
class DayPage;
}

class DayPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit DayPage(QWidget *parent = 0);
	void initializePage();
    ~DayPage();
    
private:
    Ui::DayPage *ui;
};

#endif // DAYPAGE_H
