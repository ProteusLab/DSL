#ifndef GENERATED_RISCV32I_HART_HH_INCLUDED
#define GENERATED_RISCV32I_HART_HH_INCLUDED

#include <memory>

#include "cpu_state.hh"
#include "elf_loader.hh"
#include "base_exec_engine.hh"
#include "memory.hh"

namespace prot::hart {
using namespace prot::state;
using namespace prot::isa;
using namespace prot::elf_loader;
using namespace prot::engine;
using namespace prot::memory;

class Hart {
public:
  Hart(std::unique_ptr<Memory> mem, std::unique_ptr<ExecEngine> engine);

  void setSP(uint32_t addr);

  void load(const ElfLoader &loader);

  // void dump(std::ostream &ost) const;

  void setPC(uint32_t addr);

  void run() {
    while (!m_cpu->m_finished) {
      m_engine->step(*m_cpu);
    }
  }

  auto getIcount() const { return m_cpu->m_icount; }

public:
  std::unique_ptr<Memory> m_mem;
  std::unique_ptr<CPU> m_cpu;
  std::unique_ptr<ExecEngine> m_engine;
};
} // namespace prot::hart

#endif // GENERATED_RISCV32I_HART_HH_INCLUDED
