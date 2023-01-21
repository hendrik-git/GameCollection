/// @file Overload.hpp
/// @brief Implements a template class for using the Overload pattern
/// @details ...
/// @see ...
/// @date 2022.12.21
/// @author Hendrik Poettker

#pragma once

/// @brief Contains functionality that makes writing code easier
namespace CodeHelper
{
	/// @brief The Overload pattern reduces the effort it takes to write a struct for the visitor
	/// pattern, by creating the struct from an initializer-list of lambdas
	template<class... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};
}  // namespace CodeHelper