#ifndef SNOOZEPAGE_H
#define SNOOZEPAGE_H

#include <QWizardPage>

namespace Ui {
class SnoozePage;
}

class SnoozePage : public QWizardPage
{
        Q_OBJECT

public:
        explicit SnoozePage(QWidget *parent = 0);
        ~SnoozePage();
        void initializePage();

private slots:
        void up();
        void down();

private:
        Ui::SnoozePage *_ui;
};

#endif // SNOOZEPAGE_H
