#ifndef _UNIFORM_DATASET_HPP
#define _UNIFORM_DATASET_HPP

#include <vector>

#include "Point.hpp"

class UniformDataset {

public:
    UniformDataset(const std::vector< std::vector<float> > &X,
                   const std::vector<int>                  &y);

    // One-dimensional vector, indexed into as:
    //
    //   Point point = dataset[sample number]
    //
    std::vector<Point> dataset;

    std::vector<Point> getRandomExemplars(int tau);
};

#endif //_UNIFORM_DATASET_HPP
