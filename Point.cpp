#include "Point.hpp"

Point::Point(const std::vector<float> &features, const int &label) {
    X       = features;
    y       = label;
    fitness = 0.0;
}
