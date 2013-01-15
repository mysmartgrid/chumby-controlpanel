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

    _scene->addPixmap(QPixmap(":/birds/images/higru_new.png"));

    _clouds = _scene->addPixmap(QPixmap(":/birds/images/normalclouds.png"));
    _clouds->setPos(_scene->sceneRect().x(), _scene->sceneRect().y());
    _clouds2 = _scene->addPixmap(QPixmap(":/birds/images/normalclouds.png"));
    _clouds2->setPos(_scene->sceneRect().x() - _scene->sceneRect().width(), _scene->sceneRect().y());

    QPen cordPen;
    cordPen.setWidth(3);
    cordPen.setCapStyle(Qt::RoundCap);

    // front cord
    QPainterPath p1(QPointF(20, 78));
    p1.cubicTo(90, 90 + (qSin(_counter/10)*2), 130, 95 + (qSin((_counter+5)/10)*1), 262, 85);
    _cord1 = _scene->addPath(p1, cordPen);
    //force front cord to be in front of pillars
    _cord1->setZValue(10);

    // rear cord
    QPainterPath p2(QPointF(63, 53));
    p2.quadTo(170, 68 + (qSin((_counter+qrand()%5)/10)), 300, 60);
    _cord2 = _scene->addPath(p2, cordPen);

    _swingGroup = new QGraphicsItemGroup();
    _scene->addItem(_swingGroup);

    // swing
    QPointF swingKnot1(p1.pointAtPercent(0.7));
    QPointF swingKnot2(p1.pointAtPercent(0.9));
    QGraphicsPixmapItem* swing = _scene->addPixmap(QPixmap(":/birds/images/swing.png"));
    swing->scale(0.8, 0.8);
    swing->translate(0, -45);
    _swingGroup->addToGroup(swing);

    _birds = new QGraphicsItemGroup();
    //TODO: build birds out of smaller images for animation

    /*QGraphicsPixmapItem* birds = _scene->addPixmap(QPixmap(":/birds/images/birds.png"));
    birds->scale(0.8, 0.8);
    _birds->addToGroup(birds);*/

    _largebird = new LargeBird;
    _largebird->group = new QGraphicsItemGroup();

    _largebird->tail = _scene->addPixmap(QPixmap(":/birds/images/taill.png"));
    _largebird->tail->translate(40, 45);
    _largebird->tail->scale(0.23, 0.23);
    _largebird->group->addToGroup(_largebird->tail);
    _largebird->body = _scene->addPixmap(QPixmap(":/birds/images/bodyl.png"));
    _largebird->body->scale(0.23, 0.23);
    _largebird->group->addToGroup(_largebird->body);
    _largebird->leftWing = _scene->addPixmap(QPixmap(":/birds/images/wingll.png"));
    _largebird->leftWing->translate(-8, 45);
    _largebird->leftWing->scale(0.3, 0.3);
    _largebird->group->addToGroup(_largebird->leftWing);
    _largebird->rightWing = _scene->addPixmap(QPixmap(":/birds/images/winglr.png"));
    _largebird->rightWing->translate(35, 47);
    _largebird->rightWing->scale(0.23, 0.23);
    _largebird->group->addToGroup(_largebird->rightWing);

    _largebird->group->translate(92, 65);

    _smallbird = new SmallBird;
    _smallbird->group = new QGraphicsItemGroup();

    _smallbird->tail = _scene->addPixmap(QPixmap(":/birds/images/tails.png"));
    _smallbird->tail->translate(25, 20);
    _smallbird->tail->scale(0.23, 0.23);
    _smallbird->group->addToGroup(_smallbird->tail);
    _smallbird->body = _scene->addPixmap(QPixmap(":/birds/images/bodys.png"));
    _smallbird->body->scale(0.23, 0.23);
    _smallbird->group->addToGroup(_smallbird->body);
    _smallbird->hair = _scene->addPixmap(QPixmap(":/birds/images/hairs.png"));
    _smallbird->hair->translate(7, -5);
    _smallbird->hair->scale(0.23, 0.23);
    _smallbird->group->addToGroup(_smallbird->hair);
    _smallbird->leftWing = _scene->addPixmap(QPixmap(":/birds/images/wingsl.png"));
    _smallbird->leftWing->translate(-5, 20);
    _smallbird->leftWing->scale(0.23, 0.23);
    _smallbird->group->addToGroup(_smallbird->leftWing);
    _smallbird->rightWing = _scene->addPixmap(QPixmap(":/birds/images/wingsr.png"));
    _smallbird->rightWing->translate(22, 23);
    _smallbird->rightWing->scale(0.23, 0.23);
    _smallbird->group->addToGroup(_smallbird->rightWing);

    _smallbird->group->translate(152, 106);

    _birds->addToGroup(_largebird->group);
    _birds->addToGroup(_smallbird->group);

    _birds->translate(0, p1.pointAtPercent(0.5).y() - 155);
    _birds->setZValue(11);
    _scene->addItem(_birds);

    _swingGroup->translate(0, swingKnot1.y() - swingKnot2.y());

