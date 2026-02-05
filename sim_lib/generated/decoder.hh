#ifndef GENERATED_RISCV32I_DECODER_HH_INCLUDED
#define GENERATED_RISCV32I_DECODER_HH_INCLUDED

#include "isa.hh"
#include <optional>
#include <limits>
                
namespace prot::decoder {
using namespace prot::isa;
std::optional<Instruction> decode(const uint32_t raw_insn);
} // namespace generated::riscv32i::decoder

#endif // GENERATED_RISCV32I_DECODER_HH_INCLUDED