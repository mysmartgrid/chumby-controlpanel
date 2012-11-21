#include "birdswidget.h"
#include "ui_birdswidget.h"

#include <QDebug>

BirdsWidget::BirdsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::BirdsWidget),
    _scene(new QGraphicsScene)
{
    _ui->setupUi(this);

    _ui->view->setScene(_scene);
    QPixmap* image = new QPixmap(":/birds/images/higru.png");
    _scene->addPixmap(*image);
    _ui->view->fitInView(image->rect());

    qDebug() << _scene->sceneRect().size();
    _ui->view->show();
}

BirdsWidget::~BirdsWidget()
{
    delete _ui;
}
