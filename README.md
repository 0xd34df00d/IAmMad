IAmMad - Inlining Automagical Metaprogramming Analytical Differentiation.

Cuz I'm too lazy to write derivatives in my C++ code by hand.

# Basic information

IAmMad is a library to perform analytical differentiation of various (simple)
mathematical expressions _in compile time_.

The primary benefit of doing so in compile time is the compiler being able to
inline the whole expression and perform aggressive optimizations (especially
with `-ffast-math`). The difference between the performance of the generated
code and hand-written derivatives expressions is statistically insignificant.

Compile-time differentiation is achieved by representing an expression as an
abstract syntax tree-like structure (using recursive templates) and rewriting
this structure using the standard rules taught in differential calculus 101.

The library also supports performing basic expression simplification and tree
rewriting, very basic, though. C++ is not Haskell, and doing pattern matching
via templates specializations is painful.
