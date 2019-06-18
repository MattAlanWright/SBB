#ifndef _POINT_HPP
#define _POINT_HPP

#include <vector>

class Point {

public:
    Point(std::vector<float>& features, int label);

    std::vector<float> X;
    int                y;

    static std::vector<Point> P;

    float fitness;
}

#endif //_POINT_HPP
