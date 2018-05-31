#ifndef TYPES_HPP
#define TYPES_HPP

struct Point
{
    Point(const int& px, const int& py) :
        x(px), y(py) {}
    Point() :
        x(0), y(0) {}
    ~Point() {}

    int x;
    int y;
};

#endif