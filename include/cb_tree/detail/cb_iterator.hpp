#pragma once
#include "cb_node.hpp"

namespace cb_tree
{
	namespace detail
	{

		template <class Derived, class Key, class Value, class Traits, class Allocator>
		class cb_tree_base;

		template <class Node, class Value, class Reference, class Pointer>
		class cb_iterator
			: public std::iterator<
			std::bidirectional_iterator_tag,
			Value,
			std::ptrdiff_t,
			Pointer,
			Reference
			>
		{
		private:

			typedef std::iterator<
				std::bidirectional_iterator_tag,
				Value,
				std::ptrdiff_t,
				Pointer,
				Reference
			> base_type;

			typedef Node node_type;
			typedef typename node_type::node_pointer node_pointer;

			template <class Node_, class Value_, class Reference_, class Pointer_>
			friend class cb_iterator;

			template <class Derived_, class Key_, class Value_, class Traits_, class Allocator_>
			friend class cb_tree_base;

		public:

			using typename base_type::iterator_category;
			using typename base_type::value_type;
			using typename base_type::difference_type;
			using typename base_type::pointer;
			using typename base_type::reference;

			cb_iterator()
			{
			}

			explicit cb_iterator(node_pointer node)
				: node_(node)
			{
				assert(node);
			}

			template <class Value_, class Reference_, class Pointer_>
			cb_iterator(
				cb_iterator<Node, Value_, Reference_, Pointer_> other,
				typename std::enable_if<std::is_convertible<Value_*, Value*>::value>::type* = 0)
				: node_(other.node())
			{
			}

			reference operator *() const
			{
				assert(node_);
				return node_->value();
			}

			pointer operator ->() const
			{
				return std::pointer_traits<pointer>::pointer_to(**this);
			}

			cb_iterator& operator ++()
			{
				assert(node_);
				node_ = node_->next_leaf();
				return *this;
			}

			cb_iterator operator ++(int)
			{
				cb_iterator ret = *this;
				++(*this);
				return ret;
			}

			cb_iterator& operator --()
			{
				assert(node_);
				node_ = node_->prev_leaf();
				return *this;
			}

			cb_iterator operator --(int)
			{
				cb_iterator ret = *this;
				--(*this);
				return ret;
			}

			template <class Value_, class Reference_, class Pointer_>
			bool operator ==(cb_iterator<Node, Value_, Reference_, Pointer_> rhs) const
			{
				assert(node_ && rhs.node_);
				return node_ == rhs.node_;
			}

			template <class Value_, class Reference_, class Pointer_>
			bool operator !=(cb_iterator<Node, Value_, Reference_, Pointer_> rhs) const
			{
				assert(node_ && rhs.node_);
				return node_ != rhs.node_;
			}

		private:

			node_pointer node()
			{
				return node_;
			}

			node_pointer node_ = nullptr;

		};

	}
}
