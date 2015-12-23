#pragma once

#include "diff.h"

namespace Params
{
	template<char Family, int Index, typename Vec, typename T>
	void AddValue (Vec& vec, T value)
	{
		vec [BuildIndex<Family, Index> ()] = value;
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

	namespace detail
	{
		template<typename Vec>
		void BuildValuesImpl (Vec&)
		{
		}

		template<char Family, int Index, typename T, typename Vec, typename... Tail>
		void BuildValuesImpl (Vec& vec, Node<Variable<Family, Index>>, T value, Tail&&... tail)
		{
			AddValue<Family, Index> (vec, value);
			BuildValuesImpl (vec, std::forward<Tail> (tail)...);
		}
	}

	template<typename ArrT, typename FT, typename... Tail>
	auto BuildValues (Tail&&... tail)
	{
		std::array<ArrT, MaxSize<FT>::Value + 1> vec {{ 0 }};
		detail::BuildValuesImpl (vec, std::forward<Tail> (tail)...);
		return vec;
	}

	namespace detail
	{
		template<typename F, typename S>
		struct Map : F, S
		{
			using F::operator();
			using S::operator();

			Map (F f, S s)
			: F { std::forward<F> (f) }
			, S { std::forward<S> (s) }
			{
			}
		};

		template<typename F, typename S>
		Map<F, S> Augment (F&& f, S&& s)
		{
			return { f, s };
		}
	}

	template<typename ValueType>
	auto BuildFunctor ()
	{
		struct
		{
			ValueType operator() () const
			{
				return {};
			}
		} dummy;
		return dummy;
	}

	template<typename ValueType, typename NodeType, typename... Tail>
	auto BuildFunctor (NodeType, ValueType val, Tail&&... tail)
	{
		return detail::Augment ([val] (NodeType) { return val; },
				BuildFunctor<ValueType> (std::forward<Tail> (tail)...));
	}
}
