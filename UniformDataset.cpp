#include "UniformDataset.hpp"

#include <iostream>

// Effolkronium random library
#include "random.hpp"
using Random = effolkronium::random_static;


UniformDataset::UniformDataset(const std::vector< std::vector<float> > &X,
                               const std::vector<int>                  &y)
{
    if( X.size() != y.size() ) {
        std::cout << "UniformDataset - Error - X and y are of unequal lengths" << std::endl;
    }

    // y[i] = Integer class label
    // X[i] = std::vector<float> of features
    for(int i = 0; i < X.size(); i++) {
        Point p(X[i], y[i]);
        dataset.push_back(p);
    }
}

std::vector<Point> UniformDataset::getRandomExemplars(int tau) {

    // Shuffle dataset uniformly and take the first tau Points
    Random::shuffle(dataset);

    auto start = dataset.begin();
    auto end   = dataset.begin() + tau;
    std::vector<Point> subset(start, end);

    return subset;
}
