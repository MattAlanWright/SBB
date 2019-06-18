#ifndef _HOST_H
#define _HOST_H

#define MAX_SYMBIONTS_PER_HOST 64
#define MAX_SYMBIONT_POP_SIZE  1024

#include "Symbiont.h"

#include <vector>

class Host {

public:

    Host(int num_actions);

    void operator= (const Host &other);

    void initializeSymbionts(bool do_add_to_S = false);
    void addSymbionts(float prob_sym_addition, bool do_add_to_S = false);

    int act(const Point& p);

    float fitness;

    int num_actions;

    std::vector<float> bids;

    int max_num_symbionts;
    std::vector<Symbiont*> symbionts;

    // Static symbiont populations
    static std::vector<Symbiont*> S;
    static std::vector<Symbiont*> S_prime;
    static void cleanSymbiontPopulation();
    static void mergeSymbiontGenerations();
}


#endif //_HOST_H
