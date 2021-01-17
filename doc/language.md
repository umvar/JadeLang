# JadeLang Language

## Overview
A JadeLang program is composed of global definitions and a yielding expression. Global definitions are composed of
function definitions and variable definitions. The result of the yielding expression is the output of the program.

### Example
```
; Global Definitions
(
	K := 10,
	f(x) := x + K
),
; Yielding Expression
f(3 + K)
```

The example yields `f(3 + 10) = (3 + 10) + 10 = 23`.

## Functions
A JadeLang function is a parametric expression whose parameters are assigned to at a call-site. In other words, the
output of a JadeLang function is the result of an expression, which may be comprised of the function's inputs.

### Example
```
(
	f(x) := #(x > 0, x * f(x - 1), 1)
),
f(10)
```

The function `f` is a recursive function that computes the factorial of a number.

## Variables
There are two types of variables in JadeLang: global variables and local variables. All variables are defined using the
`:=` operator, and local variables can be reassigned using the `=` operator.

## Global Variables
Global variables are immutable (i.e., cannot be assigned to) and erased at compile-time (i.e., must be initialized with
a constant expression). Unlike local variables, global variables can be accessed at any location. A constant expression
is an expression that can be evaluated at compile-time. All primary expressions of the initializing expression must be
integer literals or the identifiers of other global variables. Labeled expressions are also supported. There are no
restrictions placed on how long the computation of a constant expression is allowed to take.

### Example
```
(
	K := (
		s = 0,
		i = 1,
		#(i <= 10, (
			s = s + i,
			i = i + 1,
			#
		)),
		s
	)
),
K
```

Defines the global variable `K` as the sum of all integers from 1 to 10 (inclusive). The resulting bytecode is:
```
imm 55
hlt
```

## Local Variables
Local variables are mutable and only accessible within its scope.
