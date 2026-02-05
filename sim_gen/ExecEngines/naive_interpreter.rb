module SimGen
  module NaiveInterpreter
    module Header
      module_function

      def generate_naive_interpreter(input_ir)
        "#ifndef GENERATED_#{input_ir[:isa_name].upcase}_INTERPRETER_HH_INCLUDED
#define GENERATED_#{input_ir[:isa_name].upcase}_INTERPRETER_HH_INCLUDED

#include \"base_exec_engine.hh\"

namespace prot::engine {
using namespace prot::state;
using namespace prot::isa;

class Interpreter : public ExecEngine {
public:
void execute(CPU &cpu, const Instruction &insn) override;
};
} // namespace generated::#{input_ir[:isa_name].downcase}::engine

#endif // GENERATED_#{input_ir[:isa_name].downcase}_INTERPRETER_HH_INCLUDED
"
      end
    end
  end
end

module SimGen
  module NaiveInterpreter
    module TranslationUnit
      module_function

      def map_operands(insn)
        operands = {}
        cnt = 0
        for node in insn[:map][:tree]
          if node[:name] == :new_var && !node[:attrs].nil? && node[:attrs].include?(:op)
            operands[node[:oprnds][0][:name]] = "insn.operand#{cnt}"
            cnt += 1
          end
        end
        operands[:pc] = 'cpu.getPC()'
        operands
      end

      def emit_binary_op(emitter, operand_map, op, dest, src1, src2)
        var = operand_map[dest[:name]] || dest[:name]
        expr1 = operand_map[src1[:name]] || src1[:name]
        expr2 = operand_map[src2[:name]] || src2[:name]
        expr1 = expr1.nil? ? src1[:value] : expr1
        expr2 = expr2.nil? ? src2[:value] : expr2
        emitter.emit_line("#{var} = #{expr1} #{op} #{expr2};")
      end

      def cpu_write_reg(dst)
        "cpu.set#{dst[:regset]}"
      end

      def cpu_read_reg(dst)
        "cpu.get#{dst[:regset]}"
      end

      def cpu_write_mem(addr, val)
        "cpu.m_memory->write(#{addr}, #{val})"
      end

      def cpu_read_mem(dst, addr)
        "cpu.m_memory->read<#{Utility::HelperCpp.gen_small_type(dst[:type])}>(#{addr})"
      end

      def generate_code_node(node, operand_map, emitter)
        case node[:name]
        when :new_var
          var_name = node[:oprnds][0][:name]
          var_type = Utility::HelperCpp.gen_small_type(node[:oprnds][0][:type])
          emitter.emit_line("#{var_type} #{var_name};")
        when :let
          var_name = operand_map[node[:oprnds][0][:name]] || node[:oprnds][0][:name]
          expr_name = operand_map[node[:oprnds][1][:name]] || node[:oprnds][1][:name]
          expr_name = expr_name.nil? ? node[:oprnds][1][:value] : expr_name
          emitter.emit_line("#{var_name} = #{expr_name};")
        when :cast
          var = operand_map[node[:oprnds][0][:name]] || node[:oprnds][0][:name]
          expr = operand_map[node[:oprnds][1][:name]] || node[:oprnds][1][:name]
          expr = expr.nil? ? node[:oprnds][1][:value] : expr
          bitsize_dst = Utility.get_type(node[:oprnds][0][:type]).bitsize
          bitsize_src = Utility.get_type(node[:oprnds][1][:type]).bitsize
          cast_type = Utility::HelperCpp.gen_type(node[:oprnds][0][:type])
          if Utility.get_type(node[:oprnds][0][:type]).typeof == :s && bitsize_src < bitsize_dst
            emitter.emit_line("#{var} = (static_cast<#{cast_type}>(#{expr}) << #{bitsize_dst - bitsize_src}) >> #{bitsize_dst - bitsize_src};")
          else
            emitter.emit_line("#{var} = static_cast<#{cast_type}>(#{expr});")
          end
        when :add
          emit_binary_op(emitter, operand_map, '+', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :sub
          emit_binary_op(emitter, operand_map, '-', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :shl
          emit_binary_op(emitter, operand_map, '<<', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :shr
          emit_binary_op(emitter, operand_map, '>>', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :and
          emit_binary_op(emitter, operand_map, '&', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :or
          emit_binary_op(emitter, operand_map, '|', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :xor
          emit_binary_op(emitter, operand_map, '^', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :lt
          emit_binary_op(emitter, operand_map, '<', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :gt
          emit_binary_op(emitter, operand_map, '>', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :le
          emit_binary_op(emitter, operand_map, '<=', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :ge
          emit_binary_op(emitter, operand_map, '>=', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :eq
          emit_binary_op(emitter, operand_map, '==', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :ne
          emit_binary_op(emitter, operand_map, '!=', node[:oprnds][0], node[:oprnds][1], node[:oprnds][2])
        when :readReg
          src = node[:oprnds][1]
          src_name = operand_map[node[:oprnds][1][:name]] || node[:oprnds][1][:name]
          expr = operand_map[node[:oprnds][0][:name]] || node[:oprnds][0][:name]
          expr = expr.nil? ? node[:oprnds][0][:value] : expr

          emitter.emit_line("#{expr} = #{cpu_read_reg(src)}<#{Utility::HelperCpp.gen_small_type(src[:type])}>(#{src_name});")
        when :writeReg
          dst = node[:oprnds][0]
          dst_name = operand_map[node[:oprnds][0][:name]] || node[:oprnds][0][:name]
          expr = operand_map[node[:oprnds][1][:name]] || node[:oprnds][1][:name]
          expr = expr.nil? ? node[:oprnds][1][:value] : expr

          emitter.emit_line("#{cpu_write_reg(dst)}(#{dst_name}, #{expr});")
        when :select
          dst = operand_map[node[:oprnds][0][:name]] || node[:oprnds][0][:name]
          cond = operand_map[node[:oprnds][1][:name]] || node[:oprnds][1][:name]

          true_val = operand_map[node[:oprnds][2][:name]] || node[:oprnds][2][:name]
          false_val = operand_map[node[:oprnds][3][:name]] || node[:oprnds][3][:name]

          emitter.emit_line("#{dst} = #{cond} ? #{true_val} : #{false_val};")
        when :branch
          val = operand_map[node[:oprnds][0][:name]] || node[:oprnds][0][:name]
          emitter.emit_line("cpu.setPC(#{val});")
        when :readMem
          dst = operand_map[node[:oprnds][0][:name]] || node[:oprnds][0][:name]
          addr = operand_map[node[:oprnds][1][:name]] || node[:oprnds][1][:name]
          emitter.emit_line("#{dst} = #{cpu_read_mem node[:oprnds][0], addr};")
        when :writeMem
          addr = operand_map[node[:oprnds][0][:name]] || node[:oprnds][0][:name]
          val = operand_map[node[:oprnds][1][:name]] || node[:oprnds][1][:name]
          emitter.emit_line("#{cpu_write_mem addr, val};")
        when :extract
          dst = operand_map[node[:oprnds][0][:name]] || node[:oprnds][0][:name]
          src = operand_map[node[:oprnds][1][:name]] || node[:oprnds][1][:name]

          emitter.emit_line("#{dst} = static_cast<#{Utility::HelperCpp.gen_small_type node[:oprnds][0][:type]}>(#{src} << #{node[:oprnds][3][:value]});")
        when :sysCall
          emitter.emit_line('cpu.doExit();')
        end
      end

      def generate_exec_function(instruction)
        emitter = Utility::GenEmitter.new
        operand_map = map_operands(instruction)

        emitter.emit_line("void do#{instruction[:name].to_s.upcase}(CPU &cpu, const Instruction &insn) {")
        emitter.increase_indent
        instruction[:code][:tree].each do |node|
          generate_code_node(node, operand_map, emitter)
        end
        emitter.decrease_indent
        emitter.emit_line('}')
        emitter.emit_blank_line
        emitter
      end

      def generate_exec_functions(input_ir)
        emitter = Utility::GenEmitter.new
        input_ir[:instructions].each do |instruction|
          temp_emitter = generate_exec_function(instruction)
          emitter.concat(temp_emitter)
        end
        emitter
      end

      def is_branch_instruction(instruction)
        for node in instruction[:code][:tree]
          return true if node[:name] == :branch
        end
        false
      end

      def generate_function_is_branch(input_ir)
        emitter = Utility::GenEmitter.new
        emitter.emit_line('bool isBranchInstruction(const Instruction &insn) {')
        emitter.increase_indent
        emitter.emit_line('switch (insn.m_opc) {')
        emitter.increase_indent
        input_ir[:instructions].each do |instruction|
          emitter.emit_line("case Opcode::k#{instruction[:name].to_s.upcase}:") if is_branch_instruction(instruction)
        end
        emitter.emit_line('return true;')
        emitter.decrease_indent
        emitter.emit_line('default:')
        emitter.increase_indent
        emitter.emit_line('return false;')
        emitter.decrease_indent
        emitter.emit_line('}')
        emitter.decrease_indent
        emitter.emit_line('}')
        emitter.increase_indent_all(2)
        emitter
      end

      def generate_naive_interpreter(input_ir)
        is_branch_function = generate_function_is_branch(input_ir)
        exec_functions = generate_exec_functions(input_ir)

        "#include \"naive_interpreter.hh\"

#include <cassert>

namespace prot::engine {
using namespace prot::state;
using namespace prot::isa;

namespace {
#{is_branch_function}

#{exec_functions}

size_t getILen(Opcode opc) {
  switch (opc) {
    #{input_ir[:instructions].map { |insn| "case Opcode::k#{insn[:name].to_s.upcase}: return #{insn[:XLEN]};" }.join("\n    ")}
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
  std::array<ExecHandler, #{input_ir[:instructions].size}> m_handlers{};

public:
  constexpr ExecHandlersMap() {
    #{input_ir[:instructions].map { |insn| "m_handlers[toUnderlying(Opcode::k#{insn[:name].to_s.upcase})] = &do#{insn[:name].to_s.upcase};" }.join("\n    ")}
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
"
      end
    end
  end
end
