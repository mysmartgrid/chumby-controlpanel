#ifndef NORMALANIMATION_H
#define NORMALANIMATION_H

#include "birdsanimation.h"

class NormalAnimation : public BirdsAnimation
{
    Q_OBJECT

public:
    NormalAnimation(QGraphicsScene* scene);
    virtual void reset();

public slots:
    virtual void step();
    virtual void setValue(QString sensor, int value);

private:
    int _sensorValue;
    QGraphicsPixmapItem *_clouds, *_clouds2;
    QGraphicsPathItem *_cord1, *_cord2;
    QGraphicsTextItem *_consumption;
    QGraphicsItemGroup *_swingGroup;
    QGraphicsItemGroup *_birds;
};

#endif // NORMALANIMATION_H
