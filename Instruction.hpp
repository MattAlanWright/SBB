#ifndef _INSTRUCTION_HPP
#define _INSTRUCTION_HPP

// Effolkronium random library
#include "random.hpp"

using Random = effolkronium::random_static;

enum InstructionMode {
    INST_REGISTER_MODE,
    INST_INPUT_MODE,
    INST_NUM_MODES
};

enum InstructionOperation {
    INST_ADDITION,
    INST_SUBTRACTION,
    INST_MULTIPLICATION,
    INST_DIVISION,
    INST_COSINE,
    INST_LOG,
    INST_EXPONENTIAL,
    INST_CONDITIONAL,
    INST_NUM_OPERATIONS
};


/*
Instruction:
This struct is a bunch of inlined functionality surrounding a single 32-bit
integer. Each byte of this integer holds a single component of the instruction.
Bit-wise masks are used to ensure each byte's value is within the valid range
for that instruction.

TODO: This is not very generalizable. Changing the valid ranges/values of these
components requires manually re-calculating and changing masks, and manually
changing them back if we need to re-use the original values. Make the move to
templated struct. It will look gross but be very easy to change and experiment with.

In general, the operation for getting a specific component out of the packed 32-bit
integer is: shift the component byte to the LSB position and perform a bitwise AND
to retrieve valid bits.
*/
struct Instruction {

    int packed_instruction;

    static const std::initializer_list<int> valid_bits;

    static const unsigned int mode_shift   = 24;
    static const unsigned int target_shift = 16;
    static const unsigned int op_shift     = 8;

    static const unsigned int mode_mask    = 0b00000001;
    static const unsigned int target_mask  = 0b00000111;
    static const unsigned int op_mask      = 0b00000111;
    static const unsigned int source_mask  = 0b00000011;

    static const unsigned int valid_bits_mask = (mode_mask << mode_shift)     |
                                                (target_mask << target_shift) |
                                                (op_mask << op_shift)         |
                                                source_mask;

    inline Instruction() { packed_instruction = 0; }

    inline int getMode() {
        return (packed_instruction >> mode_shift) & mode_mask;
    }

    inline int getTarget() {
        return (packed_instruction >> target_shift) & target_mask;
    }

    inline int getOp() {
        return (packed_instruction >> op_shift) & op_mask;
    }

    inline int getSource() {
        return packed_instruction & source_mask;
    }

    inline void toggleRandomBit() {
        // Pick random bit from list of togglable bits
        int bit = Random::get<int>(valid_bits);

        // Toggle bit
        packed_instruction ^= (1UL << bit);
    }

    inline void randomize() {
        packed_instruction = Random::get<int>() & valid_bits_mask;
    }
};

// Valid Instruction bits, used for random selection when toggling a
// random bit. Values are repeated to to give each component an equal
// chance of being modified.
// - Bit 24        = Mode
// - Bits 16 to 18 = Target
// - Bits 8 to 10  = Operation
// - Bits 0 to 2   = Source
const std::initializer_list<int> Instruction::valid_bits = {
     0,  1,  0,  1,  0,  1,
     8,  9, 10,  8,  9, 10,
    16, 17, 18, 16, 17, 18,
    24, 24, 24, 24, 24, 24
};


#endif //_INSTRUCTION_HPP
