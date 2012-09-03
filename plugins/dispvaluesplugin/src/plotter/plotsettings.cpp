#include "plotsettings.h"
#include <cmath>
#include <QDebug>

PlotSettings::PlotSettings()
{
	minX = 0.0;
	maxX = 60.0;
	numXTicks = 4;

	minY = 0.0;
	maxY = 1000.0;
	numYTicks = 4;
}

PlotSettings::PlotSettings ( double minx, double miny, double maxx, double maxy, int ticksx = 4, int ticksy = 4 )
{
	minX = minx;
	maxX = maxx;
	numXTicks = ticksx;

	minY = miny;
	maxY = maxy;
	numYTicks = ticksy;

}

//PlotSettings::~PlotSettings() {
//	qDebug() << "plot settings object destroyed!";
//}

void PlotSettings::adjust()
{
	adjustAxis ( minX, maxX, numXTicks );
	adjustAxis ( minY, maxY, numYTicks );
}

// adjusts plot ticks by a simple algorithm used in "c++ gui programming with qt"
// (j. blanchette and m. summerfiled) , page 137/138
void PlotSettings::adjustAxis ( double& min, double& max, int& numTicks )
{
	const int MinTicks = 4;
	double grossStep = ( max - min ) / MinTicks;
	double step = std::pow ( 10.0, std::floor ( std::log10 ( grossStep ) ) );

	if ( 5 * step < grossStep ) {
		step *= 5;
	} else if ( 2 * step < grossStep ) {
		step *= 2;
	}

	numTicks = int ( std::ceil ( max / step ) - std::floor ( min / step ) );
	if ( numTicks < MinTicks ) {
		numTicks = MinTicks;
	}
	min = std::floor ( min / step ) * step;
	max = std::ceil ( max / step ) * step;
}
