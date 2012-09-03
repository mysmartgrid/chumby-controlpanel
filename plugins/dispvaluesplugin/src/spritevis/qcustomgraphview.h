#ifndef __QCUSTOMGRAPHSCENE_H__
#define __QCUSTOMGRAPHSCENE_H__

#include <QGraphicsView>

class CustQGraphView : public QGraphicsView
{
	Q_OBJECT
	
	public:
		CustQGraphView ( QWidget *);
		CustQGraphView ( QGraphicsScene *, QWidget * parent = 0 );
		void drawBackground ( QPainter *, const QRectF & );

};
#endif // __QCUSTOMGRAPHSCENE_H__
