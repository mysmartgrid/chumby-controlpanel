#include <QtGui/QApplication>
#include "controlpanel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controlpanel w;
    w.show();
    
    return a.exec();
}
