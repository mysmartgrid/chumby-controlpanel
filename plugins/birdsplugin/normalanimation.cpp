#include "normalanimation.h"

#include <qmath.h>

NormalAnimation::NormalAnimation(QGraphicsScene *scene)
    : BirdsAnimation(scene)
{
}

void NormalAnimation::reset()
{
    _scene->clear();

    _scene->addPixmap(QPixmap(":/birds/images/higru.png"));

    QPen cordPen;
    cordPen.setWidth(3);
    cordPen.setCapStyle(Qt::RoundCap);

    // front cord
    QPainterPath _cord1(QPointF(17, 140));
    _cord1.cubicTo(160, 170 + (qSin(_counter/10)*2), 210, 160 + (qSin((_counter+5)/10)*1), 323, 151);
    _scene->addPath(_cord1, cordPen);

    // rear cord
    QPainterPath _cord2(QPointF(72, 110));
    _cord2.quadTo(200, 140 + (qSin((_counter+qrand()%5)/10)), 378, 120);
    _scene->addPath(_cord2, cordPen);

    // swing
    QPointF swingKnot1(p1.pointAtPercent(0.7));
    QPointF swingKnot2(p1.pointAtPercent(0.9));
    //_scene->addLine(swingKnot1.x(), swingKnot1.y(), swingKnot1.x(), swingKnot1.y()+50);
    //_scene->addLine(swingKnot2.x(), swingKnot2.y(), swingKnot2.x(), swingKnot2.y()+50);
    QGraphicsPixmapItem* swing = _scene->addPixmap(QPixmap(":/birds/images/swing.png"));
    swingGroup->addToGroup(swing);
    //swing->translate(0, ceil((swingKnot1.y() - swingKnot2.y())/2));

    int texty = swingKnot1.y() + 40;
    qDebug() << texty;
    QFont cFont("Arial", 25, QFont::Bold, false);
    QGraphicsTextItem* consumptionShadow = _scene->addText("0 Watt", cFont);
    swingGroup->addToGroup(consumptionShadow);
    consumptionShadow->translate(232, 202);
    consumptionShadow->setDefaultTextColor(Qt::green);
    QGraphicsTextItem* consumption = _scene->addText("0 Watt", cFont);
    swingGroup->addToGroup(consumption);
    consumption->translate(230, 200);
    consumption->setDefaultTextColor(Qt::white);

    swingGroup->translate(0, swingKnot1.y() - swingKnot2.y());

    _scene->addPixmap(QPixmap(":/birds/images/pillars.png"));
}
