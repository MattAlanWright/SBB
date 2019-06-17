#include "Host.h"

#include "Symbiont.h"

// Effolkronium random library
#include "random.hpp"

// Symbiont population
Host::std::vector<Symbiont*> S(MAX_SYMBIONT_POP_SIZE);


Host::Host(int num_actions)
    : num_actions(num_actions)
{
    max_num_symbionts = Random::get<int>(0, MAX_SYMBIONTS_PER_HOST);
    symbionts.reserve(max_num_symbionts);
}


void Host::operator= (const Host &other) {

    num_actions = other.num_actions;
    max_num_symbionts = other.max_num_symbionts;

    symbionts = other.symbionts;

    for( auto it = symbionts.begin(); it != symbionts.end(); it++ ) {
        (*it)->incrementReferenceCount();
    }
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

    // Set their reference counts to 1
    s1->incrementReferenceCount();
    s2->incrementReferenceCount();

    // Add to the host's list and the main population
    symbionts.push_back(s1);
    symbionts.push_back(s2);

    S.push_back(s1);
    S.push_back(s2);
}


void Host::mutateHost(float prob_symbiont_mutation) {

    bool symbiont_mutated = false;

    while( !symbiont_mutated ) {
        for( auto it = symbionts.begin(); it != symbionts.end(); it++ ) {
            if( Random::get<float>(0.0, 1.0) >= prob_symbiont_mutation ) continue;

            Symbiont* s       = (*it);
            Symbiont* s_prime = new Symbiont();

            // Copy original symbiont
            *s_prime = *s;

            // Change host's pointer to s_prime
            (*it) = s_prime;

            // Mutate s_prime
            s_prime->mutate();

            symbiont_mutated = true;
        }
    }
}


void Host::addSymbionts(float prob_symbiont_addition) {

    float b = 1.0;
    while( b > Random::get<float>(0.0, 1.0) && symbionts.size() < max_num_symbionts ) {
        int pop_index = Random::get<int>(0, S.size());
        Symbiont *s = S[pop_index];
        symbionts.push_back(s);
        s->incrementReferenceCount();

        b *= prob_symbiont_addition;
    }
}


void Host::removeSymbionts(float prob_symbiont_removal) {

    float b = 1.0;
    while( b > Random::get<float>(0.0, 1.0) && symbionts.size() > 2 ) {

        int sym_index = Random::get<int>(0, symbionts.size());
        Symbiont *s = symbionts[sym_index];
        s->decrementReferenceCount();
        delete symbionts[sym_index];
        b *= prob_symbiont_removal;
    }

}


// Remove all un-used Symbionts from the main population
void Host::cleanSymbiontPopulation() {

    auto it = S.begin();
    while( it != S.end() ) {
        if( (*it)->getReferenceCount() == 0 ) {
            delete *it;
            it = S.erase(it);
        } else {
            it++;
        }
    }
}











