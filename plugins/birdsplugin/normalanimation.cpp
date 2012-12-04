#include "normalanimation.h"

#include <qmath.h>

#include <QDebug>
#include <QGraphicsItemGroup>

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
    QPainterPath p1(QPointF(17, 140));
    p1.cubicTo(160, 170 + (qSin(_counter/10)*2), 210, 160 + (qSin((_counter+5)/10)*1), 323, 151);
    _cord1 = _scene->addPath(p1, cordPen);

    // rear cord
    QPainterPath p2(QPointF(72, 110));
    p2.quadTo(200, 140 + (qSin((_counter+qrand()%5)/10)), 378, 120);
    _cord2 = _scene->addPath(p2, cordPen);

    _swingGroup = new QGraphicsItemGroup();
    _scene->addItem(_swingGroup);

    // swing
    QPointF swingKnot1(p1.pointAtPercent(0.7));
    QPointF swingKnot2(p1.pointAtPercent(0.9));
    //_scene->addLine(swingKnot1.x(), swingKnot1.y(), swingKnot1.x(), swingKnot1.y()+50);
    //_scene->addLine(swingKnot2.x(), swingKnot2.y(), swingKnot2.x(), swingKnot2.y()+50);
    QGraphicsPixmapItem* swing = _scene->addPixmap(QPixmap(":/birds/images/swing.png"));
    _swingGroup->addToGroup(swing);
    //swing->translate(0, ceil((swingKnot1.y() - swingKnot2.y())/2));

    int texty = swingKnot1.y() + 40;
    qDebug() << texty;
    QFont cFont("Arial", 25, QFont::Bold, false);
    QGraphicsTextItem* consumption = _scene->addText("0 Watt", cFont);
    _swingGroup->addToGroup(consumption);
    consumption->translate(230, 200);
    consumption->setDefaultTextColor(Qt::white);

    _swingGroup->translate(0, swingKnot1.y() - swingKnot2.y());

    _scene->addPixmap(QPixmap(":/birds/images/pillars.png"));
}

void NormalAnimation::step()
{
    _counter++;
    _scene->removeItem(_cord1);
    _scene->removeItem(_cord2);

    QPen cordPen;
    cordPen.setWidth(3);
    cordPen.setCapStyle(Qt::RoundCap);

    QPointF oldKnot1(_cord1->path().pointAtPercent(0.7));
    // front cord
    QPainterPath p1(QPointF(17, 140));
    p1.cubicTo(160, 170 + (qSin(_counter/10)*2), 210, 160 + (qSin((_counter+5)/10)*1), 323, 151);
    _cord1 = _scene->addPath(p1, cordPen);

    // rear cord
    QPainterPath p2(QPointF(72, 110));
    p2.quadTo(200, 140 + (qSin((_counter+qrand()%5)/10)), 378, 120);
    _cord2 = _scene->addPath(p2, cordPen);

    QPointF newKnot1(_cord1->path().pointAtPercent(0.7));

    _swingGroup->translate(0, newKnot1.y() - oldKnot1.y());
}
