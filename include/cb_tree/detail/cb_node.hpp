#pragma once
#include "cb_types.hpp"

namespace cb_tree
{
	namespace detail
	{

		template <class Pointer>
		class cb_tree_end_node
		{
		public:

			Pointer& left()
			{
				return left_;
			}

			Pointer left() const
			{
				return left_;
			}

			bool leaf()
			{
				return leaf_;
			}

		protected:

			Pointer left_;
			bool leaf_ = true;

		};

		template <class Value, class Allocator>
		class cb_node : public cb_tree_end_node<typename cb_types<Value, Allocator>::node_pointer>
		{
		private:

			typedef cb_types<Value, Allocator> types;

		public:

			typedef typename types::value_type value_type;
			typedef typename types::allocator_type allocator_type;
			typedef typename types::node_allocator_type node_allocator_type;
			typedef typename types::node_allocator_traits node_allocator_traits;
			typedef typename types::node_pointer node_pointer;
			typedef typename types::node_const_pointer node_const_pointer;

			static node_pointer construct(node_allocator_type& allocator)
			{
				node_pointer p = node_allocator_traits::allocate(allocator, 1);
				node_allocator_traits::construct(allocator, std::addressof(p->left_), nullptr);
				node_allocator_traits::construct(allocator, std::addressof(p->leaf_), false);
				node_allocator_traits::construct(allocator, std::addressof(p->right_), nullptr);
				node_allocator_traits::construct(allocator, std::addressof(p->parent_), nullptr);
				node_allocator_traits::construct(allocator, std::addressof(p->index_), 0);
				return p;
			}

			template <class Value_>
			static node_pointer construct(node_allocator_type& allocator, Value_&& value)
			{
				node_pointer p = node_allocator_traits::allocate(allocator, 1);
				node_allocator_traits::construct(allocator, std::addressof(p->left_), nullptr);
				node_allocator_traits::construct(allocator, std::addressof(p->leaf_), true);
				node_allocator_traits::construct(allocator, std::addressof(p->right_), nullptr);
				node_allocator_traits::construct(allocator, std::addressof(p->parent_), nullptr);
				node_allocator_traits::construct(allocator, std::addressof(p->index_), 0);
				node_allocator_traits::construct(allocator, std::addressof(p->value_), std::forward<Value_>(value));
				return p;
			}

			static void destroy(node_allocator_type& allocator, node_pointer p)
			{
				if (p->leaf())
					node_allocator_traits::destroy(allocator, std::addressof(p->value_));
				node_allocator_traits::destroy(allocator, std::addressof(p->left_));
				node_allocator_traits::destroy(allocator, std::addressof(p->leaf_));
				node_allocator_traits::destroy(allocator, std::addressof(p->right_));
				node_allocator_traits::destroy(allocator, std::addressof(p->parent_));
				node_allocator_traits::destroy(allocator, std::addressof(p->index_));
				node_allocator_traits::deallocate(allocator, p, 1);
			}

			node_pointer& right()
			{
				return right_;
			}

			node_pointer right() const
			{
				return right_;
			}

			node_pointer& parent()
			{
				return parent_;
			}

			node_pointer parent() const
			{
				return parent_;
			}

			std::size_t& index()
			{
				return index_;
			}

			std::size_t index() const
			{
				return index_;
			}

			value_type& value()
			{
				assert(this->leaf_);
				return value_;
			}

			const value_type& value() const
			{
				assert(this->leaf_);
				return value_;
			}

			template <class Value_>
			void construct_value(node_allocator_type& allocator, Value_&& value)
			{
				assert(!this->leaf_);
				node_allocator_traits::construct(allocator, std::addressof(value_), std::forward<Value_>(value));
				this->leaf_ = true;
			}

			void destroy_value(node_allocator_type& allocator)
			{
				assert(this->leaf_);
				node_allocator_traits::destroy(allocator, std::addressof(value_));
				this->leaf_ = false;
			}

			bool is_left_child() const
			{
				return parent_->left_ == std::pointer_traits<node_const_pointer>::pointer_to(*this);
			}

			node_pointer next_leaf() const
			{
				node_pointer node = std::pointer_traits<node_pointer>::pointer_to(const_cast<cb_node&>(*this));

				do
				{
					if (node->right())
						return node->right()->min_leaf();

					while(!node->is_left_child())
						node = node->parent();

					node = node->parent();
				} while (!node->leaf());

				return node;
			}

			node_pointer prev_leaf() const
			{
				node_pointer node = std::pointer_traits<node_pointer>::pointer_to(const_cast<cb_node&>(*this));

				do
				{
					if (node->left())
						return node->left()->max_leaf();

					while(node->is_left_child())
						node = node->parent();

					node = node->parent();
				} while (!node->leaf());

				return node;
			}

			node_pointer any_leaf() const
			{
				node_pointer node = std::pointer_traits<node_pointer>::pointer_to(const_cast<cb_node&>(*this));

				while (!node->leaf())
				{
					if (node->left())
						node = node->left();
					else
						node = node->right();
				}

				return node;
			}

			node_pointer min_leaf() const
			{
				node_pointer node = std::pointer_traits<node_pointer>::pointer_to(const_cast<cb_node&>(*this));

				while (true)
				{
					if (node->left())
						node = node->left();
					else
						break;
				}

				assert(node->leaf());

				return node;
			}

			node_pointer max_leaf() const
			{
				node_pointer node = std::pointer_traits<node_pointer>::pointer_to(const_cast<cb_node&>(*this));

				while (true)
				{
					if (node->right())
						node = node->right();
					else
						break;
				}

				assert(node->leaf());

				return node;
			}

			node_pointer next_branch_leaf() const
			{
				node_pointer node = std::pointer_traits<node_pointer>::pointer_to(const_cast<cb_node&>(*this));

				while (!node->is_left_child())
					node = node->parent();

				node = node->parent();

				if (!node->leaf())
					node = node->next_leaf();

				return node;
			}

		private:

			cb_node() = delete;
			cb_node(const cb_node&) = delete;
			cb_node(cb_node&&) = delete;
			~cb_node() = delete;
			cb_node& operator =(const cb_node&) = delete;
			cb_node& operator =(cb_node&&) = delete;

			node_pointer right_;
			node_pointer parent_;
			std::size_t index_;

			value_type value_;

		};

	}
}
