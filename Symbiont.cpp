#include <algorithm>
#include <array>
#include <functional>
#include <numeric>
#include <math.h>

#include "Symbiont.hpp"

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
      is_referenced(false),
      max_source_range(std::max(NUM_REGISTERS, NUM_INPUTS)),
      registers(NUM_REGISTERS)
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
    if(Random::get<float>(0.0f, 1.0f) > MUTATION_RATE) {
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


float Symbiont::bid(const std::vector<float>& input) {
    // Zero out registers
    for(int i = 0; i < NUM_REGISTERS; i++) registers[i] = 0.0;

    for(Instruction instruction : instructions) {
        executeInstruction(instruction, input);
    }

    return sigmoid(registers[0]);
}


void Symbiont::executeInstruction(Instruction& instruction, const std::vector<float>& input) {

    int mode         = instruction.getMode();
    int target_index = instruction.getTarget();
    int op_code      = instruction.getOp();
    int source_index = instruction.getSource();

    source_index %= source_mod_value[mode];

    const std::vector<float> &source = (mode == INST_REGISTER_MODE) ? registers : input;

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
            val = registers[target_index] / 2.0;
            break;
    }

    // Safeguard against overflows, NaNa, Infs, etc.
    registers[target_index] = std::clamp(val, MIN_REGISTER_VAL, MAX_REGISTER_VAL);
}
