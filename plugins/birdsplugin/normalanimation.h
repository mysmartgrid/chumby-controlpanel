#ifndef NORMALANIMATION_H
#define NORMALANIMATION_H

#include "birdsanimation.h"

class NormalAnimation : public BirdsAnimation
{
    Q_OBJECT

    struct SmallBird {
        QGraphicsPixmapItem *body;
        QGraphicsPixmapItem *leftWing, *rightWing;
        QGraphicsPixmapItem *tail;
        QGraphicsPixmapItem *leftFoot, *rightFoot;
        QGraphicsPixmapItem *hair;
        QGraphicsEllipseItem *leftEye, *rightEye;
        QGraphicsPixmapItem *beak;
        QGraphicsItemGroup *group;
    };

    struct LargeBird {
        QGraphicsPixmapItem *body;
        QGraphicsPixmapItem *leftWing, *rightWing;
        QGraphicsPixmapItem *tail;
        QGraphicsPixmapItem *leftFoot, *rightFoot;
        QGraphicsPixmapItem *eyebrows;
        QGraphicsPixmapItem *leftEye, *rightEye;
        QGraphicsEllipseItem *leftIris, *rightIris;
        QGraphicsPixmapItem *beak;
        QGraphicsItemGroup *group;
    };

public:
    NormalAnimation(QGraphicsScene* scene);
    virtual void reset();

public slots:
    virtual void step();
    virtual void setValue(QString sensor, int value);
    virtual void setError(QString error);

private:
    int _sensorValue;
    QString _errorString;
    int _errorCounter;
    QGraphicsPixmapItem *_clouds, *_clouds2;
    QGraphicsPathItem *_cord1, *_cord2;
    QGraphicsTextItem *_consumption, *_errorText;
    QGraphicsItemGroup *_swingGroup;
    QGraphicsItemGroup *_birds;
    LargeBird *_largebird;
    SmallBird *_smallbird;
};

#endif // NORMALANIMATION_H
