#ifndef NORMALANIMATION_H
#define NORMALANIMATION_H

#include "birdsanimation.h"

class NormalAnimation : public BirdsAnimation
{
public:
    NormalAnimation(QGraphicsScene* scene);
    virtual void reset();

private:
    QGraphicsPixmapItem *_cord1, *_cord2;
    QGraphicsTextItem *_consumptionShadow, *_consumption;
};

#endif // NORMALANIMATION_H
