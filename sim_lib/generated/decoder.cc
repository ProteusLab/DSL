#include "decoder.hh"
#include <optional>

namespace {
consteval std::size_t toBits(std::size_t bytes) {
  return bytes * std::numeric_limits<unsigned char>::digits;
}

template <typename T> consteval std::size_t sizeofBits() {
  return toBits(sizeof(T));
}

template <typename T>
constexpr auto toUnderlying(T val)
  requires std::is_enum_v<T>
{
  return static_cast<std::underlying_type_t<T>>(val);
}

template <std::unsigned_integral T> consteval T ones(std::size_t Num) {
  if (Num > sizeofBits<T>()) {
    // OK, we're in constexpr context
    throw "Num exceeds amount of bits";
  }
  if (Num == sizeofBits<T>()) {
    return ~T{};
  }
  return (T{1} << Num) - std::uint32_t{1};
}

template <std::unsigned_integral T>
consteval T getMask(std::size_t Msb, std::size_t Lsb) {
  if (Msb < Lsb) {
    throw "Illegal bits range";
  }
  return ones<T>(Msb - Lsb + 1) << Lsb;
}

template <std::size_t Msb, std::size_t Lsb, std::unsigned_integral T>
constexpr T slice(T word) {
  static_assert(Msb >= Lsb, "Error : illegal bits range");
  static_assert(Msb <= sizeofBits<T>());
  return (word & getMask<T>(Msb, Lsb)) >> Lsb;
}
} // namespace

