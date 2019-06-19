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
#define NUM_INPUTS                  9
#define NUM_CLASSES                 2
#define NUM_REGISTERS               8
#define SAFE_DIV_RESULT             0.0
#define MIN_SOURCE_VAL              0.00001
#define MIN_REGISTER_VAL           -10.0e3f
#define MAX_REGISTER_VAL            10.0e3f
#define MIN_ERROR_VAL               0.0f
#define MAX_ERROR_VAL               10.0e3f
#define NUM_INITIAL_INSTRUCTIONS    64
#define MIN_NUM_INSTRUCTIONS        16
#define MAX_NUM_INSTRUCTIONS        1024
#define MUTATION_RATE               0.2


class Symbiont {

    public:
        Symbiont(int action = -1);

        // Variation operations
        void mutate();
        void mutateAction(int num_actions);

        // Evaluation and training methods
        float bid(std::vector<float>& input);

        void printInstructions(int num = -1);
        static void printInstruction(Instruction &instruction);

        inline int  getReferenceCount()       { return reference_count; };
        inline void incrementReferenceCount() { reference_count++; };
        inline void decrementReferenceCount() { if( reference_count > 0 ) reference_count--; };

        void initializeInstructions();
        void executeInstruction(Instruction& instruction, float input[NUM_INPUTS]);

        // Class constants and objects
        static const int source_mod_value[2];

        const int   num_initial_instructions;
        const float mutation_rate;
        const int   max_source_range;

        // Primary GP components
        int action;
        int reference_count;
        float registers[NUM_REGISTERS];
        std::vector<Instruction> instructions;
};

#endif //_PROGRAM_H
