#include "normalanimation.h"

#include <qmath.h>

#include <QDebug>
#include <QGraphicsItemGroup>
#include <QTextCursor>

NormalAnimation::NormalAnimation(QGraphicsScene *scene)
    : BirdsAnimation(scene)
    , _sensorValue(-1)
    , _consumption(NULL)
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
    //force front cord to be in front of pillars
    _cord1->setZValue(10);

    // rear cord
    QPainterPath p2(QPointF(72, 110));
    p2.quadTo(200, 140 + (qSin((_counter+qrand()%5)/10)), 378, 120);
    _cord2 = _scene->addPath(p2, cordPen);

    _swingGroup = new QGraphicsItemGroup();
    _scene->addItem(_swingGroup);

    // swing
    QPointF swingKnot1(p1.pointAtPercent(0.7));
    QPointF swingKnot2(p1.pointAtPercent(0.9));
    QGraphicsPixmapItem* swing = _scene->addPixmap(QPixmap(":/birds/images/swing.png"));
    _swingGroup->addToGroup(swing);

    _birds = new QGraphicsItemGroup();
    _birds->addToGroup(_scene->addPixmap(QPixmap(":/birds/images/birds.png")));
    _birds->translate(0, p1.pointAtPercent(0.5).y() - 155);
    _birds->setZValue(11);
    _scene->addItem(_birds);

    _swingGroup->translate(0, swingKnot1.y() - swingKnot2.y());

    qDebug() << "Resetting _clouds to" << _scene->sceneRect().x() - _scene->sceneRect().width();
    _scene->addPixmap(QPixmap(":/birds/images/pillars.png"));
}

void NormalAnimation::step()
{
    _counter++;

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
    QPointF oldBirdPos(_cord1->path().pointAtPercent(0.5));
    // front cord
    QPainterPath p1(QPointF(17, 140));
    p1.cubicTo(160, 170 + (qSin(_counter/10)*2), 210, 160 + (qSin((_counter+5)/10)*1), 323, 151);
    _cord1->setPath(p1);

    // rear cord
    QPainterPath p2(QPointF(72, 110));
    p2.quadTo(200, 140 + (qSin((_counter+qrand()%5)/10)), 378, 120);
    _cord2->setPath(p2);

    QPointF newKnot1(_cord1->path().pointAtPercent(0.7));
    QPointF newBirdPos(_cord1->path().pointAtPercent(0.5));

    if ( _consumption == NULL )
    {
        QFont cFont("Arial", 35, QFont::Bold, false);
        _consumption = _scene->addText(QString::number(_sensorValue) + " W", cFont);
        _consumption->setDefaultTextColor(Qt::white);
    } else
        _consumption->setPlainText(QString::number(_sensorValue) + " W");

    QSizeF textsize = _consumption->boundingRect().size();
    QPointF swingpos = _swingGroup->scenePos();
    _consumption->setPos(325 - textsize.width() + swingpos.x(), 240 - textsize.height() + swingpos.y());

    _swingGroup->translate(0, newKnot1.y() - oldKnot1.y());
    _birds->translate(0, newBirdPos.y() - oldBirdPos.y());
}

void NormalAnimation::setValue(QString sensor, int value)
{
    //TODO: retrieve displayed sensor from config
    qDebug() << "setValue(" << sensor << "," << value << ")";
    if ( sensor.compare("1") == 0 || sensor.isEmpty() )
    {
        _sensorValue = value;
    }
}
