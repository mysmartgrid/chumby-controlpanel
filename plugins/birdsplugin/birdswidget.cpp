#include "birdswidget.h"
#include "ui_birdswidget.h"

#include <QDebug>

BirdsWidget::BirdsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::BirdsWidget),
    _scene(new QGraphicsScene(0, 0, 400, 300))
{
    _ui->setupUi(this);

    _ui->view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    QPixmap* image = new QPixmap(":/birds/images/higru.png");
    _scene->setBackgroundBrush(*image);
    _ui->view->setScene(_scene);
    _ui->view->show();
}

BirdsWidget::~BirdsWidget()
{
    delete _ui;
}

void BirdsWidget::paintEvent(QPaintEvent *)
{
    _ui->view->ensureVisible(_scene->sceneRect());
    _ui->view->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
}
