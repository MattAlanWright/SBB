#ifndef _HOST_H
#define _HOST_H

#define MAX_SYMBIONTS_PER_HOST 64
#define MAX_SYMBIONT_POP_SIZE  1024

#include "Symbiont.hpp"
#include "Point.hpp"

#include <vector>

class Host {

public:

    Host(int num_actions);

    void operator= (const Host &other);

    void initializeSymbionts();

    void mutateHost(float prob_symbiont_mutation, float prob_symbtiont_action_mutation);

    int act(const Point& p);

    float fitness;

    int num_actions;

    std::vector<float> bids;

    int max_num_symbionts;
    std::vector<Symbiont*> symbionts;
    void updateSymbiontRefs();

    // Variation operators
    void removeSymbionts(float prob_symbiont_removal);
    void addSymbionts(float prob_symbiont_addition);


    // Static symbiont populations
    static std::vector<Symbiont*> S;
    static std::vector<Symbiont*> S_prime;

    // Symbiont population methods
    static void resetSymbiontPopulationRefs();
    static void cleanSymbiontPopulation();
    static void mergeSymbiontGenerations();
};


#endif //_HOST_H
