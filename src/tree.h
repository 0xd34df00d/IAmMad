#pragma once

#include <string>
#include <cmath>

template<int N>
struct Number {};

template<char Family, int Index>
struct Variable {};


enum class UnaryFunction
{
	Sin,
	Cos,
	Ln,
	Neg
};

template<UnaryFunction UF>
struct UnaryFunctionWrapper;

using Sin = UnaryFunctionWrapper<UnaryFunction::Sin>;
using Cos = UnaryFunctionWrapper<UnaryFunction::Cos>;
using Neg = UnaryFunctionWrapper<UnaryFunction::Neg>;
using Ln = UnaryFunctionWrapper<UnaryFunction::Ln>;

inline std::string FunctionName (UnaryFunction uf)
{
	switch (uf)
	{
	case UnaryFunction::Sin:
		return "sin";
	case UnaryFunction::Cos:
		return "cos";
	case UnaryFunction::Ln:
		return "ln";
	case UnaryFunction::Neg:
		return "-";
	}
}

template<typename T>
T EvalUnary (const Sin&, T value)
{
	return std::sin (value);
}

template<typename T>
T EvalUnary (const Cos&, T value)
{
	return std::cos (value);
}

template<typename T>
T EvalUnary (const Ln&, T value)
{
	return std::log (value);
}

template<typename T>
T EvalUnary (const Neg&, T value)
{
	return -value;
}

enum class BinaryFunction
{
	Add,
	Mul,
	Div,
	Pow
};

template<BinaryFunction BF>
struct BinaryFunctionWrapper;

using Add = BinaryFunctionWrapper<BinaryFunction::Add>;
using Mul = BinaryFunctionWrapper<BinaryFunction::Mul>;
using Div = BinaryFunctionWrapper<BinaryFunction::Div>;
using Pow = BinaryFunctionWrapper<BinaryFunction::Pow>;

inline constexpr bool IsInfix (BinaryFunction bf)
{
	switch (bf)
	{
	default:
		return true;
	}
}

inline std::string FunctionName (BinaryFunction bf)
{
	switch (bf)
	{
	case BinaryFunction::Add:
		return "+";
	case BinaryFunction::Mul:
		return "*";
	case BinaryFunction::Div:
		return "/";
	case BinaryFunction::Pow:
		return "^";
	}
}

template<typename T>
T EvalBinary (const Add&, T left, T right)
{
	return left + right;
}

template<typename T>
T EvalBinary (const Mul&, T left, T right)
{
	return left * right;
}

template<typename T>
T EvalBinary (const Div&, T left, T right)
{
	return left / right;
}

template<typename T>
T EvalBinary (const Pow&, T left, T right)
{
	return std::pow (left, right);
}
