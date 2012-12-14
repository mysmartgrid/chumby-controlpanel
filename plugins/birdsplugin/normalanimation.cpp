#include "normalanimation.h"

#include <qmath.h>

#include <QDebug>
#include <QGraphicsItemGroup>
#include <QTextCursor>

NormalAnimation::NormalAnimation(QGraphicsScene *scene)
    : BirdsAnimation(scene)
{
}

void NormalAnimation::reset()
{
    _scene->clear();

    _scene->addPixmap(QPixmap(":/birds/images/higru.png"));

    _clouds = _scene->addPixmap(QPixmap(":/birds/images/normalclouds.png"));
    _clouds->setPos(_scene->sceneRect().x(), _scene->sceneRect().y());
    _clouds2 = _scene->addPixmap(QPixmap(":/birds/images/normalclouds.png"));
    _clouds2->setPos(_scene->sceneRect().x() - _scene->sceneRect().width(), _scene->sceneRect().y());

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

    QFont cFont("Arial", 25, QFont::Bold, false);
    _consumption = _scene->addText("0 W", cFont);
    qreal w = _consumption->textWidth();
    _swingGroup->addToGroup(_consumption);
    _consumption->translate(230, 200);
    _consumption->setDefaultTextColor(Qt::white);
    //TODO: calculate maximum width and set alignment to right

    _swingGroup->translate(0, swingKnot1.y() - swingKnot2.y());

    qDebug() << "Resetting _clouds to" << _scene->sceneRect().x() - _scene->sceneRect().width();
    _scene->addPixmap(QPixmap(":/birds/images/pillars.png"));
}

void NormalAnimation::step()
{
    //TODO
    //setValue(qrand()%100);

    _counter++;
    _scene->removeItem(_cord1);
    _scene->removeItem(_cord2);

    _clouds->translate(1, 0);
    if ( _clouds->scenePos().x() > _scene->sceneRect().x() + _scene->sceneRect().width() )
    {
        qDebug() << "Resetting _clouds to" << _scene->sceneRect().x() - _scene->sceneRect().width();
        _clouds->translate(-2 * _scene->sceneRect().width(), 0);
    }
    _clouds2->translate(1, 0);
    if ( _clouds2->scenePos().x() > _scene->sceneRect().x() + _scene->sceneRect().width() )
    {
        qDebug() << "Resetting _clouds2 to" << _scene->sceneRect().x() - _scene->sceneRect().width();
        _clouds2->translate(-2 * _scene->sceneRect().width(), 0);
    }

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

void NormalAnimation::setValue(int value)
{
    _consumption->setPlainText(QString::number(value) + " W");
}
