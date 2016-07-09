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
		Map<std::decay_t<F>, std::decay_t<S>> AugmentTwo (F&& f, S&& s)
		{
			return { f, s };
		}

		template<typename F>
		auto Augment (F&& f)
		{
			return f;
		}

		template<typename F, typename S>
		auto Augment (F&& f, S&& s)
		{
			return AugmentTwo (std::forward<F> (f), std::forward<S> (s));
		}

		template<typename F, typename S, typename... Rest>
		auto Augment (F&& f, S&& s, Rest&&... rest)
		{
			return AugmentImpl (AugmentTwo (std::forward<F> (f), std::forward<S> (s)), std::forward<Rest> (rest)...);
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

	template<typename... Fs>
	auto UniteFunctors (Fs&&... fs)
	{
		return detail::Augment (std::forward<Fs> (fs)...);
	}
}
