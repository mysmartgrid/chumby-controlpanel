#include "birdswidget.h"
#include "ui_birdswidget.h"

BirdsWidget::BirdsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::BirdsWidget),
    _scene(new QGraphicsScene)
{
    _ui->setupUi(this);

    _ui->view->setScene(_scene);
    _ui->view->setBackgroundBrush(QPixmap(":/birds/images/higru.png"));
    _ui->view->show();
}

BirdsWidget::~BirdsWidget()
{
    delete _ui;
}
