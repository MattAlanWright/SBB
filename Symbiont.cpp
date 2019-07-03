#include <algorithm>
#include <array>
#include <functional>
#include <numeric>
#include <cmath>

#include "Symbiont.hpp"

float sigmoid(float z) {
   return 1.0 / (1.0 + exp(-z));
}

float clamp(float value, float min, float max) {
    return std::max(std::min(value, max), min);
}

Symbiont::Symbiont(int action,
                   int num_registers,
                   int num_actions,
                   int num_inputs)
    : action(action),
      num_registers(num_registers),
      num_actions(num_actions),
      registers(num_registers),
      is_referenced(false),
      source_mod_value {num_registers, num_inputs}
{
    // If action not specified, select at random
    if( action == -1 ) action = Random::get<int>(0, num_actions - 1);

    // Reserve space in vectors for maximum number of possible instructions
    instructions.reserve(MAX_NUM_INSTRUCTIONS);

    // Initialize random instructions
    initializeInstructions();
}


void Symbiont::initializeInstructions() {
    int num_initial_instructions = num_registers * num_registers;
    Instruction instruction;
    for(int i = 0; i < num_initial_instructions; i++) {
        instruction.randomize();
        instructions.push_back(instruction);
    }
 }


void Symbiont::mutate() {
    deleteRandomInstruction(PROB_DEL_INST);
    addRandomInstruction(PROB_ADD_INST);
    mutateRandomInstruction(PROB_MUT_INST);
    swapRandomInstructions(PROB_SWP_INST);

}


void Symbiont::deleteRandomInstruction(float prob_delete) {
    if( Random::get<float>(0.0, 1.0) > prob_delete && instructions.size() > MIN_NUM_INSTRUCTIONS ) {
        return;
    }

    int random_instruction_index = Random::get<int>(0, instructions.size() - 1);
    instructions.erase(instructions.begin() + random_instruction_index);
}


void Symbiont::addRandomInstruction(float prob_add) {
    if( Random::get<float>(0.0, 1.0) > prob_add && instructions.size() < MAX_NUM_INSTRUCTIONS ) {
        return;
    }

    Instruction new_instruction;
    new_instruction.randomize();

    int random_instruction_index = Random::get<int>(0, instructions.size() - 1);
    instructions.insert(instructions.begin() + random_instruction_index, new_instruction);
}


void Symbiont::mutateRandomInstruction(float prob_mutate) {
    if( Random::get<float>(0.0, 1.0) > prob_mutate ) {
        return;
    }


    int random_instruction_index = Random::get<int>(0, instructions.size() - 1);
    instructions[random_instruction_index].toggleRandomBit();
}


void Symbiont::swapRandomInstructions(float prob_swap) {
    if( Random::get<float>(0.0, 1.0) > prob_swap ) {
        return;
    }

    int i1, i2;
    i1 = i2 = Random::get<int>(0, instructions.size() - 1);
    while(i1 == i2) {
        i2 = Random::get<int>(0, instructions.size() - 1);
    }

    Instruction temp = instructions[i1];
    instructions[i1] = instructions[i2];
    instructions[i2] = temp;
}


void Symbiont::mutateAction(int num_actions) {
    int new_action = action;
    while(new_action == action) {
        new_action = Random::get<int>(0, num_actions - 1);
    }
    action = new_action;
}


float Symbiont::bid(const std::vector<float> &input) {
    // Zero out registers
    for(int i = 0; i < registers.size(); i++) registers[i] = 0.0;

    for(const Instruction &instruction : instructions) {
        executeInstruction(instruction, input);
    }

    return sigmoid(registers[0]);
}


void Symbiont::executeInstruction(const Instruction& instruction,
                                  const std::vector<float>& input) {

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
        case INST_COSINE:
            val = std::cos(source[target_index]);
            break;
        case INST_LOG:
            val = std::log(source[target_index]);
            break;
        case INST_EXPONENTIAL:
            val = std::exp(source[target_index]);
            break;
        case INST_CONDITIONAL:
            if( registers[target_index] < source[source_index]) {
                registers[target_index] = -registers[target_index];
            }
            break;
    }

    // Safeguard against overflows, NaNs, Infs, etc.
    registers[target_index] = clamp(val, MIN_REGISTER_VAL, MAX_REGISTER_VAL);

}
