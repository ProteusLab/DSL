module SimGen
  module Helper
    module_function

      def find_max_xlen(regfiles)
        max_xlen = 0
        regfiles.each do |regfile|
          regfile[:regs].each do |reg|
            max_xlen = [max_xlen, reg[:size]].max
          end
        end
        max_xlen
      end
  end
end
