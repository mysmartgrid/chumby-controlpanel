#ifndef BIRDSANIMATION_H
#define BIRDSANIMATION_H

#include <QGraphicsScene>

class BirdsAnimation
{
public:
    BirdsAnimation(QGraphicsScene* scene)
        : _scene(scene)
        , _counter(0)
    {
    }

    virtual void reset() = 0;

protected:
    QGraphicsScene* _scene;
    unsigned int _counter;
};

#endif // BIRDSANIMATION_H
