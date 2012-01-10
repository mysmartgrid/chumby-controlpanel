#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QtGui/QDialog>
#include <QLabel>

class Controlpanel : public QDialog
{
    Q_OBJECT
    
public:
    Controlpanel(QWidget *parent = 0);
    ~Controlpanel();
    QLabel* label;

    void setContent(int i);
};

#endif // CONTROLPANEL_H
