require_relative "scope"

module SimInfra
    IrStmt = Struct.new(:name, :oprnds, :attrs)
end

# Basics
module SimInfra
    def assert(condition, msg = nil); raise msg if !condition; end

    @@instructions = []
    InstructionInfo= Struct.new(:name, :fields, :frmt, :code)
    class InstructionInfoBuilder
        def initialize(name); @info = InstructionInfo.new(name); end
        def encoding(frmt, fields); @info.fields = fields; @info.frmt= frmt; end
        attr_reader :info
    end

    def self.Instruction(name, &block)
        bldr = InstructionInfoBuilder.new(name)
        bldr.instance_eval &block
        @@instructions << bldr.info
        nil # only for debugging in IRB
    end
end

# * generate precise fields
module SimInfra
    class InstructionInfoBuilder
    def code(&block)
        @info.code = scope = Scope.new(nil) # root scope
        dst = nil
        @info.fields.each { |f|
            scope.add_var(f.name, :i32) if [:rs1, :rs2, :rd].include?(f.name)
            scope.stmt(:getreg, [f.name, f]) if [:rs1, :rs2].include?(f.name)
            dst = f if [:rd].include?(f.name)
        }
        scope.instance_eval &block
        scope.stmt(:setreg, [dst, dst.name]) if dst
        end
    end
end
