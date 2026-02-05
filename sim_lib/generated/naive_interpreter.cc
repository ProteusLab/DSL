#include "naive_interpreter.hh"

#include <cassert>

namespace prot::engine {
using namespace prot::state;
using namespace prot::isa;

namespace {
  bool isBranchInstruction(const Instruction &insn) {
    switch (insn.m_opc) {
      case Opcode::kBEQ:
      case Opcode::kBNE:
      case Opcode::kBLT:
      case Opcode::kBGE:
      case Opcode::kBLTU:
      case Opcode::kBGEU:
      case Opcode::kJAL:
      case Opcode::kJALR:
      return true;
    default:
      return false;
    }
  }

void doLUI(CPU &cpu, const Instruction &insn) {
  cpu.setXRegs(insn.operand0, insn.operand1);
}

void doAUIPC(CPU &cpu, const Instruction &insn) {
  int32_t _tmp6;
  _tmp6 = insn.operand1 + cpu.getPC();
  cpu.setXRegs(insn.operand0, _tmp6);
}

void doADD(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp7;
  uint32_t _tmp8;
  _tmp8 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp7 = static_cast<uint32_t>(_tmp8);
  uint32_t _tmp9;
  uint32_t _tmp10;
  _tmp10 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp9 = static_cast<uint32_t>(_tmp10);
  uint32_t _tmp11;
  _tmp11 = _tmp7 + _tmp9;
  cpu.setXRegs(insn.operand2, _tmp11);
}

void doSUB(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp12;
  uint32_t _tmp13;
  _tmp13 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp12 = static_cast<uint32_t>(_tmp13);
  uint32_t _tmp14;
  uint32_t _tmp15;
  _tmp15 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp14 = static_cast<uint32_t>(_tmp15);
  uint32_t _tmp16;
  _tmp16 = _tmp12 - _tmp14;
  cpu.setXRegs(insn.operand2, _tmp16);
}

void doSLL(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp17;
  uint32_t _tmp18;
  _tmp18 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp17 = static_cast<uint32_t>(_tmp18);
  uint32_t _tmp19;
  uint32_t _tmp20;
  _tmp20 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp19 = static_cast<uint32_t>(_tmp20);
  uint32_t _tmp21;
  _tmp21 = _tmp17 << _tmp19;
  cpu.setXRegs(insn.operand2, _tmp21);
}

void doSLT(CPU &cpu, const Instruction &insn) {
  int32_t _tmp22;
  uint32_t _tmp23;
  _tmp23 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp22 = static_cast<int32_t>(_tmp23);
  int32_t _tmp24;
  uint32_t _tmp25;
  _tmp25 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp24 = static_cast<int32_t>(_tmp25);
  uint8_t _tmp26;
  _tmp26 = _tmp22 < _tmp24;
  uint32_t _tmp27;
  _tmp27 = static_cast<uint32_t>(_tmp26);
  cpu.setXRegs(insn.operand2, _tmp27);
}

void doSLTU(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp28;
  uint32_t _tmp29;
  _tmp29 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp28 = static_cast<uint32_t>(_tmp29);
  uint32_t _tmp30;
  uint32_t _tmp31;
  _tmp31 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp30 = static_cast<uint32_t>(_tmp31);
  uint8_t _tmp32;
  _tmp32 = _tmp28 < _tmp30;
  uint32_t _tmp33;
  _tmp33 = static_cast<uint32_t>(_tmp32);
  cpu.setXRegs(insn.operand2, _tmp33);
}

void doXOR(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp34;
  uint32_t _tmp35;
  _tmp35 = cpu.getXRegs<uint32_t>(insn.operand1);
  uint32_t _tmp36;
  _tmp36 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp34 = _tmp35 ^ _tmp36;
  cpu.setXRegs(insn.operand2, _tmp34);
}

void doSRL(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp37;
  uint32_t _tmp38;
  _tmp38 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp37 = static_cast<uint32_t>(_tmp38);
  uint32_t _tmp39;
  uint32_t _tmp40;
  _tmp40 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp39 = static_cast<uint32_t>(_tmp40);
  uint32_t _tmp41;
  _tmp41 = _tmp37 >> _tmp39;
  cpu.setXRegs(insn.operand2, _tmp41);
}

void doSRA(CPU &cpu, const Instruction &insn) {
  int32_t _tmp42;
  uint32_t _tmp43;
  _tmp43 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp42 = static_cast<int32_t>(_tmp43);
  int32_t _tmp44;
  uint32_t _tmp45;
  _tmp45 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp44 = static_cast<int32_t>(_tmp45);
  int32_t _tmp46;
  _tmp46 = _tmp42 >> _tmp44;
  cpu.setXRegs(insn.operand2, _tmp46);
}

void doOR(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp47;
  uint32_t _tmp48;
  _tmp48 = cpu.getXRegs<uint32_t>(insn.operand1);
  uint32_t _tmp49;
  _tmp49 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp47 = _tmp48 | _tmp49;
  cpu.setXRegs(insn.operand2, _tmp47);
}

void doAND(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp50;
  uint32_t _tmp51;
  _tmp51 = cpu.getXRegs<uint32_t>(insn.operand1);
  uint32_t _tmp52;
  _tmp52 = cpu.getXRegs<uint32_t>(insn.operand0);
  _tmp50 = _tmp51 & _tmp52;
  cpu.setXRegs(insn.operand2, _tmp50);
}

void doADDI(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp54;
  uint32_t _tmp55;
  _tmp55 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp54 = _tmp55 + insn.operand0;
  cpu.setXRegs(insn.operand2, _tmp54);
}

void doSLTI(CPU &cpu, const Instruction &insn) {
  int32_t _tmp57;
  uint32_t _tmp58;
  _tmp58 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp57 = static_cast<int32_t>(_tmp58);
  uint8_t _tmp59;
  _tmp59 = _tmp57 < insn.operand0;
  uint32_t _tmp60;
  _tmp60 = static_cast<uint32_t>(_tmp59);
  cpu.setXRegs(insn.operand2, _tmp60);
}

void doSLTIU(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp62;
  uint32_t _tmp63;
  _tmp63 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp62 = static_cast<uint32_t>(_tmp63);
  uint32_t _tmp64;
  _tmp64 = static_cast<uint32_t>(insn.operand0);
  uint8_t _tmp65;
  _tmp65 = _tmp62 < _tmp64;
  uint32_t _tmp66;
  _tmp66 = static_cast<uint32_t>(_tmp65);
  cpu.setXRegs(insn.operand2, _tmp66);
}

void doXORI(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp68;
  uint32_t _tmp69;
  _tmp69 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp68 = _tmp69 ^ insn.operand0;
  cpu.setXRegs(insn.operand2, _tmp68);
}

void doORI(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp71;
  uint32_t _tmp72;
  _tmp72 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp71 = _tmp72 | insn.operand0;
  cpu.setXRegs(insn.operand2, _tmp71);
}

void doANDI(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp74;
  uint32_t _tmp75;
  _tmp75 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp74 = _tmp75 & insn.operand0;
  cpu.setXRegs(insn.operand2, _tmp74);
}

void doSLLI(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp76;
  uint32_t _tmp77;
  _tmp77 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp76 = _tmp77 << insn.operand0;
  cpu.setXRegs(insn.operand2, _tmp76);
}

void doSRLI(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp78;
  uint32_t _tmp79;
  _tmp79 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp78 = _tmp79 >> insn.operand0;
  cpu.setXRegs(insn.operand2, _tmp78);
}

void doSRAI(CPU &cpu, const Instruction &insn) {
  int32_t _tmp80;
  uint32_t _tmp81;
  _tmp81 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp80 = static_cast<int32_t>(_tmp81);
  int32_t _tmp82;
  _tmp82 = _tmp80 >> insn.operand0;
  cpu.setXRegs(insn.operand2, _tmp82);
}

void doBEQ(CPU &cpu, const Instruction &insn) {
  uint8_t _tmp99;
  uint32_t _tmp100;
  _tmp100 = cpu.getXRegs<uint32_t>(insn.operand1);
  uint32_t _tmp101;
  _tmp101 = cpu.getXRegs<uint32_t>(insn.operand2);
  _tmp99 = _tmp100 == _tmp101;
  uint32_t _tmp102;
  _tmp102 = cpu.getPC() + insn.operand0;
  uint32_t _tmp104;
  _tmp104 = cpu.getPC() + 4;
  uint32_t _tmp105;
  _tmp105 = _tmp99 ? _tmp102 : _tmp104;
  cpu.setPC(_tmp105);
}

void doBNE(CPU &cpu, const Instruction &insn) {
  uint8_t _tmp122;
  uint32_t _tmp123;
  _tmp123 = cpu.getXRegs<uint32_t>(insn.operand1);
  uint32_t _tmp124;
  _tmp124 = cpu.getXRegs<uint32_t>(insn.operand2);
  _tmp122 = _tmp123 != _tmp124;
  uint32_t _tmp125;
  _tmp125 = cpu.getPC() + insn.operand0;
  uint32_t _tmp127;
  _tmp127 = cpu.getPC() + 4;
  uint32_t _tmp128;
  _tmp128 = _tmp122 ? _tmp125 : _tmp127;
  cpu.setPC(_tmp128);
}

void doBLT(CPU &cpu, const Instruction &insn) {
  int32_t _tmp145;
  uint32_t _tmp146;
  _tmp146 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp145 = static_cast<int32_t>(_tmp146);
  int32_t _tmp147;
  uint32_t _tmp148;
  _tmp148 = cpu.getXRegs<uint32_t>(insn.operand2);
  _tmp147 = static_cast<int32_t>(_tmp148);
  uint8_t _tmp149;
  _tmp149 = _tmp145 < _tmp147;
  uint32_t _tmp150;
  _tmp150 = cpu.getPC() + insn.operand0;
  uint32_t _tmp152;
  _tmp152 = cpu.getPC() + 4;
  uint32_t _tmp153;
  _tmp153 = _tmp149 ? _tmp150 : _tmp152;
  cpu.setPC(_tmp153);
}

void doBGE(CPU &cpu, const Instruction &insn) {
  int32_t _tmp170;
  uint32_t _tmp171;
  _tmp171 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp170 = static_cast<int32_t>(_tmp171);
  int32_t _tmp172;
  uint32_t _tmp173;
  _tmp173 = cpu.getXRegs<uint32_t>(insn.operand2);
  _tmp172 = static_cast<int32_t>(_tmp173);
  uint8_t _tmp174;
  _tmp174 = _tmp170 >= _tmp172;
  uint32_t _tmp175;
  _tmp175 = cpu.getPC() + insn.operand0;
  uint32_t _tmp177;
  _tmp177 = cpu.getPC() + 4;
  uint32_t _tmp178;
  _tmp178 = _tmp174 ? _tmp175 : _tmp177;
  cpu.setPC(_tmp178);
}

void doBLTU(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp195;
  uint32_t _tmp196;
  _tmp196 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp195 = static_cast<uint32_t>(_tmp196);
  uint32_t _tmp197;
  uint32_t _tmp198;
  _tmp198 = cpu.getXRegs<uint32_t>(insn.operand2);
  _tmp197 = static_cast<uint32_t>(_tmp198);
  uint8_t _tmp199;
  _tmp199 = _tmp195 < _tmp197;
  uint32_t _tmp200;
  _tmp200 = cpu.getPC() + insn.operand0;
  uint32_t _tmp202;
  _tmp202 = cpu.getPC() + 4;
  uint32_t _tmp203;
  _tmp203 = _tmp199 ? _tmp200 : _tmp202;
  cpu.setPC(_tmp203);
}

void doBGEU(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp220;
  uint32_t _tmp221;
  _tmp221 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp220 = static_cast<uint32_t>(_tmp221);
  uint32_t _tmp222;
  uint32_t _tmp223;
  _tmp223 = cpu.getXRegs<uint32_t>(insn.operand2);
  _tmp222 = static_cast<uint32_t>(_tmp223);
  uint8_t _tmp224;
  _tmp224 = _tmp220 >= _tmp222;
  uint32_t _tmp225;
  _tmp225 = cpu.getPC() + insn.operand0;
  uint32_t _tmp227;
  _tmp227 = cpu.getPC() + 4;
  uint32_t _tmp228;
  _tmp228 = _tmp224 ? _tmp225 : _tmp227;
  cpu.setPC(_tmp228);
}

void doJAL(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp246;
  _tmp246 = cpu.getPC() + 4;
  cpu.setXRegs(insn.operand1, _tmp246);
  uint32_t _tmp247;
  _tmp247 = cpu.getPC() + insn.operand0;
  cpu.setPC(_tmp247);
}

void doJALR(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp250;
  _tmp250 = cpu.getPC() + 4;
  uint32_t t;
  t = _tmp250;
  uint32_t _tmp251;
  uint32_t _tmp252;
  _tmp252 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp251 = _tmp252 + insn.operand0;
  uint32_t _tmp254;
  _tmp254 = _tmp251 & -2;
  cpu.setPC(_tmp254);
  cpu.setXRegs(insn.operand2, t);
}

void doSB(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp261;
  uint32_t _tmp262;
  _tmp262 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp261 = _tmp262 + insn.operand0;
  uint8_t _tmp263;
  uint32_t _tmp266;
  _tmp266 = cpu.getXRegs<uint32_t>(insn.operand2);
  _tmp263 = static_cast<uint8_t>(_tmp266 << 0);
  cpu.m_memory->write(_tmp261, _tmp263);
}

void doSH(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp273;
  uint32_t _tmp274;
  _tmp274 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp273 = _tmp274 + insn.operand0;
  uint16_t _tmp275;
  uint32_t _tmp278;
  _tmp278 = cpu.getXRegs<uint32_t>(insn.operand2);
  _tmp275 = static_cast<uint16_t>(_tmp278 << 0);
  cpu.m_memory->write(_tmp273, _tmp275);
}

void doSW(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp285;
  uint32_t _tmp286;
  _tmp286 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp285 = _tmp286 + insn.operand0;
  uint32_t _tmp287;
  _tmp287 = cpu.getXRegs<uint32_t>(insn.operand2);
  cpu.m_memory->write(_tmp285, _tmp287);
}

void doLB(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp289;
  uint32_t _tmp290;
  _tmp290 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp289 = _tmp290 + insn.operand0;
  uint8_t _tmp291;
  _tmp291 = cpu.m_memory->read<uint8_t>(_tmp289);
  int32_t _tmp292;
  _tmp292 = (static_cast<int32_t>(_tmp291) << 24) >> 24;
  cpu.setXRegs(insn.operand2, _tmp292);
}

void doLH(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp294;
  uint32_t _tmp295;
  _tmp295 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp294 = _tmp295 + insn.operand0;
  uint16_t _tmp296;
  _tmp296 = cpu.m_memory->read<uint16_t>(_tmp294);
  int32_t _tmp297;
  _tmp297 = (static_cast<int32_t>(_tmp296) << 16) >> 16;
  cpu.setXRegs(insn.operand2, _tmp297);
}

void doLW(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp299;
  uint32_t _tmp300;
  _tmp300 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp299 = _tmp300 + insn.operand0;
  uint32_t _tmp301;
  _tmp301 = cpu.m_memory->read<uint32_t>(_tmp299);
  cpu.setXRegs(insn.operand2, _tmp301);
}

void doLBU(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp303;
  uint32_t _tmp304;
  _tmp304 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp303 = _tmp304 + insn.operand0;
  uint8_t _tmp305;
  _tmp305 = cpu.m_memory->read<uint8_t>(_tmp303);
  uint32_t _tmp306;
  _tmp306 = static_cast<uint32_t>(_tmp305);
  cpu.setXRegs(insn.operand2, _tmp306);
}

void doLHU(CPU &cpu, const Instruction &insn) {
  uint32_t _tmp308;
  uint32_t _tmp309;
  _tmp309 = cpu.getXRegs<uint32_t>(insn.operand1);
  _tmp308 = _tmp309 + insn.operand0;
  uint16_t _tmp310;
  _tmp310 = cpu.m_memory->read<uint16_t>(_tmp308);
  uint32_t _tmp311;
  _tmp311 = static_cast<uint32_t>(_tmp310);
  cpu.setXRegs(insn.operand2, _tmp311);
}

void doECALL(CPU &cpu, const Instruction &insn) {
  cpu.doExit();
}

void doEBREAK(CPU &cpu, const Instruction &insn) {
}

void doFENCE(CPU &cpu, const Instruction &insn) {
}


size_t getILen(Opcode opc) {
  switch (opc) {
    case Opcode::kLUI: return 4;
    case Opcode::kAUIPC: return 4;
    case Opcode::kADD: return 4;
    case Opcode::kSUB: return 4;
    case Opcode::kSLL: return 4;
    case Opcode::kSLT: return 4;
    case Opcode::kSLTU: return 4;
    case Opcode::kXOR: return 4;
    case Opcode::kSRL: return 4;
    case Opcode::kSRA: return 4;
    case Opcode::kOR: return 4;
    case Opcode::kAND: return 4;
    case Opcode::kADDI: return 4;
    case Opcode::kSLTI: return 4;
    case Opcode::kSLTIU: return 4;
    case Opcode::kXORI: return 4;
    case Opcode::kORI: return 4;
    case Opcode::kANDI: return 4;
    case Opcode::kSLLI: return 4;
    case Opcode::kSRLI: return 4;
    case Opcode::kSRAI: return 4;
    case Opcode::kBEQ: return 4;
    case Opcode::kBNE: return 4;
    case Opcode::kBLT: return 4;
    case Opcode::kBGE: return 4;
    case Opcode::kBLTU: return 4;
    case Opcode::kBGEU: return 4;
    case Opcode::kJAL: return 4;
    case Opcode::kJALR: return 4;
    case Opcode::kSB: return 4;
    case Opcode::kSH: return 4;
    case Opcode::kSW: return 4;
    case Opcode::kLB: return 4;
    case Opcode::kLH: return 4;
    case Opcode::kLW: return 4;
    case Opcode::kLBU: return 4;
    case Opcode::kLHU: return 4;
    case Opcode::kECALL: return 4;
    case Opcode::kEBREAK: return 4;
    case Opcode::kFENCE: return 4;
    default: return 4;
  }
}

template <typename T>
constexpr auto toUnderlying(T val)
  requires std::is_enum_v<T>
{
  return static_cast<std::underlying_type_t<T>>(val);
}

class ExecHandlersMap {
public:
  using ExecHandler = void (*)(CPU &cpu, const Instruction &insn);

private:
  std::array<ExecHandler, 40> m_handlers{};

public:
  constexpr ExecHandlersMap() {
    m_handlers[toUnderlying(Opcode::kLUI)] = &doLUI;
    m_handlers[toUnderlying(Opcode::kAUIPC)] = &doAUIPC;
    m_handlers[toUnderlying(Opcode::kADD)] = &doADD;
    m_handlers[toUnderlying(Opcode::kSUB)] = &doSUB;
    m_handlers[toUnderlying(Opcode::kSLL)] = &doSLL;
    m_handlers[toUnderlying(Opcode::kSLT)] = &doSLT;
    m_handlers[toUnderlying(Opcode::kSLTU)] = &doSLTU;
    m_handlers[toUnderlying(Opcode::kXOR)] = &doXOR;
    m_handlers[toUnderlying(Opcode::kSRL)] = &doSRL;
    m_handlers[toUnderlying(Opcode::kSRA)] = &doSRA;
    m_handlers[toUnderlying(Opcode::kOR)] = &doOR;
    m_handlers[toUnderlying(Opcode::kAND)] = &doAND;
    m_handlers[toUnderlying(Opcode::kADDI)] = &doADDI;
    m_handlers[toUnderlying(Opcode::kSLTI)] = &doSLTI;
    m_handlers[toUnderlying(Opcode::kSLTIU)] = &doSLTIU;
    m_handlers[toUnderlying(Opcode::kXORI)] = &doXORI;
    m_handlers[toUnderlying(Opcode::kORI)] = &doORI;
    m_handlers[toUnderlying(Opcode::kANDI)] = &doANDI;
    m_handlers[toUnderlying(Opcode::kSLLI)] = &doSLLI;
    m_handlers[toUnderlying(Opcode::kSRLI)] = &doSRLI;
    m_handlers[toUnderlying(Opcode::kSRAI)] = &doSRAI;
    m_handlers[toUnderlying(Opcode::kBEQ)] = &doBEQ;
    m_handlers[toUnderlying(Opcode::kBNE)] = &doBNE;
    m_handlers[toUnderlying(Opcode::kBLT)] = &doBLT;
    m_handlers[toUnderlying(Opcode::kBGE)] = &doBGE;
    m_handlers[toUnderlying(Opcode::kBLTU)] = &doBLTU;
    m_handlers[toUnderlying(Opcode::kBGEU)] = &doBGEU;
    m_handlers[toUnderlying(Opcode::kJAL)] = &doJAL;
    m_handlers[toUnderlying(Opcode::kJALR)] = &doJALR;
    m_handlers[toUnderlying(Opcode::kSB)] = &doSB;
    m_handlers[toUnderlying(Opcode::kSH)] = &doSH;
    m_handlers[toUnderlying(Opcode::kSW)] = &doSW;
    m_handlers[toUnderlying(Opcode::kLB)] = &doLB;
    m_handlers[toUnderlying(Opcode::kLH)] = &doLH;
    m_handlers[toUnderlying(Opcode::kLW)] = &doLW;
    m_handlers[toUnderlying(Opcode::kLBU)] = &doLBU;
    m_handlers[toUnderlying(Opcode::kLHU)] = &doLHU;
    m_handlers[toUnderlying(Opcode::kECALL)] = &doECALL;
    m_handlers[toUnderlying(Opcode::kEBREAK)] = &doEBREAK;
    m_handlers[toUnderlying(Opcode::kFENCE)] = &doFENCE;
  }

  [[nodiscard]] ExecHandler get(Opcode opcode) const {
    assert(toUnderlying(opcode) < m_handlers.size());
    auto toRet = m_handlers[toUnderlying(opcode)];
    assert(toRet != nullptr);
    return toRet;
  }
};

constexpr ExecHandlersMap kExecHandlers{};
} // namespace

void Interpreter::execute(CPU &cpu, const Instruction &insn) {
  const auto handler = kExecHandlers.get(insn.m_opc);

  auto oldPC = cpu.getPC();

  handler(cpu, insn);

  if (!isBranchInstruction(insn)) {
    cpu.setPC(oldPC + getILen(insn.m_opc));
  }
}

} // namespace prot::engine
