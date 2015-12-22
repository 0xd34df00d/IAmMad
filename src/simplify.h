#pragma once

#include <type_traits>
#include "diff.h"

namespace Simplify
{
	template<typename T>
	struct Simplify
	{
		using Result_t = T;
	};

	template<typename T>
	using Simplify_t = typename Simplify<T>::Result_t;

	template<>
	struct Simplify<Node<Mul, Node<Number<1>>, Node<Number<1>>>>
	{
		using Result_t = Node<Number<1>>;
	};

	template<typename Other>
	struct Simplify<Node<Mul, Other, Node<Number<1>>>>
	{
		using Result_t = Simplify_t<Other>;
	};

	template<typename Other>
	struct Simplify<Node<Mul, Node<Number<1>>, Other>>
	{
		using Result_t = Simplify_t<Other>;
	};

	template<>
	struct Simplify<Node<Mul, Node<Number<0>>, Node<Number<0>>>>
	{
		using Result_t = Node<Number<0>>;
	};

	template<typename Other>
	struct Simplify<Node<Mul, Other, Node<Number<0>>>>
	{
		using Result_t = Node<Number<0>>;
	};

	template<typename Other>
	struct Simplify<Node<Mul, Node<Number<0>>, Other>>
	{
		using Result_t = Node<Number<0>>;
	};

	template<typename Other>
	struct Simplify<Node<Add, Other, Node<Number<0>>>>
	{
		using Result_t = Other;
	};

	template<typename Other>
	struct Simplify<Node<Add, Node<Number<0>>, Other>>
	{
		using Result_t = Other;
	};

	template<UnaryFunction UF, typename Child>
	struct Simplify<Node<UnaryFunctionWrapper<UF>, Child>>
	{
		using Result_t = Node<UnaryFunctionWrapper<UF>, Simplify_t<Child>>;
	};

	template<template<typename> class F>
	struct LazyApply
	{
		template<typename T>
		using Result_t = typename F<T>::Result_t;
	};

	template<typename T>
	struct Id
	{
		using Result_t = T;
	};

	template<BinaryFunction BF, typename Left, typename Right>
	struct Simplify<Node<BinaryFunctionWrapper<BF>, Left, Right>>
	{
		using LeftPrime_t = Simplify_t<Left>;
		using RightPrime_t = Simplify_t<Right>;

		static constexpr bool IsFixedPoint = std::is_same<LeftPrime_t, Left>::value && std::is_same<RightPrime_t, Right>::value;

		using Result_t = typename std::conditional_t<
				IsFixedPoint,
				LazyApply<Id>,
				LazyApply<::Simplify::Simplify>
			>::template Result_t<Node<BinaryFunctionWrapper<BF>, LeftPrime_t, RightPrime_t>>;
	};
}
