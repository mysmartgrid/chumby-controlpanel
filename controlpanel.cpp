#include "controlpanel.h"

Controlpanel::Controlpanel(QWidget *parent)
    : QDialog(parent)
{
    label = new QLabel("blubb", this);
}

Controlpanel::~Controlpanel()
{
    
}

void Controlpanel::setContent(int i)
{
    QString str;
    str.setNum(i);
    label->setText(str);
}
