#pragma once

namespace cb_tree
{
	namespace detail
	{

		template <class Pointer>
		class cb_tree_end_node;

		template <class Value, class Allocator>
		class cb_node;

		template <class Value, class Allocator>
		struct cb_types
		{

			typedef Value value_type;
			typedef Allocator allocator_type;

			typedef cb_node<value_type, allocator_type> node_type;

			typedef typename std::allocator_traits<allocator_type>::template rebind_alloc<node_type> node_allocator_type;
			typedef std::allocator_traits<node_allocator_type> node_allocator_traits;
			typedef typename node_allocator_traits::pointer node_pointer;
			typedef typename node_allocator_traits::const_pointer node_const_pointer;

			typedef cb_tree_end_node<node_pointer> end_node_type;

		};

	}
}
