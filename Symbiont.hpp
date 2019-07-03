#ifndef _SYMBIONT_HPP
#define _SYMBIONT_HPP

#include "Instruction.hpp"

#include <iostream>
#include <vector>

// Effolkronium random library
#include "random.hpp"
using Random = effolkronium::random_static;

// Class definitions
// These things are much easier to have hard-coded since they are used as static
// array sizes. Use definitions for now and move to a templated class later.
#define MIN_REGISTER_VAL           -10.0e3f
#define MAX_REGISTER_VAL            10.0e3f
#define MIN_NUM_INSTRUCTIONS        16
#define MAX_NUM_INSTRUCTIONS        96

// Stoachastic mutation probabilities
#define PROB_DEL_INST     0.1
#define PROB_ADD_INST     0.1
#define PROB_MUT_INST     0.1
#define PROB_SWP_INST     0.1

class Symbiont {

public:

    Symbiont(int action        = -1,
             int num_registers =  8,
             int num_actions   =  3,
             int num_inputs    =  4);

    //////////////////////////////////////////////////////////
    // Data members

    int num_registers;
    int num_actions;

    int source_mod_value[2];

    // Primary GP components
    int                      action;
    std::vector<float>       registers;
    std::vector<Instruction> instructions;

    bool is_referenced;


    //////////////////////////////////////////////////////////
    // Instance methods

    // Evaluation and training methods
    float bid(const std::vector<float> &input);

    void initializeInstructions();
    void executeInstruction(const Instruction& instruction,
                            const std::vector<float> &input);

    // Variation operations
    void mutate();
    void deleteRandomInstruction(float prob_delete);
    void addRandomInstruction(float prob_add);
    void mutateRandomInstruction(float prob_mutate);
    void swapRandomInstructions(float prob_swap);
    void mutateAction(int num_actions);
};

#endif //_SYMBIONT_HPP
