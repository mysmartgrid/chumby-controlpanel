#include "plotter.h"

#include <QStylePainter>
#include <QStyleOptionFocusRect>
#include <QDebug>
#include <QDateTime>


Plotter::Plotter ( QWidget* parent ) : QWidget ( parent )
{
	setBackgroundRole ( QPalette::Dark );
	setAutoFillBackground ( true );
	setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );
	setFocusPolicy ( Qt::StrongFocus );
	rubberBandIsShown = false;

	zoomInButton = new QToolButton ( this );
	zoomInButton->setText ( " + " );
	zoomInButton->adjustSize();
	connect ( zoomInButton, SIGNAL ( clicked() ), this, SLOT ( zoomIn() ) );

	zoomOutButton = new QToolButton ( this );
	zoomOutButton->setText ( " - " );
	zoomOutButton->adjustSize();
	connect ( zoomOutButton, SIGNAL ( clicked() ), this, SLOT ( zoomOut() ) );

	conv = new ConverterDefault();
	PlotSettings* ps = new PlotSettings();
	setPlotSettings ( *ps );
	//setPlotSettings ( *(new PlotSettings()) );
}

void Plotter::setPlotSettings ( PlotSettings& settings ) //modify parameters
//to use non-constant settings-objects; make it possible to change settings for existig plotter object
{
	zoomStack.clear();
	zoomStack.append ( settings );
	curZoom = 0;
	zoomInButton->hide();
	zoomOutButton->hide();
	refreshPixmap();
}


void Plotter::zoomIn()
{
	if ( curZoom < zoomStack.count() - 1 ) {
		++curZoom;
		zoomInButton->setEnabled ( curZoom < zoomStack.count() - 1 );
		zoomOutButton->setEnabled ( true );
		zoomOutButton->show();
		refreshPixmap();
	}

}


void Plotter::zoomOut()
{
	if ( curZoom > 0 ) {
		--curZoom;
		zoomOutButton->setEnabled ( curZoom > 0 );
		zoomInButton->setEnabled ( true );
		zoomInButton->show();
		refreshPixmap();
	}
}


void Plotter::setCurveData ( int id, const QVector<QPointD>& data )
{
	curveMap.insert ( id, data ); //better way to insert data in map

	//refreshPixmap(); 	
	//do not repaint everytime new curvedata is inserted. it's much better to
	//manually call for plot update when there are multiple curves to plot
	//at once.. 
}


void Plotter::clearCurve ( int id )
{
	curveMap.remove ( id );
	refreshPixmap();
}

void Plotter::updatePlot()
{
	refreshPixmap();
}


QSize Plotter::minimumSizeHint() const
{
	return QSize ( 4  * MarginL, 4 * MarginB );
}


QSize Plotter::sizeHint() const
{
	return QSize ( 8 * MarginL, 8 * MarginB );
}


void Plotter::paintEvent ( QPaintEvent* /* event */ ) //comment unused var
{
	QStylePainter painter ( this );
	painter.drawPixmap ( 0, 0, pixmap );

	if ( rubberBandIsShown ) {
		painter.setPen ( palette().dark().color() );
		painter.drawRect ( rubberBandRect.normalized().adjusted ( 0, 0, -1, -1 ) );
	}

	if ( hasFocus() ) {
		QStyleOptionFocusRect option;
		option.initFrom ( this );
		option.backgroundColor = palette().dark().color();
		painter.drawPrimitive ( QStyle::PE_FrameFocusRect, option );
	}

}


void Plotter::resizeEvent ( QResizeEvent* /* event */ )
{
	int x = width() - ( zoomInButton->width() + zoomInButton->width() + 10 );
	zoomInButton->move ( x, 5 );
	zoomOutButton->move ( x + zoomInButton->width() + 5, 5 );
	refreshPixmap();
}


void Plotter::updateRubberBandRegion()
{
	QRect rect = rubberBandRect.normalized();
	update ( rect.left(), rect.top(), rect.width(), 1 );
	update ( rect.left(), rect.top(), 1, rect.height() );
	update ( rect.left(), rect.bottom(), rect.width(), 1 );
	update ( rect.right(), rect.top(), 1, rect.height() );
}


