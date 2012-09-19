#ifndef NAMEPAGE_H
#define NAMEPAGE_H

#include <QWizardPage>

namespace Ui {
class NamePage;
}

class NamePage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit NamePage(QWidget *parent = 0);
	void initializePage();
    ~NamePage();
    
private:
    Ui::NamePage *ui;
};

#endif // NAMEPAGE_H
