#pragma once

template<int N>
struct Number {};

template<char Family, int Index>
struct Variable {};


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
