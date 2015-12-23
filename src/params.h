#pragma once

#include "diff.h"

namespace Params
{
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

			using value_type = ValueType;
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
