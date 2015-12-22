#include <type_traits>
#include <string>
#include <iostream>
#include <climits>
#include <cmath>
#include <array>

template<typename NodeClass, typename... Args>
struct Node;

template<int N>
struct Number {};

template<char Family, int Index>
struct Variable {};

template<char Family, int Index>
constexpr size_t BuildIndex ()
{
	return (Index << CHAR_BIT) + Family;
}

enum class UnaryFunction
{
	Sin,
	Cos,
	Neg
};

std::string FunctionName (UnaryFunction uf)
{
	switch (uf)
	{
		case UnaryFunction::Sin:
			return "sin";
		case UnaryFunction::Cos:
			return "cos";
		case UnaryFunction::Neg:
			return "-";
	}
}

template<UnaryFunction>
double EvalUnary (double);

template<>
double EvalUnary<UnaryFunction::Sin> (double value)
{
	return std::sin (value);
}

template<>
double EvalUnary<UnaryFunction::Cos> (double value)
{
	return std::cos (value);
}

template<>
double EvalUnary<UnaryFunction::Neg> (double value)
{
	return -value;
}

template<UnaryFunction UF>
struct UnaryFunctionWrapper;

using Sin = UnaryFunctionWrapper<UnaryFunction::Sin>;
using Cos = UnaryFunctionWrapper<UnaryFunction::Cos>;
using Neg = UnaryFunctionWrapper<UnaryFunction::Neg>;

enum class BinaryFunction
{
	Add,
	Mul
};

constexpr bool IsInfix (BinaryFunction bf)
{
	switch (bf)
	{
		default:
			return true;
	}
}

std::string FunctionName (BinaryFunction bf)
{
	switch (bf)
	{
		case BinaryFunction::Add:
			return "+";
		case BinaryFunction::Mul:
			return "*";
	}
}

template<BinaryFunction>
double EvalBinary (double, double);

template<>
double EvalBinary<BinaryFunction::Add> (double left, double right)
{
	return left + right;
}

template<>
double EvalBinary<BinaryFunction::Mul> (double left, double right)
{
	return left * right;
}

template<BinaryFunction BF>
struct BinaryFunctionWrapper;

using Add = BinaryFunctionWrapper<BinaryFunction::Add>;
using Mul = BinaryFunctionWrapper<BinaryFunction::Mul>;

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

