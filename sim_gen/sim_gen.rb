#!/usr/bin/ruby
# frozen_string_literal: true

require 'sim_gen/CPUState/cpu_state'
require 'sim_gen/Decoders/decoder'
require 'sim_gen/ISA/isa'
require 'sim_gen/ExecEngines/naive_interpreter'
require 'sim_gen/ExecEngines/base_exec_engine'
require 'sim_gen/MemoryModels/memory'
require 'sim_gen/Hart/hart'

require 'yaml'

yaml_data = YAML.load_file('sim_lib/generated/IR.yaml')
yaml_data[:isa_name] = "RISCV"


File.write('sim_lib/generated/cpu_state.hh', SimGen::CPUState::Header.generate_cpu_state(yaml_data))
File.write('sim_lib/generated/base_exec_engine.hh', SimGen::BaseExecEngine::Header.generate_base_exec_engine(yaml_data))
File.write('sim_lib/generated/base_exec_engine.cc', SimGen::BaseExecEngine::TranslationUnit.generate_base_exec_engine(yaml_data))
File.write('sim_lib/generated/naive_interpreter.hh', SimGen::NaiveInterpreter::Header.generate_naive_interpreter(yaml_data))
File.write('sim_lib/generated/naive_interpreter.cc', SimGen::NaiveInterpreter::TranslationUnit.generate_naive_interpreter(yaml_data))
File.write('sim_lib/generated/decoder.hh', SimGen::Decoder::Header.generate_decoder(yaml_data))
File.write('sim_lib/generated/decoder.cc', SimGen::Decoder::TranslationUnit.generate_decoder(yaml_data))
File.write('sim_lib/generated/isa.hh', SimGen::ISA::Header.generate_isa_header(yaml_data))
File.write('sim_lib/generated/memory.hh', SimGen::Memory::Header.generate_memory(yaml_data))
File.write('sim_lib/generated/memory.cc', SimGen::Memory::TranslationUnit.generate_memory(yaml_data))
File.write('sim_lib/generated/hart.hh', SimGen::Hart::Header.generate_hart(yaml_data))
File.write('sim_lib/generated/hart.cc', SimGen::Hart::TranslationUnit.generate_hart(yaml_data))

puts SimGen::ISA::Helper.generate_instruction_struct(yaml_data)

# require 'yaml'
# data = YAML.safe_load(
#   SimInfra.state,
#   permitted_classes: [Symbol],
#   symbolize_names: true
# )

# puts(SimInfra.generate_exec_handler(data))
