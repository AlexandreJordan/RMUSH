#ifndef TOOLS_HPP
#define TOOLS_HPP

class Tools
{
public:
	static void swapInt(int& a, int& b)
	{
		int tmp = a;
		a = b;
		b = tmp;
	}};

#endif