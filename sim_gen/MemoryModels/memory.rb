module SimGen
  module Memory
    module Helper
      module_function  
    
      def get_addr_type(instructions)
        instructions.each { |insn| 
          insn[:code][:tree].each { |node|
            return node[:oprnds][0][:type] if node[:name] == :writeMem || node[:name] == :readMem 
          }
        }
      end
    end
  end
end


module SimGen
  module Memory
    module Header
      module_function

      def generate_memory(input_ir)
        type = Helper.get_addr_type input_ir[:instructions]
        type_str = Utility::HelperCpp::gen_type type

"#ifndef GENERATED_#{input_ir[:isa_name].upcase}_MEMORY_HH_INCLUDED
#define GENERATED_#{input_ir[:isa_name].upcase}_MEMORY_HH_INCLUDED

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

  virtual void writeBlock(std::span<const std::byte> src, #{type_str} addr) = 0;
  virtual void readBlock(#{type_str} addr, std::span<std::byte> dest) const = 0;

  void fillBlock(#{type_str} addr, std::byte value, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
      writeBlock({&value, 1}, addr + i);
    }
  }

  template <std::unsigned_integral T>
  [[nodiscard]] T read(#{type_str} addr) const {
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

  template <std::unsigned_integral T> void write(#{type_str} addr, T val) {
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

  virtual uint8_t read8(#{type_str} addr) const {
    return read<uint8_t>(addr);
  }
  virtual uint16_t read16(#{type_str} addr) const {
    return read<uint16_t>(addr);
  }
  virtual uint32_t read32(#{type_str} addr) const {
    return read<uint32_t>(addr);
  }
  virtual void write8(#{type_str} addr, uint8_t val) {
    write<uint8_t>(addr, val);
  }
  virtual void write16(#{type_str} addr, uint16_t val) {
    write<uint16_t>(addr, val);
  }
  virtual void write32(#{type_str} addr, uint32_t val) {
    write<uint32_t>(addr, val);
  }
};

std::unique_ptr<Memory> makePlain(std::size_t size, #{type_str} start = 0);

} // prot::memory

#endif // GENERATED_#{input_ir[:isa_name].upcase}_MEMORY_HH_INCLUDED
"
      end
    end
  end
end

module SimGen
  module Memory
    module TranslationUnit
      module_function

      def generate_memory(input_ir)
        type = Helper.get_addr_type input_ir[:instructions]
        type_str = Utility::HelperCpp::gen_type type

"#include \"isa.hh\"
#include \"memory.hh\"

#include <algorithm>
#include <cassert>
#include <vector>

extern \"C\" {
#include <sys/mman.h>
}

namespace prot::memory {

namespace {

class PlainMemory : public Memory {
  struct Unmap {
    std::size_t m_size = 0;

  public:
    explicit Unmap(std::size_t size) noexcept : m_size(size) {}

    void operator()(void *ptr) const noexcept { ::munmap(ptr, m_size); }
  };

public:
  explicit PlainMemory(std::size_t size, #{type_str} start)
      : m_storage(
            [size] {
              auto *ptr =
                  ::mmap(NULL, size, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
              if (ptr == MAP_FAILED) {
                throw std::runtime_error{\"\"};
              }

              return static_cast<std::byte *>(ptr);
            }(),
            Unmap{size}),
        m_data(m_storage.get(), size), m_start(start) {
    if (m_data.size() + m_start < m_start) {
      throw std::invalid_argument{\"\"};
    }
  }

  uint8_t read8(#{type_str} addr) const override {
    return *reinterpret_cast<const uint8_t *>(translateAddr(addr));
  }
  uint16_t read16(#{type_str} addr) const override {
    return *reinterpret_cast<const uint16_t *>(translateAddr(addr));
  }
  uint32_t read32(#{type_str} addr) const override {
    return *reinterpret_cast<const uint32_t *>(translateAddr(addr));
  }

  void write8(#{type_str} addr, uint8_t val) override {
    *reinterpret_cast<uint8_t *>(translateAddr(addr)) = val;
  }
  void write16(#{type_str} addr, uint16_t val) override {
    *reinterpret_cast<uint16_t *>(translateAddr(addr)) = val;
  }
  void write32(#{type_str} addr, uint32_t val) override {
    *reinterpret_cast<uint32_t *>(translateAddr(addr)) = val;
  }

  void writeBlock(std::span<const std::byte> src, #{type_str} addr) override {
    // checkRange(addr, src.size());
    std::memcpy(translateAddr(addr), src.data(), src.size());
  }

  void readBlock(#{type_str} addr, std::span<std::byte> dest) const override {
    // checkRange(addr, dest.size());
    std::memcpy(dest.data(), translateAddr(addr), dest.size());
  }

private:
  [[nodiscard]] std::size_t addrToOffset(#{type_str} addr) const {
    assert(addr >= m_start && addr <= m_start + m_data.size());
    return addr - m_start;
  }

  [[nodiscard]] std::byte *translateAddr(#{type_str} addr) {
    return m_data.data() + addrToOffset(addr);
  }

  [[nodiscard]] const std::byte *translateAddr(#{type_str} addr) const {
    return m_data.data() + addrToOffset(addr);
  }

  void checkRange(#{type_str} addr, std::size_t size) const {
    assert(addr + size >= addr);
    if (addr < m_start) {
      throw std::invalid_argument{\"\"};
    }

    if (addr + size > m_data.size() + m_start) {
      throw std::invalid_argument{\"\"};
    }
  }

  std::unique_ptr<std::byte, Unmap> m_storage;
  std::span<std::byte> m_data;
  #{type_str} m_start{};
};

} // namespace

std::unique_ptr<Memory> makePlain(std::size_t size, #{type_str} start /* = 0 */) {
  return std::make_unique<PlainMemory>(size, start);
}

} // prot::memory

"
      end
    end
  end
end

