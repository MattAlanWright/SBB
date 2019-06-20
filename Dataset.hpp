#ifndef _DATASET_HPP
#define _DATASET_HPP

#include <vector>

#include "Point.hpp"

class Dataset {

public:
    Dataset(const std::vector< std::vector<float> > &X,
            const std::vector<int>                  &y,
            int   num_classes,
            int   num_features,
            int   num_samples);

    int num_classes;
    int num_features;
    int num_samples;

    // Multi-dimensional vector, indexed into as:
    //
    //   Point point = dataset[class][sample number]
    //
    std::vector< std::vector<Point> > dataset;

    Point getRandomExemplar();
    void insertPoint(const Point &p);
};

#endif //_DATA_HPP
