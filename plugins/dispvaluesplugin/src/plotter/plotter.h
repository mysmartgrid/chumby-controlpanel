#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QToolButton>

#include "plotsettings.h"
#include "converter.h"
#include "qpointd.h"


class ConverterDefault : public Converter
{
	public:
		QString convert ( double );
};


class Plotter : public QWidget
{
		Q_OBJECT
	public:
		Plotter ( QWidget* parent = 0 );

		void setPlotSettings ( PlotSettings& settings );
		void setCurveData ( int id, const QVector<QPointD>& data );
		void clearCurve ( int id );
		QSize minimumSizeHint() const;
		QSize sizeHint() const;
		Converter* conv;
		void setConverter ( Converter* converter );


	public slots:
		void zoomIn();
		void zoomOut();
		void updatePlot();


	protected:
		void paintEvent ( QPaintEvent* event );
		void resizeEvent ( QResizeEvent* event );
		
		//not implemented for the chumby single-click-touchscreen yet!
		//can be used for zoom-feature in future versions
		//void mousePressEvent ( QMouseEvent* event ); 
		//void mouseReleaseEvent ( QMouseEvent* event );

	private:
		void updateRubberBandRegion();
		void refreshPixmap();
		void drawGrid ( QPainter* painter );
		void drawCurves ( QPainter* painter );

		enum
		{ MarginT = 8, MarginB = 18, MarginR = 20, MarginL = 32 };

		QToolButton* zoomInButton;
		QToolButton* zoomOutButton;
		QMap<int, QVector<QPointD> > curveMap;
		QVector<PlotSettings> zoomStack;
		int curZoom;
		bool rubberBandIsShown;
		QRect rubberBandRect;
		QPixmap pixmap;

};

#endif
