#include "Host.hpp"

#include "Symbiont.hpp"

// Effolkronium random library
#include "random.hpp"
using Random = effolkronium::random_static;

// Symbiont populations
std::vector<Symbiont*> Host::S;
std::vector<Symbiont*> Host::S_prime;


Host::Host(int num_actions,
           int num_registers,
           int num_inputs) :

    num_actions(num_actions),
    num_registers(num_registers),
    num_inputs(num_inputs),
    fitness(0.0)
{
    max_num_symbionts = Random::get<int>(2, MAX_SYMBIONTS_PER_HOST);
}


void Host::initializeSymbionts() {

    // Create two random and unique actions
    int a1, a2;
    a1 = a2 = Random::get<int>(0, num_actions - 1);
    while(a1 == a2) {
        a2 = Random::get<int>(0, num_actions - 1);
    }

    // Create two new Symbionts with the given actions
    Symbiont *s1 = new Symbiont(a1, num_registers, num_actions, num_inputs);
    Symbiont *s2 = new Symbiont(a2, num_registers, num_actions, num_inputs);

    // Add to the host's list and the main population
    symbionts.push_back(s1);
    symbionts.push_back(s2);

    S.push_back(s1);
    S.push_back(s2);
}


void Host::mutateHost(float prob_symbiont_mutation, float prob_symbiont_action_mutation) {

    bool symbiont_mutated = false;

    while( !symbiont_mutated ) {
        if( symbionts.size() < 2 ) {
            std::cout << "Something bad happened!" << std::endl;
        }
        for( int i = 0; i < symbionts.size(); i++ ) {
            if( Random::get<float>(0.0, 1.0) >= prob_symbiont_mutation ) {
                continue;
            }

            Symbiont *s       = symbionts[i];
            Symbiont *s_prime = new Symbiont();

            // Copy original symbiont
            *s_prime = *s;

            // Change host's pointer to s_prime
            symbionts[i] = s_prime;

            // Mutate s_prime
            s_prime->mutate();

            // Add new Symbiont to S_prime population
            S.push_back(s_prime);

            symbiont_mutated = true;

            // Possibly also alter Symbtiont's action
            if( Random::get<float>(0.0, 1.0) >= prob_symbiont_action_mutation ) {
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

        int sym_index = Random::get<int>(0, symbionts.size() - 1);
        symbionts.erase(symbionts.begin() + sym_index);
        b *= prob_symbiont_removal;
    }

}


void Host::addSymbionts(float prob_symbiont_addition) {

    float b = 1.0;
    while( b > Random::get<float>(0.0, 1.0) && symbionts.size() < max_num_symbionts ) {

        Symbiont *s = nullptr;
        while( s == nullptr ) {

            // Get random Symbiont from the primary population S
            int pop_index = Random::get<int>(0, S.size() - 1);
            Symbiont * sym = S[pop_index];

            // Ensure that sym isn't already in this Host's symbiont list
            if ( std::find(symbionts.begin(), symbionts.end(), sym) == symbionts.end() ) {
                s = sym;
            }
        }

        symbionts.push_back(s);

        b *= prob_symbiont_addition;
    }
}


float Host::accuracy(const std::vector< std::vector<float> > &X,
                     const std::vector<int> &y) {

    // Ensure dataset is valid
    if( X.size() != y.size() ) {
        std::cout << "Host::accuracy - Error - X and y are of unequal lengths" << std::endl;
    }

    // Act on each exemplar and compare to the true label
    float num_correct = 0.0;
    for( int i = 0; i < X.size(); i++ ) {
        int action = act(X[i]);
        if( action == y[i]) num_correct += 1.0;
    }

    return num_correct / X.size();
}


float Host::accuracy(const std::vector<Point> points) {

    // Act on each exemplar and compare to the true label
    float num_correct = 0.0;
    for( const Point &point : points ) {
        int action = act(point.X);
        if( action == point.y) num_correct += 1.0;
    }

    return num_correct / points.size();
}


float Host::detectionRate(const std::vector<Point> points) {
    float detection_rate = 0.0;

    std::vector<int> true_positives(num_actions, 0);
    std::vector<int> false_negatives(num_actions, 0);

    for( const Point &point : points ) {
        int action = act(point.X);
        if( action == point.y ) {
            true_positives[point.y]++;
        } else {
            false_negatives[point.y]++;
        }
    }

    for( int i = 0; i < num_actions; i++ ) {
        if( true_positives[i] == 0 ) {
            detection_rate += 0;
        } else {
            detection_rate += (float)true_positives[i] / (float)(true_positives[i] + false_negatives[i]);
        }
    }

    return detection_rate / num_actions;
}


int Host::act(const std::vector<float> &X) {

    // Allow each Symbiont to act on the data point
    // Note the highest bid and the action associated with it
    float max_bid   = -1.0;
    int  max_action = -1;
    for(int i = 0; i < symbionts.size(); i++) {
        float bid = symbionts[i]->bid(X);
        if( bid > max_bid ) {
            max_bid    = bid;
            max_action = symbionts[i]->action;
        }

    }

    return max_action;
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


// Merge S_prime into S and then clear out S_prime
void Host::mergeSymbiontGenerations() {

    S.insert(S.end(), S_prime.begin(), S_prime.end());

    S_prime.clear();
}
