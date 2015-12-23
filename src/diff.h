#pragma once

#include "tree.h"

template<char Family, int Index>
constexpr size_t BuildIndex ()
{
	return (Index << CHAR_BIT) + Family;
}

template<typename NodeClass, typename... Args>
struct Node;

template<int N>
struct Node<Number<N>>
{
	template<char FPrime, int IPrime>
	using Derivative_t = Node<Number<0>>;

	static std::string Print ()
	{
		return std::to_string (N);
	}

	template<typename Vec>
	static double Eval (const Vec&)
	{
		return N;
	}
};

template<UnaryFunction UF, typename... ChildArgs>
struct Node<UnaryFunctionWrapper<UF>, Node<ChildArgs...>>
{
	using Child_t = Node<ChildArgs...>;

	template<char FPrime, int IPrime>
	using Derivative_t = Node<Mul,
			typename UnaryFunctionWrapper<UF>::template Derivative_t<Child_t>,
			typename Node<ChildArgs...>::template Derivative_t<FPrime, IPrime>>;

	static std::string Print ()
	{
		return FunctionName (UF) + "(" + Node<ChildArgs...>::Print () + ")";
	}

	template<typename Vec>
	static double Eval (const Vec& values)
	{
		const auto child = Child_t::Eval (values);
		return EvalUnary<UF> (child);
	}
};

template<BinaryFunction BF, typename... FirstChildArgs, typename... SecondChildArgs>
struct Node<BinaryFunctionWrapper<BF>, Node<FirstChildArgs...>, Node<SecondChildArgs...>>
{
	using Left_t = Node<FirstChildArgs...>;
	using Right_t = Node<SecondChildArgs...>;

	template<char FPrime, int IPrime>
	using Derivative_t = typename BinaryFunctionWrapper<BF>::template Derivative_t<FPrime, IPrime, Left_t, Right_t>;

	static std::string Print ()
	{
		const auto& firstArg = Left_t::Print ();
		const auto& secondArg = Right_t::Print ();
		const auto& func = FunctionName (BF);

		return IsInfix (BF) ?
				firstArg + " " + func + " " + secondArg :
				func + "(" + firstArg + ", " + secondArg + ")";
	}

	template<typename Vec>
	static double Eval (const Vec& values)
	{
		const auto left = Left_t::Eval (values);
		const auto right = Right_t::Eval (values);

		return EvalBinary<BF> (left, right);
	}
};

template<char Family, int Index>
struct Node<Variable<Family, Index>>
{
	template<char FPrime, int IPrime>
	using Derivative_t = std::conditional_t<FPrime == Family && IPrime == Index,
			Node<Number<1>>,
			Node<Number<0>>>;

	static std::string Print ()
	{
		return std::string { Family, '_' } + std::to_string (Index);
	}

	template<typename Vec>
	static double Eval (const Vec& values)
	{
		return values [BuildIndex<Family, Index> ()];
	}
};

template<>
struct UnaryFunctionWrapper<UnaryFunction::Sin>
{
	template<typename Child>
	using Derivative_t = Node<Cos, Child>;
};

template<>
struct UnaryFunctionWrapper<UnaryFunction::Cos>
{
	template<typename Child>
	using Derivative_t = Node<Neg, Node<Sin, Child>>;
};

template<>
struct UnaryFunctionWrapper<UnaryFunction::Ln>
{
	template<typename Child>
	using Derivative_t = Node<Div, Node<Number<1>>, Child>;
};

template<>
struct UnaryFunctionWrapper<UnaryFunction::Neg>
{
	template<typename>
	using Derivative_t = Node<Number<-1>>;
};

template<>
struct BinaryFunctionWrapper<BinaryFunction::Add>
{
	template<char Family, int Index, typename Left, typename Right>
	using Derivative_t = Node<Add,
				typename Left::template Derivative_t<Family, Index>,
				typename Right::template Derivative_t<Family, Index>
			>;
};

template<>
struct BinaryFunctionWrapper<BinaryFunction::Mul>
{
	template<char Family, int Index, typename Left, typename Right>
	using Derivative_t = Node<Add,
				Node<Mul,
					typename Left::template Derivative_t<Family, Index>,
					Right
				>,
				Node<Mul,
					Left,
					typename Right::template Derivative_t<Family, Index>
				>
			>;
};

template<>
struct BinaryFunctionWrapper<BinaryFunction::Div>
{
	template<char Family, int Index, typename U, typename V>
	using Derivative_t = Node<Div,
				Node<Add,
					Node<Mul,
						typename U::template Derivative_t<Family, Index>,
						V
					>,
					Node<Neg,
						Node<Mul,
							U,
							typename V::template Derivative_t<Family, Index>
						>
					>
				>,
				Node<Mul,
					V,
					V
				>
			>;
};
