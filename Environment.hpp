#ifndef _ENVIRONMENT_HPP
#define _ENVIRONMENT_HPP

#include "Point.hpp"
#include "Host.hpp"
#include "Dataset.hpp"

#include <vector>

class ClassificationEnvironment {

public:
    ClassificationEnvironment(int num_classes,
                              int p_size,
                              int p_gap,
                              int h_size,
                              int h_gap);

    Dataset            dataset;
    std::vector<Point> point_pop;
    std::vector<Host>  host_pop;

    // Outcome matrix. Recalculated each training round
    std::vector< std::vector<int> > G;

    // Initialization
    void initializeHostPop();
    void initializePointPop();

    // Population generation
    void generateHosts();
    void generatePoints();

    // Let the Hosts bid on the Points and store the results
    void calculateOutcome();

    // Evaluation methods
    void evaluateHosts();
    void evaluatePoints();

    // Remove lowest performing individuals
    void removeHosts();
    void removePoints();

    int num_classes;
    int p_size;
    int p_gap;
    int h_size;
    int h_gap;

}

#endif //_ENVIRONMENT_HPP
