#ifndef _POINT_HPP
#define _POINT_HPP

#include <vector>

class Point {

public:
    Point(const std::vector<float> &features, const int &label);

    std::vector<float> X;
    int                y;

    float fitness;
};

#endif //_POINT_HPP
