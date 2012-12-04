#ifndef NORMALANIMATION_H
#define NORMALANIMATION_H

#include "birdsanimation.h"

class NormalAnimation : public BirdsAnimation
{
    Q_OBJECT

public:
    NormalAnimation(QGraphicsScene* scene);
    virtual void reset();

protected slots:
    virtual void step();

private:
    QGraphicsPathItem *_cord1, *_cord2;
    QGraphicsTextItem *_consumption;
    QGraphicsItemGroup *_swingGroup;
};

#endif // NORMALANIMATION_H
