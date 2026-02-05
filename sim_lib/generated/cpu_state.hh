#ifndef GENERATED_RISCV32I_CPUSTATE_HH_INCLUDED
#define GENERATED_RISCV32I_CPUSTATE_HH_INCLUDED

#include "memory.hh"

#include <array>
#include <cstddef>
#include <cstdint>

namespace prot::memory {
class Memory;
} // prot::memory

namespace prot::state {
using namespace prot::memory;

class CPU final {
public:
  // Register file: XRegs
  std::array<uint32_t, 32> m_XRegs;
  
  // Program Counter
  uint32_t m_pc;
  
  // Instruction count
  std::size_t m_icount{0};

  // Pointer to memory
  Memory *m_memory{nullptr};

  // Finished flag
  bool m_finished{false};

  explicit CPU(Memory *mem) : m_memory(mem) {}

  // Set register function for XRegs
  template<std::integral T>
  void setXRegs(const std::size_t reg, const T value) {
    if (reg == 0) return; // x0 is zero register
    m_XRegs[reg] = value;
  }
  
  // Read register function for XRegs
  template<std::integral T>
  T getXRegs(const std::size_t reg) const {
    return static_cast<T>(m_XRegs[reg]);
  }
  
  // Set PC function
  void setPC(const uint32_t value) {
    m_pc = value;
  }
  
  // Read PC function
  uint32_t getPC() const {
    return m_pc;
  }
  
  uint32_t getSysCallNum() const;
  uint32_t getSysCallArg(std::size_t num) const;
  uint32_t getSysCallRet() const;
  void emulateSysCall();

  // Function to stop the CPU execution
  void doExit() {
    m_finished = true;
  }

  // Function to increase instruction count
  void increaseICount() {
    ++m_icount;
  }
};

} // prot::state

#endif // GENERATED_RISCV32I_CPUSTATE_HH_INCLUDED
