#pragma once

#include "diff.h"

namespace Parse
{
	template<char Family, int Index>
	Node<Variable<Family, Index>> Var {};

	Node<Variable<'x', 0>> x0;
	Node<Variable<'x', 1>> x1;
	Node<Variable<'x', 2>> x2;

	Node<Variable<'y', 0>> y0;
	Node<Variable<'y', 1>> y1;
	Node<Variable<'y', 2>> y2;

	Node<Variable<'w', 0>> w0;
	Node<Variable<'w', 1>> w1;
	Node<Variable<'w', 2>> w2;

	Node<Number<1>> _1;

	template<typename T1, typename T2>
	Node<Add, T1, T2> operator+ (T1, T2);

	template<typename T1, typename T2>
	Node<Mul, T1, T2> operator* (T1, T2);

	template<typename T1, typename T2>
	Node<Div, T1, T2> operator/ (T1, T2);

	template<typename T>
	Node<Neg, T> operator- (T);

	template<typename T>
	Node<Sin, T> Sin (T);

	template<typename T>
	Node<Cos, T> Cos (T);

	template<typename T>
	Node<Ln, T> Ln (T);

	void TestParses ()
	{
		std::cout << decltype (x0 * y0)::Derivative_t<'x', 0>::Print () << std::endl;
		std::cout << decltype (Sin (x0))::Derivative_t<'x', 0>::Print () << std::endl;
		std::cout << decltype (Ln (x0))::Derivative_t<'x', 0>::Print () << std::endl;
		std::cout << decltype (_1 / x0)::Derivative_t<'x', 0>::Print () << std::endl;
		std::cout << decltype (Sin (x0) * Cos (y0))::Derivative_t<'x', 0>::Print () << std::endl;
	}
}
