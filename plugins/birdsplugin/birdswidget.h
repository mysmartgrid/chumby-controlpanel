#ifndef BIRDSWIDGET_H
#define BIRDSWIDGET_H

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class BirdsWidget;
}

class BirdsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BirdsWidget(QWidget *parent = 0);
    ~BirdsWidget();

protected:
    void paintEvent(QPaintEvent *);
    
private:
    Ui::BirdsWidget *_ui;
    QGraphicsScene *_scene;
};

#endif // BIRDSWIDGET_H
