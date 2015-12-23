#pragma once

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

std::string FunctionName (UnaryFunction uf)
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
double EvalUnary<UnaryFunction::Ln> (double value)
{
	return std::log (value);
}

template<>
double EvalUnary<UnaryFunction::Neg> (double value)
{
	return -value;
}

enum class BinaryFunction
{
	Add,
	Mul,
	Div
};

template<BinaryFunction BF>
struct BinaryFunctionWrapper;

using Add = BinaryFunctionWrapper<BinaryFunction::Add>;
using Mul = BinaryFunctionWrapper<BinaryFunction::Mul>;
using Div = BinaryFunctionWrapper<BinaryFunction::Div>;

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
	case BinaryFunction::Div:
		return "/";
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

template<>
double EvalBinary<BinaryFunction::Div> (double left, double right)
{
	return left / right;
}
