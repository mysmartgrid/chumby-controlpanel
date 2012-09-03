//this is a prototype class for the plotter labels on x-axis.  
//it takes values of "double" type and returns a QString for a single label.
//i.e. it could take the unix timestamp value and format it to a human-readable
//date/time string...

#ifndef __CONVERTER_H__
#define __CONVERTER_H__

class Converter
{
	public:
		virtual ~Converter() {}
		virtual QString convert ( double ) = 0;
};

#endif // __CONVERTER_H__
