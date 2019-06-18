#include "Host.h"

#include "Symbiont.h"

// Effolkronium random library
#include "random.hpp"
using Random = effolkronium::random_static;

// Symbiont population
Host::std::vector<Symbiont*> S;
Host::std::vector<Symbiont*> S_prime;


Host::Host(int num_actions) :
    num_actions(num_actions),
    fitness(0.0)
{
    max_num_symbionts = Random::get<int>(0, MAX_SYMBIONTS_PER_HOST);
    symbionts.reserve(max_num_symbionts);
    bids.reserve(max_num_symbionts);
}


void Host::initializeSymbionts() {

    // Create two random and unique actions
    int a1, a2;
    a1 = a2 = Random::get<int>(0, num_actions);
    while(a1 == a2) {
        a2 = Random::get<int>(0, num_actions);
    }

    // Create two new Symbionts with the given actions
    Symbiont* s1 = new Symbiont(a1);
    Symbiont* s2 = new Symbiont(a2);

    // Add to the host's list and the main population
    symbionts.push_back(s1);
    symbionts.push_back(s2);

    S.push_back(s1);
    S.push_back(s2);
}


void Host::mutateHost(float prob_symbiont_mutation, float prob_symbtiont_action_mutation) {

    bool symbiont_mutated = false;

    while( !symbiont_mutated ) {
        for( auto it = symbionts.begin(); it != symbionts.end(); it++ ) {
            if( Random::get<float>(0.0, 1.0) >= prob_symbiont_mutation ) {
                continue;
            }

            Symbiont* s       = (*it);
            Symbiont* s_prime = new Symbiont();

            // Copy original symbiont
            *s_prime = *s;

            // Change host's pointer to s_prime
            (*it) = s_prime;

            // Mutate s_prime
            s_prime->mutate();

            // Add new Symbiont to S_prime population
            S_prime.push_back(s_prime);

            symbiont_mutated = true;

            // Possibly also alter Symbtiont's action
            if( Random::get<float>(0.0, 1.0) >= prob_symbtiont_action_mutation ) {
                continue;
            }

            // Change Symbiont's action
            s_prime->mutateAction(num_actions);
        }
    }
}


void Host::removeSymbionts(float prob_symbiont_removal) {

    float b = 1.0;
    while( b > Random::get<float>(0.0, 1.0) && symbionts.size() > 2 ) {

        int sym_index = Random::get<int>(0, symbionts.size());
        Symbiont *s = symbionts[sym_index];
        delete symbionts[sym_index];
        b *= prob_symbiont_removal;
    }

}


void Host::addSymbionts(float prob_symbiont_addition) {

    float b = 1.0;
    while( b > Random::get<float>(0.0, 1.0) && symbionts.size() < max_num_symbionts ) {
        int pop_index = Random::get<int>(0, S.size());
        Symbiont *s = S[pop_index];
        symbionts.push_back(s);

        b *= prob_symbiont_addition;
    }
}


int Host::act(const Point& p) {

    // Allow each Symbiont to act on the data point
    bids.clear();
    for(int i = 0; i < symbionts.size(); i++) {
        float bid = symbionts[i].bid(p.X);
        bids.push_back(bid);
    }

    // Get the highest bidder
    auto result = std::max_element(bids.begin(), bids.end());
    auto bidder = std::distance(bids.begin(), result);

    // Get the action of the highest bidder
    return symbionts[bidder].action;
}


void Host::updateSymbiontRefs() {
    for(Symbiont * s : symbionts) {
        s->is_referenced = true;
    }
}


void Host::resetSymbiontPopulationRefs() {
    for(Symbiont * s : S) {
        s->is_referenced = false;
    }
}


// Remove all un-used Symbionts from the main population
void Host::cleanSymbiontPopulation() {

    auto it = S.begin();
    while( it != S.end() ) {
        if( (*it)->is_referenced == false ) {
            delete *it;
            it = S.erase(it);
        } else {
            it++;
        }
    }
}


// Remove all un-used Symbionts from the main population
void Host::mergeSymbiontGenerations() {

    // Merge S_prime into S and then clear out S_prime
    S.insert(S.end(), S_prime.begin(), S_prime.end());

    S_prime.clear();
}
