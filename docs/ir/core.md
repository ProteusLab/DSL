Core operations
===============

This section describes the *core* ProteaIR operations. The type of an operation is defined by the operand types. For instance, `shr` is a logical shift when applied to bitvector types or unsigned integers, and an arithmetic shift when applied to signed integers.

Types
-----

Core ProteaIR defines three primitive types:
* `bN`: N-bit type, for instance `b32`, `b7`.
* `sN`: N-bit signed integer type, two's complement.
* `uN`: N-bit unsigned integer type, unsigned binary representation.

Variable and Conversion Operations
----------------------------------

This section describes operations related to variable definition, binding, and explicit type conversion.

* `new_var`: Variable declaration.
  Introduces a new variable with a specified type. The variable is uninitialized unless explicitly assigned later.

* `let`: Variable binding.
  Assign variable with value. Destination operand shall be with same type as source.

* `cast`: Explicit type conversion.
  Converts a value from one type to another. Casts must be explicit and are only permitted between compatible types as defined by the ProteaIR type system. The semantics of the conversion depend on the source and target types, including possible truncation or sign interpretation changes.

Arithmetic
----------

These instructions define basic arithmetic and bitwise operations. All operations take two operands of the same type and produce a result of that type, unless stated otherwise. Operand and result bit widths are identical.

* `add`: Integer addition.
  Computes the sum of the two operands. Overflow is handled according to the rules of the operand type (two’s complement wrap-around for signed integers, modulo 2^N for unsigned integers and bitvectors).

* `sub`: Integer subtraction.
  Computes the difference of the first operand minus the second. Overflow and underflow follow the same rules as for `add`.

* `mul`: Integer multiplication.
  Computes the product of the two operands. The result is truncated to N bits. Overflow is handled according to the operand type representation.

* `div`: Integer division.
  Computes the quotient of the first operand divided by the second.
  For signed integers, division semantics follow two’s complement rules.
  Division by zero is an error.

* `shl`: Logical left shift.
  Shifts the first operand left by the number of bits specified by the second operand. Vacated low-order bits are filled with zeros. Bits shifted out of the high end are discarded.

* `shr`: Right shift.
  For unsigned integers and bitvector types, this is a logical right shift: vacated high-order bits are filled with zeros.
  For signed integers, this operation performs an arithmetic right shift, preserving the sign bit.

* `and`: Bitwise conjunction.
  Performs a bitwise AND of the two operands.

* `or`: Bitwise disjunction.
  Performs a bitwise OR of the two operands.

* `xor`: Bitwise exclusive disjunction.
  Performs a bitwise XOR of the two operands.

Comparison
----------

This section describes comparison operations. Comparison operations take two operands of the same type and produce a `b1` result.

* `eq`: Equality.
  Evaluates to true if both operands are equal.

* `ne`: Inequality.
  Evaluates to true if the operands are not equal.

* `lt`: Less-than comparison.
  Evaluates to true if the first operand is strictly less than the second.

* `le`: Less-than-or-equal comparison.
  Evaluates to true if the first operand is less than or equal to the second.

* `gt`: Greater-than comparison.
  Evaluates to true if the first operand is strictly greater than the second.

* `ge`: Greater-than-or-equal comparison.
  Evaluates to true if the first operand is greater than or equal to the second.


