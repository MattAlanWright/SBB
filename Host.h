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

    void initializeSymbionts();
    void addSymbionts(float prob_sym_addition);

    static void cleanSymbiontPopulation();

private:
    int num_actions;

    int max_num_symbionts;
    std::vector<Symbiont*> symbionts;

    // Main Symbiont population
    static std::vector<Symbiont*> S;
}


#endif //_HOST_H
