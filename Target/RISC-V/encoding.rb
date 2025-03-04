require_relative "../../Generic/base"

module SimInfra
    def self.format_r(opcode, funct3, funct7)
        return :R, [
            field(:rd, 11, 7, :reg),
            field(:rs1, 0, 0, :reg),
            field(:rs2, 0, 0, :reg),
            field(:opcode, 0, 0, opcode),
            field(:funct7, 0, 0, funct7),
            field(:funct3, 0, 0, funct3),
        ]
    end

    def self.format_r_alu(name)
        funct3, funct7 =
        {
            add: [0,0],
            sub: [0, 1 << 5]
        }[name]
        format_r 0, funct3, funct7
    end
end
