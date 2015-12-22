#include <type_traits>
#include <string>
#include <iostream>
#include <climits>
#include <cmath>
#include <array>
#include "tree.h"
#include "diff.h"

template<char Family, int Index, typename Vec>
void AddValue (Vec& vec, double value)
{
	const auto index = BuildIndex<Family, Index> ();
	vec [index] = value;
}

template<typename Tree>
struct MaxSize;

template<char Family, int Index>
struct MaxSize<Node<Variable<Family, Index>>>
{
	static constexpr size_t Value = BuildIndex<Family, Index> ();
};

template<UnaryFunction UF, typename Child>
struct MaxSize<Node<UnaryFunctionWrapper<UF>, Child>>
{
	static constexpr size_t Value = MaxSize<Child>::Value;
};

template<BinaryFunction BF, typename Left, typename Right>
struct MaxSize<Node<BinaryFunctionWrapper<BF>, Left, Right>>
{
	static constexpr size_t Value = MaxSize<Left>::Value > MaxSize<Right>::Value ? MaxSize<Left>::Value : MaxSize<Right>::Value;
};

template<typename... Args>
struct MaxSize<Args...>
{
	static constexpr size_t Value = 0;
};

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

