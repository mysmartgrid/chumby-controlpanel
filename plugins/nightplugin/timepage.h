#ifndef TIMEPAGE_H
#define TIMEPAGE_H

#include <QWizardPage>

namespace Ui {
class TimePage;
}

class TimePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit TimePage(QWidget *parent = 0);
    void initializePage();
    ~TimePage();

private slots:
    void hourUp();
    void hourDown();
    void minuteUp();
    void minuteDown();

private:
    Ui::TimePage *_ui;
};

#endif // TIMEPAGE_H
