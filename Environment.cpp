#include "Environment.hpp"

#include <cmath>

// Effolkronium random library
#include "random.hpp"
using Random = effolkronium::random_static;

#define PROB_SYMBIONT_ADDITION      0.2
#define PROB_SYMBIONT_REMOVAL       0.2
#define PROB_SYMBIONT_MUTATION      0.1
#define PROB_ACTION_MUTATION        0.1


ClassificationEnvironment::ClassificationEnvironment(int num_classes,
                                                     int num_features,
                                                     int num_samples,
                                                     int p_size,
                                                     int p_gap,
                                                     int h_size,
                                                     int h_gap,
                                                     std::vector<std::vector<float>> &X,
                                                     std::vector<float>              &y) :

    num_classes(num_classes),
    p_size(p_size),
    p_gap(p_gap),
    h_size(h_size),
    h_gap(h_gap),
    dataset(X, y, num_classes, num_features, num_samples)
{
    // Reserve space for outcome matrix
    G.resize(h_size);
    for(int i = 0; i < h_size; i++) {
        G[i].resize(p_size);
    }

    initializeHostPop();
    initializePointPop();
}


void ClassificationEnvironment::train(int num_generations) {

    for(int t = 0; t < num_generations; t++) {

        // Generate new individuals
        generatePoints();
        generateHosts();

        // Evaluate all hosts on all points
        calculateOutcomeMatrix();
        evaluatePoints();
        evaluateHosts();

        // Remove low-performing points and hosts
        removeHosts();
        removePoints();

        std::cout << '\r' << std::flush << "Best fitness: " << host_pop[0].fitness;
    }
}


void ClassificationEnvironment::initializeHostPop() {

    // 1. Reserve space for h_size hosts
    host_pop.reserve(h_size);

    // 2. Create h_size - h_gap hosts
    for(int i = 0 i < (h_size - h_gap); i++) {
        Host h(num_classes);
        h.initializeSymbionts(true);
        host_pop.push_back(h);
    }

    // 3. Fill out the hosts with references to other Symbionts
    // Step 2 must have already taken place to ensure the full
    // Symbiont population exists.
    for(Host& h : host_pop) {
        h.addSymbionts(PROB_SYMBIONT_ADDITION, true);
    }
}


void ClassificationEnvironment::initializePointPop() {

    for( int i = 0; i < (p_size - p_gap); i++ ) {
        Point p = dataset.getRandomExemplar();
        point_pop.push_back(p);
    }
}


void ClassificationEnvironment::generateHosts() {

    for( int i = 0; i < h_gap; i++ ) {

        // Choose a random host
        random_host_index = Random::get<int>(0, h_size);

        // Create a copy of the Host with references to Symbionts
        Host h = host_pop[random_host_index];

        h.removeSymbionts(PROB_SYMBIONT_REMOVAL);
        h.addSymbionts(PROB_SYMBIONT_ADDITION);
        h.mutateHost(PROB_SYMBIONT_MUTATION, PROB_ACTION_MUTATION);

        host_pop.push_back(h);
    }
}


void ClassificationEnvironment::generatePoints() {

    for( int i = 0; i < p_gap; i++ ) {
        Point p = dataset.getRandomExemplar();
        point_pop.push_back(p);
    }
}


void ClassificationEnvironment::calculateOutcomeMatrix() {
    for( int h_index = 0; h_index < h_size; h_index++ ) {
        for( int p_index = 0; p_index < p_size; p_index++ ) {

            Point& p        = point_pop[p_index];
            int    h_action = host_pop[h_index].act(p);

            if( h_action == label ) {
                G[h_index][p_index] = 1;
            } else {
                G[h_index][p_index] = 0;
            }
        }
    }
}


void ClassificationEnvironment::evaluatePoints() {

    std::vector<float> c(point_pop.size());

    for( int k = 0; k < p_size; k++ ) {
        for( int i = 0; i < h_size; i++ ) {
            c[k] += G[i][k];
        }

        if(c[k] > 0) {
            point_pop[k].fitness = 1.0 + (1.0 - c[k])/h_size;
        } else {
            point_pop[k].fitness = 0.0;
        }
    }
}


void ClassificationEnvironment::evaluateHosts() {

    for( int i = 0; i < h_size; i++ ) {
        float fitness = 0.0;
        for( int k = 0; k < p_size; k++ ) {

            float denominator = 0.0;
            for( int j = 0; j < h_size; j++ ) {
                denominator += G[j][k];
            }

            float fitness += std::pow(G[i][k] / denominator, 3);
        }

        host_pop[i].fitness = temp;
    }
}


void ClassificationEnvironment::removePoints() {

    // Sort points from highest to lowest fitness
    std::sort(point_pop.begin(), point_pop.end(), [](Point& a, Point& b) {
        return a.fitness > b.fitness;
    });

    // Remove the p_gap lowest ones
    point_pop.erase(point_pop.begin() + (p_size - p_gap), point_pop.end());
}


void ClassificationEnvironment::removeHosts() {

    // Sort hosts from highest to lowest fitness
    std::sort(host_pop.begin(), host_pop.end(), [](Host& a, Host& b) {
        return a.fitness > b.fitness;
    });

    // Before removing the h_gap lowest performing Symbionts,
    // ensure the Symbionts' ref counts are decremented.
    h_keep = h_size - h_gap;
    for( int i = h_keep; i < h_size; i++ ) {
        host_pop[i].decrementSymbiontRefCounts();
    }

    // Remove the h_gap lowest performing hosts
    host_pop.erase(host_pop.begin() + h_keep, host_pop.end());
}


ClassificationEnvironment::cleanSymbiontPopulation() {
    Host::resetSymbiontPopulationRefs();
    for(Host& h : host_pop) {
        h.updateSymbiontRefs();
    }
    Host::cleanSymbiontPopulation();
}