void Plotter::refreshPixmap()
{
	pixmap = QPixmap ( size() );
	pixmap.fill ( this, 0, 0 );

	QPainter painter ( &pixmap );
	painter.initFrom ( this );
	drawGrid ( &painter );
	drawCurves ( &painter );
	update();
}


void Plotter::drawGrid ( QPainter* painter )
{
	QRect rect ( MarginL, MarginT, width() - MarginL - MarginR, height() - MarginT - MarginB );
	if ( !rect.isValid() ) {
		return;
	}

	PlotSettings settings = zoomStack[curZoom];
	QPen quiteDark = palette().dark().color().light();
	QPen light = palette().light().color();
	QFont f ( painter->fontInfo().family(), 10, QFont::Light, painter );
	painter->setFont ( f );

	for ( int i = 0; i <= settings.numXTicks; ++i ) {
		int x = rect.left() + ( i * ( rect.width() - 1 ) / settings.numXTicks );
		double label = settings.minX + ( i * settings.spanX() / settings.numXTicks );
		painter->setPen ( quiteDark );
		painter->drawLine ( x, rect.top(), x, rect.bottom() );
		painter->setPen ( light );
		painter->drawLine ( x, rect.bottom(), x, rect.bottom() + 5 );
		//painter->drawText ( x - 50, rect.bottom() + 5, 100, 20, Qt::AlignHCenter | Qt::AlignTop, QString::number ( label, 'f', 0 ) );
		painter->drawText ( x - 50, rect.bottom() + 5, 100, 20, Qt::AlignHCenter | Qt::AlignTop, conv->convert ( label ) );
	}

	for ( int j = 0; j <= settings.numYTicks; ++j ) {
		int y = rect.bottom() - ( j * ( rect.height() - 1 ) / settings.numYTicks );
		double label = settings.minY + ( j * settings.spanY() / settings.numYTicks );
		painter->setPen ( quiteDark );
		painter->drawLine ( rect.left(), y, rect.right(), y );
		painter->setPen ( light );
		painter->drawLine ( rect.left() - 5, y, rect.left(), y );
		painter->drawText ( rect.left() - MarginL, y - 10, MarginL - 5, 20, Qt::AlignVCenter | Qt::AlignRight, QString::number ( label , 'f', 0 ) );
	}
	painter->drawRect ( rect.adjusted ( 0, 0, -1, -1 ) );
}


void Plotter::drawCurves ( QPainter* painter )
{
	static const QColor colorForIds[6] = {
	                                       Qt::red, Qt::green, Qt::yellow, Qt::cyan, Qt::magenta, Qt::blue
	                                     };
	PlotSettings settings = zoomStack[curZoom];

	QRect rect ( MarginL, MarginT, width() - MarginL - MarginR, height() - MarginT - MarginB );
	if ( !rect.isValid() ) {
		return;
	}

	painter->setClipRect ( rect.adjusted ( + 1, + 1, -1, -1 ) );
	
	QMapIterator<int, QVector<QPointD> > i ( curveMap );
	while ( i.hasNext() ) {
		i.next();
		int id = i.key();
		QVector<QPointD> data = i.value();
		QPolygonF polyline ( data.count() );

		for ( int j = 0; j < data.count(); ++j ) {
			
			double dx = data[j].x() - settings.minX;
			double dy = data[j].y() - settings.minY;
			double x = rect.left() + ( dx * ( rect.width() - 1 ) / settings.spanX() );
			double y = rect.bottom() - ( dy * ( rect.height() - 1 ) / settings.spanY() );
			
			polyline[j] = QPointF ( x, y );
		}

		painter->setPen ( colorForIds[uint ( id ) % 6] );
		painter->drawPolyline ( polyline );

	}

}


void Plotter::setConverter ( Converter* converter )
{
	conv = converter;
}


QString ConverterDefault::convert ( double value )
{
	return QString::number ( value, 'f', 0 );
}


