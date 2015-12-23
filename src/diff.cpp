#include <type_traits>
#include <string>
#include <iostream>
#include <climits>
#include <cmath>
#include <array>
#include "tree.h"
#include "diff.h"
#include "parse.h"
#include "params.h"

int main ()
{
	{
		using df = Node<Cos,
				Node<Variable<'x', 0>>
			>::Derivative_t<'x', 0>;
		std::cout << df::Print () << std::endl;

		std::array<double, MaxSize<df>::Value + 1> vec {{ 0 }};
		AddValue<'x', 0> (vec, M_PI / 2);

		std::cout << df::Eval (vec) << std::endl;
	}

	std::cout << std::endl;

	{
		using df = Node<Mul,
				Node<Sin,
					Node<Variable<'x', 0>>
				>,
				Node<Cos,
					Node<Variable<'y', 0>>
				>
			>::Derivative_t<'x', 0>;

		std::cout << df::Print () << std::endl;

		std::array<double, MaxSize<df>::Value + 1> vec;
		AddValue<'x', 0> (vec, 1);
		AddValue<'y', 0> (vec, 2);

		std::cout << df::Eval (vec) << std::endl;
	}

	std::cout << std::endl;

	{
		using df = Node<Mul,
				  Node<Variable<'x', 0>>,
				  Node<Variable<'y', 0>>
			  >::Derivative_t<'x', 0>;

		std::array<double, MaxSize<df>::Value + 1> vec {{ 0 }};
		AddValue<'x', 0> (vec, 1);
		AddValue<'y', 0> (vec, 2);
		std::cout << df::Eval (vec) << std::endl;
	}
}

