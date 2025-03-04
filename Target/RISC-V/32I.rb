require_relative "encoding"
require_relative "../../Generic/base"

module RV32I
    SimInfra.Instruction(:add) {
        encoding *SimInfra.format_r_alu(:add)
        code { rd[]= rs1 + rs2 }
    }

    SimInfra.Instruction(:sub) {
        encoding *SimInfra.format_r_alu(:sub)
        code { rd[]= rs1 - rs2 }
    }
end