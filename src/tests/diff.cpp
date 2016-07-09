#include "tree.h"
#include "diff.h"
#include "parse.h"
#include "params.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <limits>

template<typename Val>
struct Domain
{
	Val From_;
	Val To_;
};

auto GenerateValues ()
{
	return std::vector<std::tuple<>> { std::make_tuple () };
}

template<typename Val, typename... Rest>
auto GenerateValues (Domain<Val>&& dom, Domain<Rest>&&... rest)
{
	std::vector<std::tuple<Val, Rest...>> result;

	const auto& restTuples = GenerateValues (std::move (rest)...);

	const size_t count = 100;
	const auto step = (dom.To_ - dom.From_) / count;
	for (auto val = dom.From_; val < dom.To_; val += step)
		for (const auto& item : restTuples)
			result.push_back (std::tuple_cat (std::make_tuple (val), item));

	return result;
}

template<typename... Vals, size_t... Is>
auto BuildFunctor (std::tuple<Vals...> vals, std::index_sequence<Is...>)
{
	return Params::UniteFunctors (Params::BuildFunctor<Vals> (Parse::Var<'x', Is>, std::get<Is> (vals))...);
}

template<typename... Vals>
auto BuildFunctors (const std::vector<std::tuple<Vals...>>& vecs)
{
	std::index_sequence_for<Vals...> seq;

	std::vector<decltype (BuildFunctor (vecs [0], seq))> result;

	for (const auto& tuple : vecs)
		result.push_back (BuildFunctor (tuple, seq));

	return result;
}

template<typename Der, typename Ref, typename... Domains>
bool CheckEqual (Domains&&... doms)
{
	const auto& vals = BuildFunctors (GenerateValues (std::forward<Domains> (doms)...));

	std::vector<decltype (Der::Eval (vals [0]))> derResults;
	for (const auto& item : vals)
		derResults.push_back (Der::Eval (item));

	std::vector<decltype (Ref::Eval (vals [0]))> refResults;
	for (const auto& item : vals)
		refResults.push_back (Ref::Eval (item));

	bool allGood = true;
	for (size_t i = 0; i < derResults.size (); ++i)
		if (std::abs (derResults [i] - refResults [i]) > 0.0001 * std::min (derResults [i], refResults [i]))
		{
			allGood = false;
			std::cerr << "mismatch: " << derResults [i] << " vs " << refResults [i] << std::endl;
		}

	return allGood;
}

int main ()
{
	using Formula_t = decltype (Parse::Sqrt (Parse::x0));
	using D_t = Simplify::Simplify_t<VarDerivative_t<Formula_t, decltype (Parse::x0)>>;

	using namespace Parse;
	if (CheckEqual<D_t, decltype (Parse::_1 / (Parse::_2 * Parse::Sqrt (Parse::x0)))> (Domain<double> { 1, 11 }))
		return 1;

	return 0;
}

