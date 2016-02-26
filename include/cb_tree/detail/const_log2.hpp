#pragma once

namespace cb_tree
{
	namespace detail
	{

		template <std::size_t N>
		struct const_log2
		{
			static const std::size_t value = 1 + const_log2<(N >> 1)>::value;
		};

		template <>
		struct const_log2<1>
		{
			static const std::size_t value = 0;
		};

	}
}
