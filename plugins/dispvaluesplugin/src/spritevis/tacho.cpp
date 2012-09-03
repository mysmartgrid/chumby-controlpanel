#include "tacho.h"
//#include "mouse.h"
//#include "rad.h"
//#include "zeiger.h"

#include <QtGui>

#include <math.h>
#include "qcustomgraphview.h"
#include <QDebug>

//static const int MouseCount = 0;
//static const int RadCount = 1;


Tacho::Tacho( QWidget* parent ) : QWidget( parent ), val (0)
{

    qsrand ( QTime ( 0, 0, 0 ).secsTo ( QTime::currentTime() ) );

    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect ( -150, -100, 300, 200 );
    scene->setItemIndexMethod ( QGraphicsScene::NoIndex );

    //	for ( int i = 0; i < MouseCount; ++i ) {
    //		Mouse *mouse = new Mouse;
    //mouse->setPos ( ::sin ( ( i * 6.28 ) / MouseCount ) * 200,
    //               ::cos ( ( i * 6.28 ) / MouseCount ) * 200 );
    //	mouse->setPos ( -50 + i*30 , -50 + i*30 );
    //		scene->addItem ( mouse );
    //	}

    //Rad* rad = new Rad;
    //rad->setPos(0,0);
    //scene->addItem(rad);

    zeig = new Zeiger;
    zeig->setPos ( 0, 0 );
    scene->addItem ( zeig );

    intext = scene->addText ( "W: " + QString::number ( val ), QFont("Times", 15, QFont::Bold) );
    intext->setPos ( 60, 55 );

    CustQGraphView* view = new CustQGraphView ( scene, this );
    view->setRenderHint ( QPainter::Antialiasing );
    //view->setBackgroundBrush ( QPixmap ( ":/images/Funken.jpg" ) );
    view->setCacheMode ( QGraphicsView::CacheBackground );
    view->setViewportUpdateMode ( QGraphicsView::BoundingRectViewportUpdate );
    view->setDragMode ( QGraphicsView::ScrollHandDrag );
    //view->setWindowTitle ( QT_TRANSLATE_NOOP ( QGraphicsView, "Colliding Blitz" ) );

    view->resize ( 320, 205 );
    view->show();


    QTimer* timer = new QTimer();
    connect ( timer, SIGNAL ( timeout() ), scene, SLOT ( advance() ) );
    timer->start ( 1000 / 25 );

    QTimer* timer2 = new QTimer();
    QObject::connect ( timer2, SIGNAL ( timeout() ), this, SLOT ( doanim() ) );
    timer2->start ( 1500 );
}

void Tacho::doanim()
{
    //qDebug() << "called timer..";
    intext->setPlainText("W: " + QString::number ( val ));
}

void Tacho::setValue(uint value){
    val = value;
    zeig->setVal(val);
    //qDebug() << "tacho: " << val;
}
