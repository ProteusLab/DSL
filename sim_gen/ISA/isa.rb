module SimGen
    module ISA
        module Helper
            module_function
            def find_max_reg(regfiles)
                max_reg_size = 0
                regfiles.each do |regfile|
                    regfile[:regs].each do |reg|
                        if reg[:size] > max_reg_size
                            max_reg_size = reg[:size]
                        end
                    end
                end
                max_reg_size
            end

            def find_max_operands(instructions)
                max_operands = 0
                max_size = 0
                instructions.each do |insn|
                    operands_count = 0
                    insn[:map][:tree].each do |node|
                        if node[:name] == :new_var && !node[:attrs].nil? && node[:attrs].include?(:op)
                            operands_count += 1
                            max_size = Utility.get_type(node[:oprnds][0][:type]).bitsize if Utility.get_type(node[:oprnds][0][:type]).bitsize > max_size
                        end
                    end
                    max_operands = operands_count if operands_count > max_operands
                end
                [max_operands, max_size]
            end

            def generate_fields_struct(max_operands, max_size)
                emitter = Utility::GenEmitter.new
                for i in 0...max_operands
                    emitter.emit_line("uint#{max_size}_t operand#{i};")
                end
                emitter.increase_indent_all(2)
                emitter
            end

            def generate_instruction_struct(input_ir)
                max_operands, max_size = find_max_operands(input_ir[:instructions])
                fields_struct = generate_fields_struct(max_operands, max_size)
"struct Instruction {
  Opcode m_opc;
#{fields_struct.to_s}
};"
            end

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
    module ISA
        module Header
            module_function

            def generate_isa_header(input_ir)
                type = Helper.get_addr_type input_ir[:instructions]
                type_str = Utility::HelperCpp::gen_type type

                instruction_struct = Helper.generate_instruction_struct(input_ir)
"#ifndef GENERATED_#{input_ir[:isa_name].upcase}_ISA_HH_INCLUDED
#define GENERATED_#{input_ir[:isa_name].upcase}_ISA_HH_INCLUDED

#include <cstdint>

namespace prot::isa {
using Addr = #{type_str};

enum class Opcode : uint32_t {
#{input_ir[:instructions].map { |insn| "  k#{insn[:name].to_s.upcase}," }.join("\n")}
};

#{instruction_struct.to_s}

} // namespace prot::isa

#endif // GENERATED_#{input_ir[:isa_name].upcase}_ISA_HH_INCLUDED"
            end
        end
    end
end