#include <chrono>
#include <cstddef>
#include <filesystem>
#include <iostream>

#include "elf_loader.hh"
#include "hart.hh"
#include "naive_interpreter.hh"
#include "base_jit.hh"
#include "memory.hh"

int main(int argc, const char *argv[]) {

  std::filesystem::path elfPath{argv[1]};
  constexpr prot::isa::Addr kDefaultStack = 0x7fffffff;

  auto hart = [&] {
    prot::elf_loader::ElfLoader loader{elfPath};

    std::unique_ptr<prot::engine::ExecEngine> engine = std::make_unique<prot::engine::CachedInterpreter>();

    prot::hart::Hart hart{prot::memory::makePlain(4ULL << 30U), std::move(engine)};
    hart.load(loader);

    return hart;
  }();

  hart.m_cpu->setXRegs<uint32_t>(2, kDefaultStack);

  auto start = std::chrono::high_resolution_clock::now();
  hart.run();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "MIPS: " << hart.getIcount() / (duration.count() * 1000000) << std::endl;

  return EXIT_SUCCESS;
}
