#!/usr/bin/ruby
# frozen_string_literal: true

require 'ADL/base'
require 'ADL/builder'
require 'Target/RISC-V/32I'

require 'yaml'

SimInfra.serialize

yaml_data = YAML.safe_load(
  SimInfra.state,
  permitted_classes: [Symbol],
  symbolize_names: true
)

File.write('sim_lib/generated/IR.yaml', yaml_data.to_yaml)
