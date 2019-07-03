#ifndef _ENVIRONMENT_HPP
#define _ENVIRONMENT_HPP

#include "Point.hpp"
#include "Host.hpp"
#include "UniformDataset.hpp"

#include <vector>

class ClassificationEnvironment {

public:
    ClassificationEnvironment(int num_classes,
                              int num_features,
                              int num_samples,
                              int h_size,
                              int h_gap,
                              int tau,
                              const std::vector<std::vector<float>> &X,
                              const std::vector<int>                &y);

    UniformDataset dataset;

    int num_classes;
    int tau;
    int h_size;
    int h_gap;

    // Core training alorithm
    void train(int num_generations);

    // Initialization
    std::vector<Host> initializeHostPop();

    // Population generation
    void generateHosts(std::vector<Host> &host_pop);
    std::vector<Point> generatePoints();

    // Evaluation methods
    void evaluateHosts();

    // Remove lowest performing individuals
    void removeHosts(std::vector<Host> &);

    // Clean up Symbiont population
    void cleanSymbiontPopulation(std::vector<Host> &host_pop);
};

#endif //_ENVIRONMENT_HPP
