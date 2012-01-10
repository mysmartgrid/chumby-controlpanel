#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QtGui/QDialog>

class Controlpanel : public QDialog
{
    Q_OBJECT
    
public:
    Controlpanel(QWidget *parent = 0);
    ~Controlpanel();
};

#endif // CONTROLPANEL_H
