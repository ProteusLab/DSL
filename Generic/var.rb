require_relative "base"

module SimInfra
    IrStmt = Struct.new(:name, :oprnds, :attrs)
    class Var
        attr_reader :scope, :name, :type
        def initialize(scope, name, type)
            @scope = scope; @name = name; @type = type;
        end
        # Syntax "var[]=value" is used to assign variable
        # it's similar to "var[hi:lo]=value" for partial assignment
        def []=(other); @scope.stmt(:let, [self, other]); end
        # dumps states and disables @scope dump
        def inspect; "#{@name}:#{@type} (#{@scope.object_id})"; end
    end
end

# * constant resolution
module SimInfra
    class Constant
        attr_reader :scope, :name, :type, :value
        def initialize(scope, name, value);
            @const = value; @scope = scope; @type = :iconst; @value = value
            @scope.stmt(:new_const, [@const]);
        end
        def let(other); raise "Assign to constant"; end
        def inspect; "#{@name}:#{@type} (#{@scope.object_id}) {=#{@const}}"; end
    end
    #
    class Var
        def+(other); @scope.add(self, other); end
        def-(other); @scope.sub(self, other); end
    end
end