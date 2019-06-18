#include "Point.hpp"

Point::Point(std::vector<float> &features, int label) {
    X       = features;
    y       = label;
    fitness = 0.0;
}
