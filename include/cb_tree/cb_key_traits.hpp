#pragma once
#include "detail/bit_util.hpp"
#include "detail/const_log2.hpp"

namespace cb_tree
{

	template <class T, class Enable = void>
	struct cb_key_traits;

	template <class Char, class CharTraits, class Allocator>
	struct cb_key_traits<std::basic_string<Char, CharTraits, Allocator>>
	{

		typedef Char char_type;
		typedef CharTraits char_traits_type;
		typedef Allocator allocator_type;
		typedef std::basic_string<char_type, char_traits_type, allocator_type> string_type;

		typedef std::make_unsigned_t<char_type> unsigned_char_type;
		static const std::size_t char_bit = sizeof(char_type) * CHAR_BIT;
		static const std::size_t index_mask = char_bit - 1;
		static const std::size_t index_shift = detail::const_log2<char_bit>::value;

		static std::size_t size(const string_type& s)
		{
			return s.size() * char_bit * 2 + 1;
		}

		static bool test(const string_type& s, std::size_t index)
		{
			assert(index < size(s));
			if (index & 1)
			{
				index >>= 1;
				std::size_t q = index >> index_shift;
				std::size_t r = index & index_mask;
				unsigned_char_type c = static_cast<unsigned_char_type>(s[q]);
				return (c & (static_cast<unsigned_char_type>(1) << static_cast<unsigned_char_type>(char_bit - r - 1))) != 0;
			}
			else
			{
				index >>= 1 + index_shift;
				return index != s.size();
			}
		}

		static bool equal(const string_type& s1, const string_type& s2)
		{
			return s1 == s2;
		}

		static std::size_t mismatch(const string_type& s1, const string_type& s2)
		{
			std::size_t min_size = std::min(s1.size(), s2.size());
			auto data1 = reinterpret_cast<const unsigned_char_type*>(s1.data());
			auto data2 = reinterpret_cast<const unsigned_char_type*>(s2.data());

			std::size_t mismatch_index = std::mismatch(data1, data1 + min_size, data2).first - data1;
			std::size_t mismatch_bit_index = mismatch_index * char_bit * 2;

			if (mismatch_index == min_size)
			{
				if (s1.size() == s2.size())
					++mismatch_bit_index;
			}
			else
			{
				unsigned_char_type x = static_cast<unsigned_char_type>(data1[mismatch_index] ^ data2[mismatch_index]);
				mismatch_bit_index += (char_bit - detail::msb(x)) * 2 - 1;
			}

			return mismatch_bit_index;
		}

	};

	template <class Char = char, class CharTraits = std::char_traits<char>, class Allocator = std::allocator<char>>
	struct cb_raw_string_traits
	{

		typedef Char char_type;
		typedef CharTraits char_traits_type;
		typedef Allocator allocator_type;
		typedef std::basic_string<char_type, char_traits_type, allocator_type> string_type;

		typedef std::make_unsigned_t<char_type> unsigned_char_type;
		static const std::size_t char_bit = sizeof(char_type) * CHAR_BIT;
		static const std::size_t index_mask = char_bit - 1;
		static const std::size_t index_shift = detail::const_log2<char_bit>::value;

		static std::size_t size(const string_type& s)
		{
			return s.size() * char_bit;
		}

		static bool test(const string_type& s, std::size_t index)
		{
			assert(index < size(s));
			std::size_t q = index >> index_shift;
			std::size_t r = index & index_mask;
			unsigned_char_type c = static_cast<unsigned_char_type>(s[q]);
			return (c & (static_cast<unsigned_char_type>(1) << static_cast<unsigned_char_type>(char_bit - r - 1))) != 0;
		}

		static bool equal(const string_type& s1, const string_type& s2)
		{
			return s1 == s2;
		}

		static std::size_t mismatch(const string_type& s1, const string_type& s2)
		{
			std::size_t min_size = std::min(s1.size(), s2.size());
			auto data1 = reinterpret_cast<const unsigned_char_type*>(s1.data());
			auto data2 = reinterpret_cast<const unsigned_char_type*>(s2.data());

			std::size_t mismatch_index = std::mismatch(data1, data1 + min_size, data2).first - data1;
			std::size_t mismatch_bit_index = mismatch_index * char_bit;

			if (mismatch_index != min_size)
			{
				unsigned_char_type x = static_cast<unsigned_char_type>(data1[mismatch_index] ^ data2[mismatch_index]);
				mismatch_bit_index += char_bit - detail::msb(x) - 1;
			}

			return mismatch_bit_index;
		}

	};

}
