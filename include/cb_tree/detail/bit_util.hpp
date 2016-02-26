#pragma once

namespace cb_tree
{
	namespace detail
	{

		inline int count_bit(std::uint8_t v)
		{
			std::uint8_t result;
			result = (v & 0x55) + ((v >> 1) & 0x55);
			result = (result & 0x33) + ((result >> 2) & 0x33);
			result = (result & 0x0f) + ((result >> 4) & 0x0f);
			return static_cast<int>(result);
		}

		inline int count_bit(std::uint16_t v)
		{
			std::uint16_t result;
			result = (v & 0x5555) + ((v >> 1) & 0x5555);
			result = (result & 0x3333) + ((result >> 2) & 0x3333);
			result = (result & 0x0f0f) + ((result >> 4) & 0x0f0f);
			result = (result & 0x00ff) + ((result >> 8) & 0x00ff);
			return static_cast<int>(result);
		}

		inline int count_bit(std::uint32_t v)
		{
			std::uint32_t result;
			result = (v & 0x55555555) + ((v >> 1) & 0x55555555);
			result = (result & 0x33333333) + ((result >> 2) & 0x33333333);
			result = (result & 0x0f0f0f0f) + ((result >> 4) & 0x0f0f0f0f);
			result = (result & 0x00ff00ff) + ((result >> 8) & 0x00ff00ff);
			result = (result & 0x0000ffff) + ((result >> 16) & 0x0000ffff);
			return static_cast<int>(result);
		}

		inline int count_bit(std::uint64_t v)
		{
			std::uint64_t result;
			result = (v & 0x5555555555555555) + ((v >> 1) & 0x5555555555555555);
			result = (result & 0x3333333333333333) + ((result >> 2) & 0x3333333333333333);
			result = (result & 0x0f0f0f0f0f0f0f0f) + ((result >> 4) & 0x0f0f0f0f0f0f0f0f);
			result = (result & 0x00ff00ff00ff00ff) + ((result >> 8) & 0x00ff00ff00ff00ff);
			result = (result & 0x0000ffff0000ffff) + ((result >> 16) & 0x0000ffff0000ffff);
			result = (result & 0x00000000ffffffff) + ((result >> 32) & 0x00000000ffffffff);
			return static_cast<int>(result);
		}

		inline int msb(std::uint8_t v)
		{
			v |= (v >> 1);
			v |= (v >> 2);
			v |= (v >> 4);

			return count_bit(v) - 1;
		}

		inline int msb(std::uint16_t v)
		{
			v |= (v >> 1);
			v |= (v >> 2);
			v |= (v >> 4);
			v |= (v >> 8);

			return count_bit(v) - 1;
		}

		inline int msb(std::uint32_t v)
		{
			v |= (v >> 1);
			v |= (v >> 2);
			v |= (v >> 4);
			v |= (v >> 8);
			v |= (v >> 16);

			return count_bit(v) - 1;
		}

		inline int msb(std::uint64_t v)
		{
			v |= (v >> 1);
			v |= (v >> 2);
			v |= (v >> 4);
			v |= (v >> 8);
			v |= (v >> 16);
			v |= (v >> 32);

			return count_bit(v) - 1;
		}

		inline int lsb(std::uint8_t v)
		{
			v |= (v << 1);
			v |= (v << 2);
			v |= (v << 4);

			return 8 - count_bit(v);
		}

		inline int lsb(std::uint16_t v)
		{
			v |= (v << 1);
			v |= (v << 2);
			v |= (v << 4);
			v |= (v << 8);

			return 16 - count_bit(v);
		}

		inline int lsb(std::uint32_t v)
		{
			v |= (v << 1);
			v |= (v << 2);
			v |= (v << 4);
			v |= (v << 8);
			v |= (v << 16);

			return 32 - count_bit(v);
		}

		inline int lsb(std::uint64_t v)
		{
			v |= (v << 1);
			v |= (v << 2);
			v |= (v << 4);
			v |= (v << 8);
			v |= (v << 16);
			v |= (v << 32);

			return 64 - count_bit(v);
		}

	}
}
