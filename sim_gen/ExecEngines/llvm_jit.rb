module SimGen
  class LLVMJIT
    attr_reader :input_ir
    
    def initialize(input_ir)
      @input_ir = input_ir
    end

    def generate_header
      emitter = Utility::GenEmitter.new
      
    end

    def generate_translation_unit
      emitter = Utility::GenEmitter.new
    end
  end
end
