#ifndef GENERATED_RISCV32I_MEMORY_HH_INCLUDED
#define GENERATED_RISCV32I_MEMORY_HH_INCLUDED

#include <array>
#include <memory>
#include <span>
#include <cstring>


namespace prot::memory {

struct Memory {

  Memory() = default;
  Memory(const Memory &) = delete;
  Memory &operator=(const Memory &) = delete;
  Memory(Memory &&) = delete;
  Memory &operator=(Memory &&) = delete;
  virtual ~Memory() = default;

  virtual void writeBlock(std::span<const std::byte> src, uint32_t addr) = 0;
  virtual void readBlock(uint32_t addr, std::span<std::byte> dest) const = 0;

  void fillBlock(uint32_t addr, std::byte value, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
      writeBlock({&value, 1}, addr + i);
    }
  }

  template <std::unsigned_integral T>
  [[nodiscard]] T read(uint32_t addr) const {
    if constexpr (std::same_as<T, uint8_t>) {
      return read8(addr);
    }
    if constexpr (std::same_as<T, uint16_t>) {
      return read16(addr);
    }
    if constexpr (std::same_as<T, uint32_t>) {
      return read32(addr);
    }

    std::array<std::byte, sizeof(T)> buf;
    readBlock(addr, buf);
    return std::bit_cast<T>(buf);
  }

  template <std::unsigned_integral T> void write(uint32_t addr, T val) {
    if constexpr (std::same_as<T, uint8_t>) {
      return write8(addr, val);
    }
    if constexpr (std::same_as<T, uint16_t>) {
      return write16(addr, val);
    }
    if constexpr (std::same_as<T, uint32_t>) {
      return write32(addr, val);
    }
    const auto &buf = std::bit_cast<std::array<std::byte, sizeof(T)>>(val);
    writeBlock(buf, addr);
  }

  virtual uint8_t read8(uint32_t addr) const {
    return read<uint8_t>(addr);
  }
  virtual uint16_t read16(uint32_t addr) const {
    return read<uint16_t>(addr);
  }
  virtual uint32_t read32(uint32_t addr) const {
    return read<uint32_t>(addr);
  }
  virtual void write8(uint32_t addr, uint8_t val) {
    write<uint8_t>(addr, val);
  }
  virtual void write16(uint32_t addr, uint16_t val) {
    write<uint16_t>(addr, val);
  }
  virtual void write32(uint32_t addr, uint32_t val) {
    write<uint32_t>(addr, val);
  }
};

std::unique_ptr<Memory> makePlain(std::size_t size, uint32_t start = 0);

} // prot::memory

#endif // GENERATED_RISCV32I_MEMORY_HH_INCLUDED
