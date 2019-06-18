#include <algorithm>
#include <array>
#include <functional>
#include <numeric>
#include <math.h>

#include "Symbiont.h"

float sigmoid(float z) {
   return 1.0 / (1.0 + exp(-z));
}

// Modulo operands. The modulo operation is always performed on the source
// index, which is always between 0 and max(NUM_INPUTS, NUM_REGISTERS).
// This ensures that the index is always within the bounds of what we're
// indexing into.
const int Symbiont::source_mod_value[2] = {
    NUM_REGISTERS,
    NUM_INPUTS
};


Symbiont::Symbiont(int action)
    : action(action),
      reference_count(0),
      max_source_range(std::max(NUM_REGISTERS, NUM_INPUTS))
{
    // If action not specified, select at random
    if( action == -1 ) action = Random::get<int>(0, NUM_CLASSES);

    // Reserve space in vectors for maximum number of possible instructions
    instructions.reserve(MAX_NUM_INSTRUCTIONS);

    // Initialize random instructions
    initializeInstructions();
}


void Symbiont::initializeInstructions() {
    Instruction instruction;
    for(int i = 0; i < NUM_INITIAL_INSTRUCTIONS; i++) {
        instruction.randomize();
        instructions.push_back(instruction);
    }
 }


void Symbiont::mutate() {
    if(Random::get<float>(0.0f, 1.0f) > mutation_rate) {
        return;
    }

    int instruction_to_mutate = Random::get<int>(0, instructions.size());
    instructions[instruction_to_mutate].toggleRandomBit();
}


void Symbiont::mutateAction(int num_actions) {
    int new_action = action;
    while(new_action == action) {
        new_action = Random::get<int>(0, num_actions);
    }
    action = new_action;
}


float Symbiont::bid(float input[NUM_INPUTS]) {
    // Zero out registers
    for(int i = 0; i < NUM_REGISTERS; i++) registers[i] = 0.0;

    for(Instruction instruction : instructions) {
        executeInstruction(instruction, input);
    }

    return sigmoid(registers[0]);
}


void Symbiont::executeInstruction(Instruction& instruction, float input[NUM_INPUTS]) {

    int mode         = instruction.getMode();
    int target_index = instruction.getTarget();
    int op_code      = instruction.getOp();
    int source_index = instruction.getSource();

    source_index %= source_mod_value[mode];

    float * source;
    if(mode == INST_REGISTER_MODE) {
        source = registers;
    } else {
        source = input;
    }

    float val;
    switch(op_code) {
        case INST_ADDITION:
            val = registers[target_index] + source[source_index];
            break;
        case INST_SUBTRACTION:
            val = registers[target_index] - source[source_index];
            break;
        case INST_MULTIPLICATION:
            val = registers[target_index] * source[source_index];
            break;
        case INST_DIVISION:
            if( source[source_index] < MIN_SOURCE_VAL ) {
                val = SAFE_DIV_RESULT;
            } else {
                val = registers[target_index] / source[source_index];
            }
            break;
    }
    registers[target_index] = std::clamp(val, MIN_REGISTER_VAL, MAX_REGISTER_VAL);
}


void getTournamentCompetitors(std::array<int, 4>& competitor_indices, int population_size) {

    // Get 4 random competitors
    competitor_indices[0] = Random::get<int>(0, population_size - 1);

    do {
        competitor_indices[1] = Random::get<int>(0, population_size -1);
    } while (competitor_indices[0] == competitor_indices[1]);

    do {
        competitor_indices[2] = Random::get<int>(0, population_size - 1);
    } while (competitor_indices[2] == competitor_indices[0] ||
             competitor_indices[2] == competitor_indices[1]);

    do {
        competitor_indices[3] = Random::get<int>(0, population_size - 1);
    } while (competitor_indices[3] == competitor_indices[0] ||
             competitor_indices[3] == competitor_indices[1] ||
             competitor_indices[3] == competitor_indices[2]);
}
