#include <type_traits>
#include <string>
#include <iostream>
#include <climits>
#include <cmath>
#include <array>
#include "tree.h"
#include "diff.h"
#include "parse.h"
#include "params.h"

int main ()
{
	using namespace Parse;

	using Formula_t = decltype (Sqrt (x0));
	using D_t = Simplify::Simplify_t<VarDerivative_t<Formula_t, decltype (x0)>>;
}

