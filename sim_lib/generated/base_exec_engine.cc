#include "base_exec_engine.hh"
#include "memory.hh"
#include "decoder.hh"

namespace prot::engine {
using namespace prot::state;
using namespace prot::isa;

void ExecEngine::step(CPU &cpu) {
  const auto bytes = cpu.m_memory->read<uint32_t>(cpu.getPC());

  auto &&instr = decoder::decode(bytes);

  execute(cpu, *instr);
  cpu.increaseICount();
}
} // namespace prot::engine
