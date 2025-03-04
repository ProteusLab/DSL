# Testing infra

module SimInfra
    # @@instructions -array of instruction description
    # shows result of our tests in interactive Ruby (IRB) or standalone
    def self.serialize(msg= nil)
        return @@instructions if Object.const_defined?(:IRB)
        require 'pp'
        puts "\n\n\n#{'*' * 100}\n#{' ' * 30}#{msg}\n\n\n"
        PP.pp @@instructions
    end

    # reset state
    def siminfra_reset_module_state; @@instructions = []; end

    # mixin for global counter, function returns 0,1,2,....
    module GlobalCounter
        @@counter = -1
        def next_counter; @@counter += 1; end
    end

    Field = Struct.new(:name, :from, :to, :value)
    ImmFieldPart = Struct.new(:name, :from, :to, :hi, :lo)

    def self.field(name, from, to, value = nil)
        Field.new(name, from, to, value).freeze
    end
    def self.immpart(name, from, to, hi, lo)
        ImmFieldPart.new(name, from, to, hi, lo).freeze
    end

    def assert(condition, msg = nil); raise msg if !condition; end
end
