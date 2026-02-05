#ifndef GENERATED_RISCV32I_ISA_HH_INCLUDED
#define GENERATED_RISCV32I_ISA_HH_INCLUDED

#include <cstdint>

namespace prot::isa {
using Addr = uint32_t;

enum class Opcode : uint32_t {
  kLUI,
  kAUIPC,
  kADD,
  kSUB,
  kSLL,
  kSLT,
  kSLTU,
  kXOR,
  kSRL,
  kSRA,
  kOR,
  kAND,
  kADDI,
  kSLTI,
  kSLTIU,
  kXORI,
  kORI,
  kANDI,
  kSLLI,
  kSRLI,
  kSRAI,
  kBEQ,
  kBNE,
  kBLT,
  kBGE,
  kBLTU,
  kBGEU,
  kJAL,
  kJALR,
  kSB,
  kSH,
  kSW,
  kLB,
  kLH,
  kLW,
  kLBU,
  kLHU,
  kECALL,
  kEBREAK,
  kFENCE,
};

struct Instruction {
  Opcode m_opc;
  uint32_t operand0;
  uint32_t operand1;
  uint32_t operand2;
};

} // namespace prot::isa

#endif // GENERATED_RISCV32I_ISA_HH_INCLUDED