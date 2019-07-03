#ifndef _HOST_H
#define _HOST_H

#define MAX_SYMBIONTS_PER_HOST 64
#define MAX_SYMBIONT_POP_SIZE  1024

#include "Symbiont.hpp"
#include "Point.hpp"

#include <vector>

class Host {

public:

    Host(int num_actions   =  3,
         int num_registers =  8,
         int num_inputs    =  4);

    //////////////////////////////////////////////////////////
    // Data members

    // Updated at every generation. Represents the relative
    // fitness of the host on the current exemplar population.
    float fitness;

    // Analogous to the number of classes. This is the upper
    // bound (not inclusive) on possible action values.
    int num_actions;

    // Symbiont properties
    int num_registers;
    int num_inputs;

    // Stochastically chosen maximum number of possible Symbionts
    // in the host.
    int max_num_symbionts;

    // Vector of pointers into the static Symbiont population.
    // These are the host's Symbionts.
    std::vector<Symbiont*> symbionts;


    //////////////////////////////////////////////////////////
    // Instance methods

    // Primary classification method. Returns the predicted
    // class label of the Point p.
    int act(const std::vector<float> &X);

    // Create two unique Symbionts, added to both the static
    // Symbiont population and the host's own symbiont list.
    // Every Host creates a minimum of two unique Symbionts when
    // it is created, each with a unique action. Since this affects
    // the static Symbtion population, this is kept out of the constructor
    // so that temporary Hosts can safely be created.
    void initializeSymbionts();

    // Set all of the host's Symbtionts 'is_referenced'
    // flags to true. It is up to the owner of the hosts
    // to reset all of these flags to false ever generation.
    void updateSymbiontRefs();

    // Variation operators
    void removeSymbionts(float prob_symbiont_removal);
    void addSymbionts(float prob_symbiont_addition);
    void mutateHost(float prob_symbiont_mutation, float prob_symbtiont_action_mutation);


    //////////////////////////////////////////////////////////
    // Static Symbiont Populations

    // Primary static Symbiont population
    static std::vector<Symbiont*> S;

    // Secondary static Symbiont population. Used to hold newly-created
    // Symbionts while keeping the primary population frozen.
    static std::vector<Symbiont*> S_prime;

    // Symbiont population methods
    static void resetSymbiontPopulationRefs();
    static void cleanSymbiontPopulation();
    static void mergeSymbiontGenerations();
};


#endif //_HOST_H
