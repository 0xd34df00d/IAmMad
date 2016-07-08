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
		Map<std::decay_t<F>, std::decay_t<S>> Augment (F&& f, S&& s)
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

	template<typename Functor, typename NodeType>
	auto AugmentFunctor (Functor&& f, NodeType, typename Functor::value_type val)
	{
		return detail::Augment ([val] (NodeType) { return val; }, std::forward<Functor> (f));
	}

	template<typename F1, typename F2>
	auto UniteFunctors (F1&& f1, F2&& f2)
	{
		return detail::Augment (f1, f2);
	}
}
