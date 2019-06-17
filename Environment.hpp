#ifndef _ENVIRONMENT_HPP
#define _ENVIRONMENT_HPP

#include "Point.hpp"
#include "Dataset.hpp"

#include <vector>

class Environment {

public:
    Environment();

    Dataset            dataset;
    std::vector<Point> point_pop;

}

#endif //_ENVIRONMENT_HPP
