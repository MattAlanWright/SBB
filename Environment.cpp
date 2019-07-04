#include "Environment.hpp"

#include <cmath>

// Effolkronium random library
#include "random.hpp"
using Random = effolkronium::random_static;

#define PROB_SYMBIONT_ADDITION      0.1
#define PROB_SYMBIONT_REMOVAL       0.1
#define PROB_SYMBIONT_MUTATION      1//0.1
#define PROB_ACTION_MUTATION        1//0.1


ClassificationEnvironment::ClassificationEnvironment(
    int num_classes,
    int num_features,
    int num_samples,
    int h_size,
    int h_gap,
    int tau,
    const std::vector<std::vector<float>> &train_X,
    const std::vector<int>                &train_y,
    const std::vector<std::vector<float>> &test_X,
    const std::vector<int>                &test_y) :

    num_classes(num_classes),
    h_size(h_size),
    h_gap(h_gap),
    tau(tau),
    dataset(train_X, train_y),
    test_set(test_X, test_y) {}


void
ClassificationEnvironment::train(int num_generations) {

    // Reserve space for outcome matrix
    std::vector< std::vector<float> > G;
    G.resize(h_size);
    for( int i = 0; i < G.size(); i++ ) {
        G[i].resize(tau);
    }

    std::vector<Host> hosts = initializeHostPop();

    for(int t = 0; t < num_generations; t++) {

        // Generate new batch of Hosts
        generateHosts(hosts);

        // Generate new individuals
        std::vector<Point> points = generatePoints();

        // Calculate the outcome of each Learner on each Point
        for( int i = 0; i < h_size; i++ ) {
            for( int k = 0; k < tau; k++ ) {
                float action = hosts[i].act(points[k].X);
                if( action == points[k].y ) {
                    G[i][k] = 1.0;
                } else {
                    G[i][k] = 0.0;
                }
            }
        }

        // Sum across rows of G to calculate fitness
        std::vector<float> denom(G[0].size(), 0.0);
        for( int k = 0; k < tau; k++ ) {
            for( int i = 0; i < h_size; i++ ) {
                denom[k] += G[i][k];
            }
        }

        for( int k = 0; k < tau; k++ ) {
            for( int i = 0; i < h_size; i++ ) {
                G[i][k] /= denom[k];
            }
        }

        for( int i = 0; i < h_size; i++ ) {
            hosts[i].fitness = 0.0;
            for( int k = 0; k < tau; k++ ) {
                hosts[i].fitness += G[i][k];
            }
        }

        removeHosts(hosts);

        cleanSymbiontPopulation(hosts);

        std::cout << '\r' << std::flush;
        std::cout << "Generation: " << t <<
                     " Accuracy: " << hosts[0].accuracy(points) <<
                     " Recall: " << hosts[0].detectionRate(points);
    }

    std::cout << std::endl;

    std::cout << "Training accuracy: " << hosts[0].accuracy(dataset.dataset) << std::endl;
    std::cout << "Training recall: " << hosts[0].detectionRate(dataset.dataset) << std::endl;
    std::cout << "Test accuracy: " << hosts[0].accuracy(test_set.dataset) << std::endl;
    std::cout << "Test recall: " << hosts[0].detectionRate(test_set.dataset) << std::endl;
}


std::vector<Host>
ClassificationEnvironment::initializeHostPop() {

    std::vector<Host> host_pop;

    // 1. Reserve space for h_size hosts
    host_pop.reserve(h_size);

    // 2. Create h_size - h_gap hosts
    for(int i = 0; i < (h_size - h_gap); i++) {
        Host h(num_classes);
        h.initializeSymbionts();
        host_pop.push_back(h);
    }

    // 3. Fill out the hosts with references to other Symbionts
    // Step 2 must have already taken place to ensure the full
    // Symbiont population exists to sample.
    for(Host &host : host_pop) {
        host.addSymbionts(PROB_SYMBIONT_ADDITION);
    }

    return host_pop;
}


void
ClassificationEnvironment::generateHosts(std::vector<Host> &host_pop) {

    for( int i = 0; i < h_gap; i++ ) {

        // Choose a random host
        int random_host_index = Random::get<int>(0, (h_size - h_gap) - 1);

        // Create a copy of the Host with references to Symbionts
        Host h = host_pop[random_host_index];

        h.removeSymbionts(PROB_SYMBIONT_REMOVAL);
        h.addSymbionts(PROB_SYMBIONT_ADDITION);
        h.mutateHost(PROB_SYMBIONT_MUTATION, PROB_ACTION_MUTATION);

        host_pop.push_back(h);
    }
}


std::vector<Point>
ClassificationEnvironment::generatePoints() {
    return dataset.getRandomExemplars(tau);
}

/*
void ClassificationEnvironment::evaluateHosts() {

    for( int i = 0; i < host_pop.size(); i++ ) {
        float fitness = 0.0;
        for( int k = 0; k < point_pop.size(); k++ ) {

            int denominator = 0;
            for( int j = 0; j < host_pop.size(); j++ ) {
                denominator += G[j][k];
            }

            fitness += std::pow((float)G[i][k] / (float)denominator, 1.0);
        }

        host_pop[i].fitness = fitness;
    }
}
*/


void
ClassificationEnvironment::removeHosts(std::vector<Host> &host_pop) {

    // Sort hosts from highest to lowest fitness
    std::sort(host_pop.begin(), host_pop.end(), [](Host& a, Host& b) {
        return a.fitness > b.fitness;
    });

    // Remove the h_gap lowest performing hosts
    int h_keep = h_size - h_gap;
    host_pop.erase(host_pop.begin() + h_keep, host_pop.end());
}


void
ClassificationEnvironment::cleanSymbiontPopulation(std::vector<Host> &host_pop) {
    Host::resetSymbiontPopulationRefs();
    for(Host& h : host_pop) {
        h.updateSymbiontRefs();
    }
    Host::cleanSymbiontPopulation();
}
