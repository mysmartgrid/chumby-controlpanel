#ifndef BIRDSANIMATION_H
#define BIRDSANIMATION_H

#include <QGraphicsScene>

class BirdsAnimation : public QObject
{
    Q_OBJECT

public:
    BirdsAnimation(QGraphicsScene* scene)
        : QObject()
        , _scene(scene)
        , _counter(0)
    {
    }

    virtual void reset() = 0;
    virtual void setValue(int value) = 0;

protected slots:
    virtual void step() = 0;

protected:
    QGraphicsScene* _scene;
    unsigned int _counter;
};

#endif // BIRDSANIMATION_H
