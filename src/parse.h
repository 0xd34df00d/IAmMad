#pragma once

#include "diff.h"
#include "simplify.h"

namespace Parse
{
	template<char Family, int Index = 0>
	constexpr Node<Variable<Family, Index>> Var {};

	using X0 = Node<Variable<'x', 0>>;
	using X1 = Node<Variable<'x', 1>>;
	using X2 = Node<Variable<'x', 2>>;
	using X3 = Node<Variable<'x', 3>>;

	using Y0 = Node<Variable<'y', 0>>;
	using Y1 = Node<Variable<'y', 1>>;
	using Y2 = Node<Variable<'y', 2>>;
	using Y3 = Node<Variable<'y', 3>>;

	using W0 = Node<Variable<'w', 0>>;
	using W1 = Node<Variable<'w', 1>>;
	using W2 = Node<Variable<'w', 2>>;
	using W3 = Node<Variable<'w', 3>>;

	constexpr X0 x0;
	constexpr X1 x1;
	constexpr X2 x2;
	constexpr X3 x3;

	constexpr Y0 y0;
	constexpr Y1 y1;
	constexpr Y2 y2;
	constexpr Y3 y3;

	constexpr W0 w0;
	constexpr W1 w1;
	constexpr W2 w2;
	constexpr W3 w3;

	constexpr Node<Number<1>> _1;
	constexpr Node<Number<2>> _2;

	template<NumberType_t N>
	Node<Number<N>> Num {};

	template<typename T1, typename T2>
	Node<Add, std::decay_t<T1>, std::decay_t<T2>> operator+ (T1, T2);

	template<typename T1, typename T2>
	Node<Mul, std::decay_t<T1>, std::decay_t<T2>> operator* (T1, T2);

	template<typename T1, typename T2>
	Node<Div, std::decay_t<T1>, std::decay_t<T2>> operator/ (T1, T2);

	template<typename T1, typename T2>
	Node<Add, std::decay_t<T1>, Node<Neg, std::decay_t<T2>>> operator- (T1, T2);

	template<typename T>
	Node<Neg, std::decay_t<T>> operator- (T);

	template<typename T>
	Node<Sin, std::decay_t<T>> Sin (T);

	template<typename T>
	Node<Cos, std::decay_t<T>> Cos (T);

	template<typename T>
	Node<Ln, std::decay_t<T>> Ln (T);

	template<typename T>
	Node<Pow, std::decay_t<T>, Node<Div, Node<Number<1>>, Node<Number<2>>>> Sqrt (T);

	template<typename T>
	Node<Mul, std::decay_t<T>, std::decay_t<T>> operator^ (T, Node<Number<2>>);
}
