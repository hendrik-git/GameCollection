/// @file Overload.hpp
/// @brief Implements a template class for using the Overload pattern
/// @details ...
/// @see ...
/// @date 2022.12.21
/// @author Hendrik Poettker

#pragma once

namespace CodeHelper
{
	template<class... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};
}  // namespace CodeHelper