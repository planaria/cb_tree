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
		static const std::size_t num_bits = sizeof(char_type) * CHAR_BIT;
		static const std::size_t index_mask = num_bits - 1;
		static const std::size_t index_shift = detail::const_log2<num_bits>::value;

		static std::size_t size(const string_type& s)
		{
			return s.size() * num_bits * 2 + 1;
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
				return (c & (static_cast<unsigned_char_type>(1) << static_cast<unsigned_char_type>(num_bits - r - 1))) != 0;
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
			std::size_t mismatch_bit_index = mismatch_index * num_bits * 2;

			if (mismatch_index == min_size)
			{
				if (s1.size() == s2.size())
					++mismatch_bit_index;
			}
			else
			{
				unsigned_char_type x = data1[mismatch_index];
				x ^= data2[mismatch_index];
				mismatch_bit_index += (num_bits - detail::msb(x)) * 2 - 1;
			}

			return mismatch_bit_index;
		}

	};

	template <class T>
	struct cb_key_traits<T, typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value>::type>
	{

		typedef T value_type;

		static const std::size_t num_bits = sizeof(value_type) * CHAR_BIT;

		static std::size_t size(value_type /*value*/)
		{
			return num_bits;
		}

		static bool test(value_type value, std::size_t index)
		{
			assert(index < size(value));
			return (value & (static_cast<value_type>(1) << static_cast<value_type>(num_bits - index - 1))) != 0;
		}

		static bool equal(value_type value1, value_type value2)
		{
			return value1 == value2;
		}

		static std::size_t mismatch(value_type value1, value_type value2)
		{
			value_type x = value1;
			x ^= value2;
			return num_bits - detail::msb(x) - 1;
		}

	};

	template <class T>
	struct cb_key_traits<T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
	{

		typedef T value_type;

		typedef std::make_unsigned_t<value_type> unsigned_value_type;
		static const std::size_t num_bits = sizeof(value_type) * CHAR_BIT;
		static const unsigned_value_type signbit = static_cast<unsigned_value_type>(1) << (num_bits - 1);

		static std::size_t size(value_type /*value*/)
		{
			return num_bits;
		}

		static bool test(value_type value, std::size_t index)
		{
			assert(index < size(value));
			unsigned_value_type unsigned_value = static_cast<unsigned_value_type>(value);
			unsigned_value ^= signbit;
			return (unsigned_value & (static_cast<unsigned_value_type>(1) << static_cast<unsigned_value_type>(num_bits - index - 1))) != 0;
		}

		static bool equal(value_type value1, value_type value2)
		{
			return value1 == value2;
		}

		static std::size_t mismatch(value_type value1, value_type value2)
		{
			unsigned_value_type x = static_cast<unsigned_value_type>(value1);
			x ^= static_cast<unsigned_value_type>(value2);
			return num_bits - detail::msb(x) - 1;
		}

	};

	template <>
	struct cb_key_traits<float>
	{

		typedef float value_type;
		typedef std::uint32_t unsigned_value_type;
		static const std::size_t num_bits = sizeof(unsigned_value_type) * CHAR_BIT;

		static std::size_t size(value_type /*value*/)
		{
			return num_bits;
		}

		static bool test(value_type value, std::size_t index)
		{
			assert(index < size(value));
			unsigned_value_type unsigned_value = reinterpret_cast<unsigned_value_type&>(value);
			if (unsigned_value & 0x80000000)
				unsigned_value = ~unsigned_value;
			else
				unsigned_value ^= 0x80000000;
			return (unsigned_value & (static_cast<unsigned_value_type>(1) << static_cast<unsigned_value_type>(num_bits - index - 1))) != 0;
		}

		static bool equal(value_type value1, value_type value2)
		{
			return value1 == value2;
		}

		static std::size_t mismatch(value_type value1, value_type value2)
		{
			unsigned_value_type x = reinterpret_cast<unsigned_value_type&>(value1);
			x ^= reinterpret_cast<unsigned_value_type&>(value2);
			return num_bits - detail::msb(x) - 1;
		}

	};

	template <>
	struct cb_key_traits<double>
	{

		typedef double value_type;
		typedef std::uint64_t unsigned_value_type;
		static const std::size_t num_bits = sizeof(unsigned_value_type) * CHAR_BIT;

		static std::size_t size(value_type /*value*/)
		{
			return num_bits;
		}

		static bool test(value_type value, std::size_t index)
		{
			assert(index < size(value));
			unsigned_value_type unsigned_value = reinterpret_cast<unsigned_value_type&>(value);
			if (unsigned_value & 0x8000000000000000)
				unsigned_value = ~unsigned_value;
			else
				unsigned_value ^= 0x8000000000000000;
			return (unsigned_value & (static_cast<unsigned_value_type>(1) << static_cast<unsigned_value_type>(num_bits - index - 1))) != 0;
		}

		static bool equal(value_type value1, value_type value2)
		{
			return value1 == value2;
		}

		static std::size_t mismatch(value_type value1, value_type value2)
		{
			unsigned_value_type x = reinterpret_cast<unsigned_value_type&>(value1);
			x ^= reinterpret_cast<unsigned_value_type&>(value2);
			return num_bits - detail::msb(x) - 1;
		}

	};

	template <class T>
	struct cb_key_traits<T*>
	{

		typedef T* value_type;
		typedef std::size_t unsigned_value_type;
		static const std::size_t num_bits = sizeof(unsigned_value_type) * CHAR_BIT;

		static std::size_t size(value_type /*value*/)
		{
			return num_bits;
		}

		static bool test(value_type value, std::size_t index)
		{
			assert(index < size(value));
			unsigned_value_type unsigned_value = reinterpret_cast<unsigned_value_type&>(value);
			return (unsigned_value & (static_cast<unsigned_value_type>(1) << static_cast<unsigned_value_type>(num_bits - index - 1))) != 0;
		}

		static bool equal(value_type value1, value_type value2)
		{
			return value1 == value2;
		}

		static std::size_t mismatch(value_type value1, value_type value2)
		{
			unsigned_value_type x = reinterpret_cast<unsigned_value_type&>(value1);
			x ^= reinterpret_cast<unsigned_value_type&>(value2);
			return num_bits - detail::msb(x) - 1;
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
		static const std::size_t num_bits = sizeof(char_type) * CHAR_BIT;
		static const std::size_t index_mask = num_bits - 1;
		static const std::size_t index_shift = detail::const_log2<num_bits>::value;

		static std::size_t size(const string_type& s)
		{
			return s.size() * num_bits;
		}

		static bool test(const string_type& s, std::size_t index)
		{
			assert(index < size(s));
			std::size_t q = index >> index_shift;
			std::size_t r = index & index_mask;
			unsigned_char_type c = static_cast<unsigned_char_type>(s[q]);
			return (c & (static_cast<unsigned_char_type>(1) << static_cast<unsigned_char_type>(num_bits - r - 1))) != 0;
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
			std::size_t mismatch_bit_index = mismatch_index * num_bits;

			if (mismatch_index != min_size)
			{
				unsigned_char_type x = data1[mismatch_index];
				x ^= data2[mismatch_index];
				mismatch_bit_index += num_bits - detail::msb(x) - 1;
			}

			return mismatch_bit_index;
		}

	};

}
