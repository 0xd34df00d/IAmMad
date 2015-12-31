#pragma once

#include <type_traits>
#include "diff.h"

namespace Simplify
{
	template<typename T, typename Enable = void>
	struct Simplify
	{
		using Result_t = T;
	};

	template<typename T>
	using SimplifyPrime = Simplify<T>;

	template<typename T>
	using Simplify_t = typename Simplify<T>::Result_t;

	template<typename Other>
	using NotANum_t = std::enable_if_t<!IsNumberNode<Other>::value>;

	template<typename Other>
	struct Simplify<Node<Mul, Other, Node<Number<1>>>, NotANum_t<Other>>
	{
		using Result_t = Simplify_t<Other>;
	};

	template<typename Other>
	struct Simplify<Node<Mul, Node<Number<1>>, Other>, NotANum_t<Other>>
	{
		using Result_t = Simplify_t<Other>;
	};

	template<typename Other>
	struct Simplify<Node<Mul, Other, Node<Number<0>>>, NotANum_t<Other>>
	{
		using Result_t = Node<Number<0>>;
	};

	template<typename Other>
	struct Simplify<Node<Mul, Node<Number<0>>, Other>, NotANum_t<Other>>
	{
		using Result_t = Node<Number<0>>;
	};

	template<int N, int M>
	struct Simplify<Node<Mul, Node<Number<N>>, Node<Number<M>>>>
	{
		using Result_t = Node<Number<N * M>>;
	};

	template<typename Other>
	struct Simplify<Node<Mul, Other, Node<Number<-1>>>, NotANum_t<Other>>
	{
		using Result_t = Node<Neg, Other>;
	};

	template<typename Other>
	struct Simplify<Node<Mul, Node<Number<-1>>, Other>, NotANum_t<Other>>
	{
		using Result_t = Node<Neg, Other>;
	};

	template<typename Other>
	struct Simplify<Node<Div, Node<Number<0>>, Other>>
	{
		using Result_t = Node<Number<0>>;
	};

	template<>
	struct Simplify<Node<Add, Node<Number<0>>, Node<Number<0>>>>
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

	template<int N>
	struct Simplify<Node<Neg, Node<Number<N>>>>
	{
		using Result_t = Node<Number<-N>>;
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

	namespace detail
	{
		template<typename T, int... Numbers>
		struct IsNumberValue : std::false_type {};

		template<int N, int... Rest>
		struct IsNumberValue<Node<Number<N>>, N, Rest...> : std::true_type {};

		template<int K, int N, int... Rest>
		struct IsNumberValue<Node<Number<K>>, N, Rest...> : IsNumberValue<Node<Number<K>>, Rest...> {};

		template<int K>
		struct IsNumberValue<Node<Number<K>>> : std::false_type {};
	}

	template<BinaryFunction BF, typename Left, typename Right>
	struct Simplify<Node<BinaryFunctionWrapper<BF>, Left, Right>,
			std::enable_if_t<
					(!IsNumberNode<Left>::value && !detail::IsNumberValue<Right, -1, 0, 1>::value) ||
					(!IsNumberNode<Right>::value && !detail::IsNumberValue<Left, -1, 0, 1>::value)
				>>
	{
		using LeftPrime_t = Simplify_t<Left>;
		using RightPrime_t = Simplify_t<Right>;

		static constexpr bool IsFixedPoint = std::is_same<LeftPrime_t, Left>::value && std::is_same<RightPrime_t, Right>::value;

		using Result_t = typename std::conditional_t<
				IsFixedPoint,
				LazyApply<Id>,
				LazyApply<SimplifyPrime>
			>::template Result_t<Node<BinaryFunctionWrapper<BF>, LeftPrime_t, RightPrime_t>>;
	};
}
