#include "Instruction.hpp"

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
