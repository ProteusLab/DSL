# frozen_string_literal: true

module Utility
  # GenEmitter is a utility class for generating code with proper indentation and formatting.
  class GenEmitter
    attr_reader :code, :indent_level, :indent_size

    def initialize(indent_size = 2)
      @code = []
      @indent_level = 0
      @indent_size = indent_size
    end

    # Increases or decreases the current indentation level. Identation is represented by 4 spaces.
    def increase_indent
      @indent_level += 1
    end

    def decrease_indent
      @indent_level -= 1
    end

    # Emits a line of code with the current indentation level.
    def emit_line(line)
      raise 'Line should be string' unless line.is_a? String
      @code << "#{' ' * @indent_size * @indent_level}#{line}"
    end

    # Emits a blank line.
    def emit_blank_line
      @code << ""
    end

    # Returns the generated code as a string.
    def to_s
      @code.join("\n")
    end

    # Alias for emit_line, allowing usage like emitter << "line of code"
    def <<(line)
      emit_line line
    end

    # Concat two gen emitters
    def +(other)
        combined = GenEmitter.new(@indent_size)
        combined.code << @code
        other_lines = other.code.lines.map do |line|
            (' ' * @indent_size) + line
        end
        combined.code << other_lines.join
        combined
    end

    def concat(other)
        @code.concat(other.code)
    end

    def increase_indent_all(indent_level)
      @code = @code.map do |line|
        (' ' * indent_level) + line
      end
    end
  end
end
