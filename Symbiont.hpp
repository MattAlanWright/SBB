#ifndef __PROGRAM_H
#define __PROGRAM_H

#include "Instruction.hpp"

#include <iostream>
#include <vector>

// Effolkronium random library
#include "random.hpp"
using Random = effolkronium::random_static;

// Class definitions
// These things are much easier to have hard-coded since they are used as static
// array sizes. Use definitions for now and move to a templated class later.
#define NUM_INPUTS                  4
#define NUM_CLASSES                 3
#define NUM_REGISTERS               8
#define MIN_REGISTER_VAL           -10.0e3f
#define MAX_REGISTER_VAL            10.0e3f
#define NUM_INITIAL_INSTRUCTIONS    (NUM_REGISTERS * NUM_REGISTERS)
#define MIN_NUM_INSTRUCTIONS        16
#define MAX_NUM_INSTRUCTIONS        1024
#define MUTATION_RATE               0.2


class Symbiont {

    public:
        Symbiont(int action = -1);

        // Evaluation and training methods
        float bid(const std::vector<float> &input);

        bool is_referenced;

        void initializeInstructions();
        void executeInstruction(Instruction& instruction, const std::vector<float> &input);

        // Class constants and objects
        static const int source_mod_value[2];

        int max_source_range;

        // Primary GP components
        int                      action;
        std::vector<float>       registers;
        std::vector<Instruction> instructions;

        // Variation operations
        void mutate();
        void mutateAction(int num_actions);
};

#endif //_PROGRAM_H
