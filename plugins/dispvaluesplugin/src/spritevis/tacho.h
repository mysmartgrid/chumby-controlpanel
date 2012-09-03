#ifndef __TACHO_H__
#define __TACHO_H__
#include <QWidget>
#include <QGraphicsTextItem>
#include "zeiger.h"

class Tacho : public QWidget
{
		Q_OBJECT

	public:
		Tacho( QWidget* parent = 0 );
		void setValue(uint);
	
	private:
		QGraphicsTextItem* intext;	
		Zeiger* zeig;
		uint val;
	
	private slots:
		void doanim();
};
#endif // __TACHO_H__