namespace prot::decoder {
using namespace isa;
std::optional<Instruction> decode(const uint32_t raw_insn) {
  Instruction insn{};
  switch ((raw_insn >> 2) & 0b11111) {
    case 0b0: {
    switch ((raw_insn >> 12) & 0b111) {
      case 0b0: {
        // Decoded instruction: lb
        insn.m_opc = Opcode::kLB;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp288;
        _tmp288 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp288;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b1: {
        // Decoded instruction: lh
        insn.m_opc = Opcode::kLH;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp293;
        _tmp293 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp293;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b10: {
        // Decoded instruction: lw
        insn.m_opc = Opcode::kLW;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp298;
        _tmp298 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp298;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b100: {
        // Decoded instruction: lbu
        insn.m_opc = Opcode::kLBU;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp302;
        _tmp302 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp302;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b101: {
        // Decoded instruction: lhu
        insn.m_opc = Opcode::kLHU;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp307;
        _tmp307 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp307;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
    }
    }
    case 0b11: {
      // Decoded instruction: fence
      insn.m_opc = Opcode::kFENCE;
      uint32_t c1 = slice<31, 28>(raw_insn);
      uint32_t c2 = slice<27, 24>(raw_insn);
      uint32_t c3 = slice<23, 20>(raw_insn);
      uint32_t c4 = slice<19, 0>(raw_insn);
      return insn;
    }
    case 0b100: {
    switch ((raw_insn >> 12) & 0b111) {
      case 0b0: {
        // Decoded instruction: addi
        insn.m_opc = Opcode::kADDI;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp53;
        _tmp53 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp53;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b1: {
        // Decoded instruction: slli
        insn.m_opc = Opcode::kSLLI;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t func3 = slice<14, 12>(raw_insn);
        uint32_t f_imm4_0 = slice<24, 20>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_temp = slice<26, 25>(raw_insn);
        uint32_t f_sopcode = slice<31, 27>(raw_insn);
        int32_t imm;
        insn.operand0 = f_imm4_0;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b10: {
        // Decoded instruction: slti
        insn.m_opc = Opcode::kSLTI;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp56;
        _tmp56 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp56;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b11: {
        // Decoded instruction: sltiu
        insn.m_opc = Opcode::kSLTIU;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp61;
        _tmp61 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp61;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b100: {
        // Decoded instruction: xori
        insn.m_opc = Opcode::kXORI;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp67;
        _tmp67 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp67;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b101: {
      switch ((raw_insn >> 30) & 0b1) {
        case 0b0: {
          // Decoded instruction: srli
          insn.m_opc = Opcode::kSRLI;
          uint32_t f_opcode = slice<6, 0>(raw_insn);
          uint32_t func3 = slice<14, 12>(raw_insn);
          uint32_t f_imm4_0 = slice<24, 20>(raw_insn);
          uint32_t f_rd = slice<11, 7>(raw_insn);
          uint32_t f_rs1 = slice<19, 15>(raw_insn);
          uint32_t f_temp = slice<26, 25>(raw_insn);
          uint32_t f_sopcode = slice<31, 27>(raw_insn);
          int32_t imm;
          insn.operand0 = f_imm4_0;
          uint32_t rs1;
          insn.operand1 = f_rs1;
          uint32_t rd;
          insn.operand2 = f_rd;
          return insn;
        }
        case 0b1: {
          // Decoded instruction: srai
          insn.m_opc = Opcode::kSRAI;
          uint32_t f_opcode = slice<6, 0>(raw_insn);
          uint32_t func3 = slice<14, 12>(raw_insn);
          uint32_t f_imm4_0 = slice<24, 20>(raw_insn);
          uint32_t f_rd = slice<11, 7>(raw_insn);
          uint32_t f_rs1 = slice<19, 15>(raw_insn);
          uint32_t f_temp = slice<26, 25>(raw_insn);
          uint32_t f_sopcode = slice<31, 27>(raw_insn);
          int32_t imm;
          insn.operand0 = f_imm4_0;
          uint32_t rs1;
          insn.operand1 = f_rs1;
          uint32_t rd;
          insn.operand2 = f_rd;
          return insn;
        }
      }
      }
      case 0b110: {
        // Decoded instruction: ori
        insn.m_opc = Opcode::kORI;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp70;
        _tmp70 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp70;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b111: {
        // Decoded instruction: andi
        insn.m_opc = Opcode::kANDI;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_imm = slice<31, 20>(raw_insn);
        int32_t _tmp73;
        _tmp73 = (static_cast<int32_t>(f_imm) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp73;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
    }
    }
    case 0b101: {
      // Decoded instruction: auipc
      insn.m_opc = Opcode::kAUIPC;
      uint32_t f_opcode = slice<6, 0>(raw_insn);
      uint32_t f_rd = slice<11, 7>(raw_insn);
      uint32_t f_imm = slice<31, 12>(raw_insn);
      uint32_t rd;
      insn.operand0 = f_rd;
      int32_t _tmp3;
      _tmp3 = static_cast<int32_t>(f_imm);
      int32_t _tmp5;
      _tmp5 = _tmp3 << 12;
      int32_t imm;
      insn.operand1 = _tmp5;
      return insn;
    }
    case 0b1000: {
    switch ((raw_insn >> 12) & 0b11) {
      case 0b0: {
        // Decoded instruction: sb
        insn.m_opc = Opcode::kSB;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t func3 = slice<14, 12>(raw_insn);
        uint32_t f_imm4_0 = slice<11, 7>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_imm11_5 = slice<31, 25>(raw_insn);
        uint32_t _tmp255;
        _tmp255 = static_cast<uint32_t>(f_imm11_5);
        uint32_t _tmp257;
        _tmp257 = _tmp255 << 5;
        uint32_t _tmp258;
        _tmp258 = static_cast<uint32_t>(f_imm4_0);
        uint32_t _tmp259;
        _tmp259 = _tmp257 | _tmp258;
        int32_t _tmp260;
        _tmp260 = (static_cast<int32_t>(_tmp259) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp260;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rs2;
        insn.operand2 = f_rs2;
        return insn;
      }
      case 0b1: {
        // Decoded instruction: sh
        insn.m_opc = Opcode::kSH;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t func3 = slice<14, 12>(raw_insn);
        uint32_t f_imm4_0 = slice<11, 7>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_imm11_5 = slice<31, 25>(raw_insn);
        uint32_t _tmp267;
        _tmp267 = static_cast<uint32_t>(f_imm11_5);
        uint32_t _tmp269;
        _tmp269 = _tmp267 << 5;
        uint32_t _tmp270;
        _tmp270 = static_cast<uint32_t>(f_imm4_0);
        uint32_t _tmp271;
        _tmp271 = _tmp269 | _tmp270;
        int32_t _tmp272;
        _tmp272 = (static_cast<int32_t>(_tmp271) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp272;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rs2;
        insn.operand2 = f_rs2;
        return insn;
      }
      case 0b10: {
        // Decoded instruction: sw
        insn.m_opc = Opcode::kSW;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t func3 = slice<14, 12>(raw_insn);
        uint32_t f_imm4_0 = slice<11, 7>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_imm11_5 = slice<31, 25>(raw_insn);
        uint32_t _tmp279;
        _tmp279 = static_cast<uint32_t>(f_imm11_5);
        uint32_t _tmp281;
        _tmp281 = _tmp279 << 5;
        uint32_t _tmp282;
        _tmp282 = static_cast<uint32_t>(f_imm4_0);
        uint32_t _tmp283;
        _tmp283 = _tmp281 | _tmp282;
        int32_t _tmp284;
        _tmp284 = (static_cast<int32_t>(_tmp283) << 20) >> 20;
        int32_t imm;
        insn.operand0 = _tmp284;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rs2;
        insn.operand2 = f_rs2;
        return insn;
      }
    }
    }
    case 0b1100: {
    switch ((raw_insn >> 12) & 0b111) {
      case 0b0: {
      switch ((raw_insn >> 30) & 0b1) {
        case 0b0: {
          // Decoded instruction: add
          insn.m_opc = Opcode::kADD;
          uint32_t f_opcode = slice<6, 0>(raw_insn);
          uint32_t f_rd = slice<11, 7>(raw_insn);
          uint32_t f_funct3 = slice<14, 12>(raw_insn);
          uint32_t f_rs1 = slice<19, 15>(raw_insn);
          uint32_t f_rs2 = slice<24, 20>(raw_insn);
          uint32_t f_funct7 = slice<31, 25>(raw_insn);
          uint32_t rs2;
          insn.operand0 = f_rs2;
          uint32_t rs1;
          insn.operand1 = f_rs1;
          uint32_t rd;
          insn.operand2 = f_rd;
          return insn;
        }
        case 0b1: {
          // Decoded instruction: sub
          insn.m_opc = Opcode::kSUB;
          uint32_t f_opcode = slice<6, 0>(raw_insn);
          uint32_t f_rd = slice<11, 7>(raw_insn);
          uint32_t f_funct3 = slice<14, 12>(raw_insn);
          uint32_t f_rs1 = slice<19, 15>(raw_insn);
          uint32_t f_rs2 = slice<24, 20>(raw_insn);
          uint32_t f_funct7 = slice<31, 25>(raw_insn);
          uint32_t rs2;
          insn.operand0 = f_rs2;
          uint32_t rs1;
          insn.operand1 = f_rs1;
          uint32_t rd;
          insn.operand2 = f_rd;
          return insn;
        }
      }
      }
      case 0b1: {
        // Decoded instruction: sll
        insn.m_opc = Opcode::kSLL;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_funct7 = slice<31, 25>(raw_insn);
        uint32_t rs2;
        insn.operand0 = f_rs2;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b10: {
        // Decoded instruction: slt
        insn.m_opc = Opcode::kSLT;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_funct7 = slice<31, 25>(raw_insn);
        uint32_t rs2;
        insn.operand0 = f_rs2;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b11: {
        // Decoded instruction: sltu
        insn.m_opc = Opcode::kSLTU;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_funct7 = slice<31, 25>(raw_insn);
        uint32_t rs2;
        insn.operand0 = f_rs2;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b100: {
        // Decoded instruction: xor
        insn.m_opc = Opcode::kXOR;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_funct7 = slice<31, 25>(raw_insn);
        uint32_t rs2;
        insn.operand0 = f_rs2;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b101: {
      switch ((raw_insn >> 30) & 0b1) {
        case 0b0: {
          // Decoded instruction: srl
          insn.m_opc = Opcode::kSRL;
          uint32_t f_opcode = slice<6, 0>(raw_insn);
          uint32_t f_rd = slice<11, 7>(raw_insn);
          uint32_t f_funct3 = slice<14, 12>(raw_insn);
          uint32_t f_rs1 = slice<19, 15>(raw_insn);
          uint32_t f_rs2 = slice<24, 20>(raw_insn);
          uint32_t f_funct7 = slice<31, 25>(raw_insn);
          uint32_t rs2;
          insn.operand0 = f_rs2;
          uint32_t rs1;
          insn.operand1 = f_rs1;
          uint32_t rd;
          insn.operand2 = f_rd;
          return insn;
        }
        case 0b1: {
          // Decoded instruction: sra
          insn.m_opc = Opcode::kSRA;
          uint32_t f_opcode = slice<6, 0>(raw_insn);
          uint32_t f_rd = slice<11, 7>(raw_insn);
          uint32_t f_funct3 = slice<14, 12>(raw_insn);
          uint32_t f_rs1 = slice<19, 15>(raw_insn);
          uint32_t f_rs2 = slice<24, 20>(raw_insn);
          uint32_t f_funct7 = slice<31, 25>(raw_insn);
          uint32_t rs2;
          insn.operand0 = f_rs2;
          uint32_t rs1;
          insn.operand1 = f_rs1;
          uint32_t rd;
          insn.operand2 = f_rd;
          return insn;
        }
      }
      }
      case 0b110: {
        // Decoded instruction: or
        insn.m_opc = Opcode::kOR;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_funct7 = slice<31, 25>(raw_insn);
        uint32_t rs2;
        insn.operand0 = f_rs2;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
      case 0b111: {
        // Decoded instruction: and
        insn.m_opc = Opcode::kAND;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_rd = slice<11, 7>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_funct7 = slice<31, 25>(raw_insn);
        uint32_t rs2;
        insn.operand0 = f_rs2;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rd;
        insn.operand2 = f_rd;
        return insn;
      }
    }
    }
    case 0b1101: {
      // Decoded instruction: lui
      insn.m_opc = Opcode::kLUI;
      uint32_t f_opcode = slice<6, 0>(raw_insn);
      uint32_t f_rd = slice<11, 7>(raw_insn);
      uint32_t f_imm = slice<31, 12>(raw_insn);
      uint32_t rd;
      insn.operand0 = f_rd;
      int32_t _tmp0;
      _tmp0 = static_cast<int32_t>(f_imm);
      int32_t _tmp2;
      _tmp2 = _tmp0 << 12;
      int32_t imm;
      insn.operand1 = _tmp2;
      return insn;
    }
    case 0b11000: {
    switch ((raw_insn >> 12) & 0b111) {
      case 0b0: {
        // Decoded instruction: beq
        insn.m_opc = Opcode::kBEQ;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_imm4_1 = slice<11, 8>(raw_insn);
        uint32_t f_imm10_5 = slice<30, 25>(raw_insn);
        uint32_t f_imm11 = slice<7, 7>(raw_insn);
        uint32_t f_imm12 = slice<31, 31>(raw_insn);
        uint32_t _tmp83;
        _tmp83 = static_cast<uint32_t>(f_imm12);
        uint32_t _tmp85;
        _tmp85 = _tmp83 << 12;
        uint32_t _tmp86;
        _tmp86 = static_cast<uint32_t>(f_imm11);
        uint32_t _tmp88;
        _tmp88 = _tmp86 << 11;
        uint32_t _tmp89;
        _tmp89 = _tmp85 | _tmp88;
        uint32_t _tmp90;
        _tmp90 = static_cast<uint32_t>(f_imm10_5);
        uint32_t _tmp92;
        _tmp92 = _tmp90 << 5;
        uint32_t _tmp93;
        _tmp93 = _tmp89 | _tmp92;
        uint32_t _tmp94;
        _tmp94 = static_cast<uint32_t>(f_imm4_1);
        uint32_t _tmp96;
        _tmp96 = _tmp94 << 1;
        uint32_t _tmp97;
        _tmp97 = _tmp93 | _tmp96;
        int32_t _tmp98;
        _tmp98 = (static_cast<int32_t>(_tmp97) << 19) >> 19;
        int32_t imm;
        insn.operand0 = _tmp98;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rs2;
        insn.operand2 = f_rs2;
        return insn;
      }
      case 0b1: {
        // Decoded instruction: bne
        insn.m_opc = Opcode::kBNE;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_imm4_1 = slice<11, 8>(raw_insn);
        uint32_t f_imm10_5 = slice<30, 25>(raw_insn);
        uint32_t f_imm11 = slice<7, 7>(raw_insn);
        uint32_t f_imm12 = slice<31, 31>(raw_insn);
        uint32_t _tmp106;
        _tmp106 = static_cast<uint32_t>(f_imm12);
        uint32_t _tmp108;
        _tmp108 = _tmp106 << 12;
        uint32_t _tmp109;
        _tmp109 = static_cast<uint32_t>(f_imm11);
        uint32_t _tmp111;
        _tmp111 = _tmp109 << 11;
        uint32_t _tmp112;
        _tmp112 = _tmp108 | _tmp111;
        uint32_t _tmp113;
        _tmp113 = static_cast<uint32_t>(f_imm10_5);
        uint32_t _tmp115;
        _tmp115 = _tmp113 << 5;
        uint32_t _tmp116;
        _tmp116 = _tmp112 | _tmp115;
        uint32_t _tmp117;
        _tmp117 = static_cast<uint32_t>(f_imm4_1);
        uint32_t _tmp119;
        _tmp119 = _tmp117 << 1;
        uint32_t _tmp120;
        _tmp120 = _tmp116 | _tmp119;
        int32_t _tmp121;
        _tmp121 = (static_cast<int32_t>(_tmp120) << 19) >> 19;
        int32_t imm;
        insn.operand0 = _tmp121;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rs2;
        insn.operand2 = f_rs2;
        return insn;
      }
      case 0b100: {
        // Decoded instruction: blt
        insn.m_opc = Opcode::kBLT;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_imm4_1 = slice<11, 8>(raw_insn);
        uint32_t f_imm10_5 = slice<30, 25>(raw_insn);
        uint32_t f_imm11 = slice<7, 7>(raw_insn);
        uint32_t f_imm12 = slice<31, 31>(raw_insn);
        uint32_t _tmp129;
        _tmp129 = static_cast<uint32_t>(f_imm12);
        uint32_t _tmp131;
        _tmp131 = _tmp129 << 12;
        uint32_t _tmp132;
        _tmp132 = static_cast<uint32_t>(f_imm11);
        uint32_t _tmp134;
        _tmp134 = _tmp132 << 11;
        uint32_t _tmp135;
        _tmp135 = _tmp131 | _tmp134;
        uint32_t _tmp136;
        _tmp136 = static_cast<uint32_t>(f_imm10_5);
        uint32_t _tmp138;
        _tmp138 = _tmp136 << 5;
        uint32_t _tmp139;
        _tmp139 = _tmp135 | _tmp138;
        uint32_t _tmp140;
        _tmp140 = static_cast<uint32_t>(f_imm4_1);
        uint32_t _tmp142;
        _tmp142 = _tmp140 << 1;
        uint32_t _tmp143;
        _tmp143 = _tmp139 | _tmp142;
        int32_t _tmp144;
        _tmp144 = (static_cast<int32_t>(_tmp143) << 19) >> 19;
        int32_t imm;
        insn.operand0 = _tmp144;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rs2;
        insn.operand2 = f_rs2;
        return insn;
      }
      case 0b101: {
        // Decoded instruction: bge
        insn.m_opc = Opcode::kBGE;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_imm4_1 = slice<11, 8>(raw_insn);
        uint32_t f_imm10_5 = slice<30, 25>(raw_insn);
        uint32_t f_imm11 = slice<7, 7>(raw_insn);
        uint32_t f_imm12 = slice<31, 31>(raw_insn);
        uint32_t _tmp154;
        _tmp154 = static_cast<uint32_t>(f_imm12);
        uint32_t _tmp156;
        _tmp156 = _tmp154 << 12;
        uint32_t _tmp157;
        _tmp157 = static_cast<uint32_t>(f_imm11);
        uint32_t _tmp159;
        _tmp159 = _tmp157 << 11;
        uint32_t _tmp160;
        _tmp160 = _tmp156 | _tmp159;
        uint32_t _tmp161;
        _tmp161 = static_cast<uint32_t>(f_imm10_5);
        uint32_t _tmp163;
        _tmp163 = _tmp161 << 5;
        uint32_t _tmp164;
        _tmp164 = _tmp160 | _tmp163;
        uint32_t _tmp165;
        _tmp165 = static_cast<uint32_t>(f_imm4_1);
        uint32_t _tmp167;
        _tmp167 = _tmp165 << 1;
        uint32_t _tmp168;
        _tmp168 = _tmp164 | _tmp167;
        int32_t _tmp169;
        _tmp169 = (static_cast<int32_t>(_tmp168) << 19) >> 19;
        int32_t imm;
        insn.operand0 = _tmp169;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rs2;
        insn.operand2 = f_rs2;
        return insn;
      }
      case 0b110: {
        // Decoded instruction: bltu
        insn.m_opc = Opcode::kBLTU;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_imm4_1 = slice<11, 8>(raw_insn);
        uint32_t f_imm10_5 = slice<30, 25>(raw_insn);
        uint32_t f_imm11 = slice<7, 7>(raw_insn);
        uint32_t f_imm12 = slice<31, 31>(raw_insn);
        uint32_t _tmp179;
        _tmp179 = static_cast<uint32_t>(f_imm12);
        uint32_t _tmp181;
        _tmp181 = _tmp179 << 12;
        uint32_t _tmp182;
        _tmp182 = static_cast<uint32_t>(f_imm11);
        uint32_t _tmp184;
        _tmp184 = _tmp182 << 11;
        uint32_t _tmp185;
        _tmp185 = _tmp181 | _tmp184;
        uint32_t _tmp186;
        _tmp186 = static_cast<uint32_t>(f_imm10_5);
        uint32_t _tmp188;
        _tmp188 = _tmp186 << 5;
        uint32_t _tmp189;
        _tmp189 = _tmp185 | _tmp188;
        uint32_t _tmp190;
        _tmp190 = static_cast<uint32_t>(f_imm4_1);
        uint32_t _tmp192;
        _tmp192 = _tmp190 << 1;
        uint32_t _tmp193;
        _tmp193 = _tmp189 | _tmp192;
        int32_t _tmp194;
        _tmp194 = (static_cast<int32_t>(_tmp193) << 19) >> 19;
        int32_t imm;
        insn.operand0 = _tmp194;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rs2;
        insn.operand2 = f_rs2;
        return insn;
      }
      case 0b111: {
        // Decoded instruction: bgeu
        insn.m_opc = Opcode::kBGEU;
        uint32_t f_opcode = slice<6, 0>(raw_insn);
        uint32_t f_funct3 = slice<14, 12>(raw_insn);
        uint32_t f_rs1 = slice<19, 15>(raw_insn);
        uint32_t f_rs2 = slice<24, 20>(raw_insn);
        uint32_t f_imm4_1 = slice<11, 8>(raw_insn);
        uint32_t f_imm10_5 = slice<30, 25>(raw_insn);
        uint32_t f_imm11 = slice<7, 7>(raw_insn);
        uint32_t f_imm12 = slice<31, 31>(raw_insn);
        uint32_t _tmp204;
        _tmp204 = static_cast<uint32_t>(f_imm12);
        uint32_t _tmp206;
        _tmp206 = _tmp204 << 12;
        uint32_t _tmp207;
        _tmp207 = static_cast<uint32_t>(f_imm11);
        uint32_t _tmp209;
        _tmp209 = _tmp207 << 11;
        uint32_t _tmp210;
        _tmp210 = _tmp206 | _tmp209;
        uint32_t _tmp211;
        _tmp211 = static_cast<uint32_t>(f_imm10_5);
        uint32_t _tmp213;
        _tmp213 = _tmp211 << 5;
        uint32_t _tmp214;
        _tmp214 = _tmp210 | _tmp213;
        uint32_t _tmp215;
        _tmp215 = static_cast<uint32_t>(f_imm4_1);
        uint32_t _tmp217;
        _tmp217 = _tmp215 << 1;
        uint32_t _tmp218;
        _tmp218 = _tmp214 | _tmp217;
        int32_t _tmp219;
        _tmp219 = (static_cast<int32_t>(_tmp218) << 19) >> 19;
        int32_t imm;
        insn.operand0 = _tmp219;
        uint32_t rs1;
        insn.operand1 = f_rs1;
        uint32_t rs2;
        insn.operand2 = f_rs2;
        return insn;
      }
    }
    }
    case 0b11001: {
      // Decoded instruction: jalr
      insn.m_opc = Opcode::kJALR;
      uint32_t f_opcode = slice<6, 0>(raw_insn);
      uint32_t f_rd = slice<11, 7>(raw_insn);
      uint32_t f_funct3 = slice<14, 12>(raw_insn);
      uint32_t f_rs1 = slice<19, 15>(raw_insn);
      uint32_t f_imm = slice<31, 20>(raw_insn);
      int32_t _tmp248;
      _tmp248 = (static_cast<int32_t>(f_imm) << 20) >> 20;
      int32_t imm;
      insn.operand0 = _tmp248;
      uint32_t rs1;
      insn.operand1 = f_rs1;
      uint32_t rd;
      insn.operand2 = f_rd;
      return insn;
    }
    case 0b11011: {
      // Decoded instruction: jal
      insn.m_opc = Opcode::kJAL;
      uint32_t f_opcode = slice<6, 0>(raw_insn);
      uint32_t f_rd = slice<11, 7>(raw_insn);
      uint32_t f_imm20 = slice<31, 31>(raw_insn);
      uint32_t f_imm19_12 = slice<19, 12>(raw_insn);
      uint32_t f_imm11 = slice<20, 20>(raw_insn);
      uint32_t f_imm10_1 = slice<30, 21>(raw_insn);
      uint32_t _tmp229;
      _tmp229 = static_cast<uint32_t>(f_imm20);
      uint32_t _tmp231;
      _tmp231 = _tmp229 << 20;
      uint32_t _tmp232;
      _tmp232 = static_cast<uint32_t>(f_imm19_12);
      uint32_t _tmp234;
      _tmp234 = _tmp232 << 12;
      uint32_t _tmp235;
      _tmp235 = _tmp231 | _tmp234;
      uint32_t _tmp236;
      _tmp236 = static_cast<uint32_t>(f_imm11);
      uint32_t _tmp238;
      _tmp238 = _tmp236 << 11;
      uint32_t _tmp239;
      _tmp239 = _tmp235 | _tmp238;
      uint32_t _tmp240;
      _tmp240 = static_cast<uint32_t>(f_imm10_1);
      uint32_t _tmp242;
      _tmp242 = _tmp240 << 1;
      uint32_t _tmp243;
      _tmp243 = _tmp239 | _tmp242;
      int32_t _tmp244;
      _tmp244 = (static_cast<int32_t>(_tmp243) << 11) >> 11;
      int32_t imm;
      insn.operand0 = _tmp244;
      uint32_t rd;
      insn.operand1 = f_rd;
      return insn;
    }
    case 0b11100: {
    switch ((raw_insn >> 20) & 0b1) {
      case 0b0: {
        // Decoded instruction: ecall
        insn.m_opc = Opcode::kECALL;
        uint32_t c = slice<31, 0>(raw_insn);
        return insn;
      }
      case 0b1: {
        // Decoded instruction: ebreak
        insn.m_opc = Opcode::kEBREAK;
        uint32_t c = slice<31, 0>(raw_insn);
        return insn;
      }
    }
    }
  }
  return {}; // No matching instruction found
}
} // namespace generated::riscv32i::decoder