#ifdef BIRDS_DEBUG
    qDebug() << "Resetting _clouds to" << _scene->sceneRect().x() - _scene->sceneRect().width();
#endif
    _scene->addPixmap(QPixmap(":/birds/images/pillars_new2.png"));
}

void NormalAnimation::step()
{
    _counter++;

    _clouds->translate(1, 0);
    if ( _clouds->scenePos().x() > _scene->sceneRect().x() + _scene->sceneRect().width() )
    {
#ifdef BIRDS_DEBUG
        qDebug() << "Resetting _clouds to" << _scene->sceneRect().x() - _scene->sceneRect().width();
#endif
        _clouds->translate(-2 * _scene->sceneRect().width(), 0);
    }
    _clouds2->translate(1, 0);
    if ( _clouds2->scenePos().x() > _scene->sceneRect().x() + _scene->sceneRect().width() )
    {
#ifdef BIRDS_DEBUG
        qDebug() << "Resetting _clouds2 to" << _scene->sceneRect().x() - _scene->sceneRect().width();
#endif
        _clouds2->translate(-2 * _scene->sceneRect().width(), 0);
    }

    QPen cordPen;
    cordPen.setWidth(3);
    cordPen.setCapStyle(Qt::RoundCap);

    QPointF oldKnot1(_cord1->path().pointAtPercent(0.7));
    QPointF oldBirdPos(_cord1->path().pointAtPercent(0.5));
    // front cord
    QPainterPath p1(QPointF(20, 78));
    p1.cubicTo(90, 90 + (qSin(_counter/10)*2), 130, 95 + (qSin((_counter+5)/10)*1), 262, 85);
    _cord1->setPath(p1);

    // rear cord
    QPainterPath p2(QPointF(63, 53));
    p2.quadTo(170, 68 + (qSin((_counter+qrand()%5)/10)), 300, 60);
    _cord2->setPath(p2);

    QPointF newKnot1(_cord1->path().pointAtPercent(0.7));
    QPointF newBirdPos(_cord1->path().pointAtPercent(0.5));

    if ( _consumption == NULL )
    {
        QFont cFont("Arial", 35, QFont::Bold, false);
        _consumption = _scene->addText(QString::number(_sensorValue) + " W", cFont);
        _consumption->setDefaultTextColor(QColor("#006633"));
    } else
        _consumption->setPlainText(QString::number(_sensorValue) + " W");

    QSizeF textsize = _consumption->boundingRect().size();
    QPointF swingpos = _swingGroup->scenePos();
    _consumption->setPos(260 - textsize.width() + swingpos.x(), 163 - textsize.height() + swingpos.y());

    _swingGroup->translate(0, newKnot1.y() - oldKnot1.y());
    _birds->translate(0, newBirdPos.y() - oldBirdPos.y());
}

void NormalAnimation::setValue(QString sensor, int value)
{
    //TODO: retrieve displayed sensor from config
#ifdef BIRDS_DEBUG
    qDebug() << "setValue(" << sensor << "," << value << ")";
#endif
    if ( sensor.compare("1") == 0 || sensor.isEmpty() )
    {
        _sensorValue = value;
    }
}
