#ifndef __QPOINTD_H__
#define __QPOINTD_H__

// this is an simple 2d-point class implemented explictly with "double" data type.
// the arm compiler normaly trasfers Qt's "qreal" into "float" instead of double
// which cuses problems when using values with a precision higher than 24bit.
// use it instead of QPointF class (most QPointF methods are not available in this class!)

class QPointD
{
	public:
		QPointD ();
		QPointD ( double xin, double yin );

		double x() const {return X;}
		double y() const {return Y;}


	private:
		double X;
		double Y;

};

#endif // __QPOINTD_H__
