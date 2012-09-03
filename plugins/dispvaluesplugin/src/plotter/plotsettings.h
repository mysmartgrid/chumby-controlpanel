#ifndef __PLOTSETTINGS_H__
#define __PLOTSETTINGS_H__

//#include <QtGlobal>
//#include <QVariant>

class PlotSettings
{
	public:
		PlotSettings();
		PlotSettings ( double minx, double miny, double maxx, double maxy, int ticksx , int ticksy );
		//~PlotSettings();

		//void scroll ( int dx, int dy );
		double spanX() const { return maxX - minX; }
		double spanY() const { return maxY - minY; }
		void adjust();

		double minX;
		double maxX;
		int numXTicks;
		
		double minY;
		double maxY;
		int numYTicks;

	private:
		static void adjustAxis ( double& min, double& max, int& numTicks );

};

#endif // __PLOTSETTINGS_H__
