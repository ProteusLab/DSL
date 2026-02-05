#ifndef GENERATED_RISCV32I_EXEC_ENGINE_HH_INCLUDED
#define GENERATED_RISCV32I_EXEC_ENGINE_HH_INCLUDED

#include "cpu_state.hh"
#include "isa.hh"
#include "memory.hh"

namespace prot::engine {
using namespace prot::state;
using namespace prot::isa;

struct ExecEngine {
  virtual ~ExecEngine() = default;

  virtual void execute(CPU &cpu, const Instruction &insn) = 0;
  virtual void step(CPU &cpu);
};
} // namespace prot::engine

#endif // GENERATED_RISCV32I_EXEC_ENGINE_HH_INCLUDED
