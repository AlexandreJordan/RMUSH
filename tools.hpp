#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <math.h>

class Tools
{
public:
	static void swapInt(int& a, int& b)
	{
		int tmp = a;
		a = b;
		b = tmp;
	}
	
	static float getDistance(const int& x1, const int& y1, const int& x2, const int& y2)
	{
		int deltaX = x1 - x2;
		int deltaY = y1 - y2;
	
		return sqrtf( (deltaX * deltaX) + (deltaY * deltaY) );
	}
};

#endif