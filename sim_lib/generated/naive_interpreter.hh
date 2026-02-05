#ifndef GENERATED_RISCV32I_INTERPRETER_HH_INCLUDED
#define GENERATED_RISCV32I_INTERPRETER_HH_INCLUDED

#include "base_exec_engine.hh"

namespace prot::engine {
using namespace prot::state;
using namespace prot::isa;

class Interpreter : public ExecEngine {
public:
void execute(CPU &cpu, const Instruction &insn) override;
};
} // namespace generated::riscv32i::engine

#endif // GENERATED_riscv32i_INTERPRETER_HH_INCLUDED
