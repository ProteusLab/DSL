require 'Utility/helper_cpp'
# frozen_string_literal: true

# Semantics Generator: Converts IR to C++ code
class SemaGen
  def initialize(emitter, mapping = {})
    @emitter = emitter
    @mapping = mapping
  end

  def binary_operation(emitter, operation, op_str)
    dst = @mapping[operation[:oprnds][0][:name]] || operation[:oprnds][0][:name]
    src1 = @mapping[operation[:oprnds][1][:name]] || operation[:oprnds][1][:name]
    src2 = @mapping[operation[:oprnds][2][:name]] || operation[:oprnds][2][:name]

    src1 = src1.nil? ? operation[:oprnds][1][:value] : src1
    src2 = src2.nil? ? operation[:oprnds][2][:value] : src2

    emitter.emit_line("#{dst} = #{src1} #{op_str} #{src2};")
  end

  def self.generate_statement(operation)
    emitter = Utility::GenEmitter.new
    SemaGen.new(emitter).generate_statement(operation)
    emitter.to_s
  end

  def generate_statement(operation)
    case operation[:name]
    when :add
      binary_operation(@emitter, operation, '+')
    when :sub
      binary_operation(@emitter, operation, '-')
    when :mul
      binary_operation(@emitter, operation, '*')
    when :div
      binary_operation(@emitter, operation, '/')
    when :shr
      binary_operation(@emitter, operation, '>>')
    when :shl
      binary_operation(@emitter, operation, '<<')
    when :and
      binary_operation(@emitter, operation, '&')
    when :or
      binary_operation(@emitter, operation, '|')
    when :xor
      binary_operation(@emitter, operation, '^')
    when :lt
      binary_operation(@emitter, operation, '<')
    when :gt
      binary_operation(@emitter, operation, '>')
    when :le
      binary_operation(@emitter, operation, '<=')
    when :ge
      binary_operation(@emitter, operation, '>=')
    when :eq
      binary_operation(@emitter, operation, '==')
    when :ne
      binary_operation(@emitter, operation, '!=')
    when :let
      dst = @mapping[operation[:oprnds][0][:name]] || operation[:oprnds][0][:name]
      src = @mapping[operation[:oprnds][1][:name]] || operation[:oprnds][1][:name]
      src = src.nil? ? operation[:oprnds][1][:value] : src
      @emitter.emit_line("#{dst} = #{src};")
    when :new_var
      var_name = operation[:oprnds][0][:name]
      var_type = Utility::HelperCpp.gen_type(operation[:oprnds][0][:type])
      @emitter.emit_line("#{var_type} #{var_name};")
    when :cast
      dst = @mapping[operation[:oprnds][0][:name]] || operation[:oprnds][0][:name]
      src = @mapping[operation[:oprnds][1][:name]] || operation[:oprnds][1][:name]
      src = src.nil? ? operation[:oprnds][1][:value] : src
      bitsize_dst = Utility.get_type(operation[:oprnds][0][:type]).bitsize
      bitsize_src = Utility.get_type(operation[:oprnds][1][:type]).bitsize
      cast_type = Utility::HelperCpp.gen_type(operation[:oprnds][0][:type])
      if Utility.get_type(operation[:oprnds][0][:type]).typeof == :s && bitsize_src < bitsize_dst
        @emitter.emit_line("#{dst} = (static_cast<#{cast_type}>(#{src}) << #{bitsize_dst - bitsize_src}) >> #{bitsize_dst - bitsize_src};")
      else
        @emitter.emit_line("#{dst} = static_cast<#{cast_type}>(#{src});")
      end
    end
  end
end
