#ifndef BIRDSWIDGET_H
#define BIRDSWIDGET_H

#include <QWidget>
#include <QGraphicsScene>

#include <QTimer>

#include "birdsanimation.h"

namespace Ui {
class BirdsWidget;
}

class BirdsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BirdsWidget(QWidget *parent = 0);
    ~BirdsWidget();

protected:
    void paintEvent(QPaintEvent *);
    void normalAnimation(unsigned int counter, int value);

protected slots:
    void animate();
    
private:
    Ui::BirdsWidget *_ui;
    QGraphicsScene *_scene;
    QTimer* _timer;
    BirdsAnimation* _animation;
};

#endif // BIRDSWIDGET_H
