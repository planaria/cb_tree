#pragma once
#include "detail/cb_tree_base.hpp"
#include "cb_key_traits.hpp"

namespace cb_tree
{

	template <
		class Value,
		class Traits = cb_key_traits<Value>,
		class Allocator = std::allocator<Value>
	>
	class cb_set : private detail::cb_tree_base<cb_set<Value, Traits, Allocator>, Value, Value, Traits, Allocator>
	{
	private:

		typedef detail::cb_tree_base<cb_set<Value, Traits, Allocator>, Value, Value, Traits, Allocator> base_type;
		friend class detail::cb_tree_base<cb_set<Value, Traits, Allocator>, Value, Value, Traits, Allocator>;

	public:

		using typename base_type::key_type;
		using typename base_type::value_type;
		using typename base_type::traits_type;
		using typename base_type::allocator_type;

		using typename base_type::reference;
		using typename base_type::const_reference;
		using typename base_type::pointer;
		using typename base_type::const_pointer;

		using typename base_type::reverse_iterator;
		using typename base_type::const_reverse_iterator;
		using typename base_type::iterator;
		using typename base_type::const_iterator;

		using typename base_type::difference_type;
		using typename base_type::size_type;

		using base_type::base_type;

		using base_type::begin;
		using base_type::cbegin;
		using base_type::end;
		using base_type::cend;
		using base_type::rbegin;
		using base_type::crbegin;
		using base_type::rend;
		using base_type::crend;

		using base_type::insert;
		using base_type::assign;
		using base_type::erase;
		using base_type::clear;

		using base_type::find;
		using base_type::count;
		using base_type::lower_bound;
		using base_type::upper_bound;
		using base_type::equal_range;

		using base_type::empty;
		using base_type::size;
		using base_type::max_size;

		using base_type::swap;

		using base_type::operator =;

		using base_type::get_allocator;

	private:

		const key_type& key(const value_type& value)
		{
			return value;
		}

	};

	template <class Value, class Traits, class Allocator>
	void swap(cb_set<Value, Traits, Allocator>& lhs, cb_set<Value, Traits, Allocator>& rhs)
	{
		lhs.swap(rhs);
	}

}
