#pragma once
#include "cb_node.hpp"
#include "cb_iterator.hpp"

namespace cb_tree
{
	namespace detail
	{

		template <class Derived, class Key, class Value, class Traits, class Allocator>
		class cb_tree_base
		{
		private:

			typedef Derived derived_type;

		public:

			typedef Key key_type;
			typedef Value value_type;
			typedef Traits traits_type;
			typedef Allocator allocator_type;

			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef typename std::allocator_traits<allocator_type>::pointer pointer;
			typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;

		private:

			typedef std::allocator_traits<allocator_type> allocator_traits;

			typedef cb_types<value_type, allocator_type> node_types;
			typedef typename node_types::end_node_type end_node_type;
			typedef typename node_types::node_type node_type;
			typedef typename node_types::node_allocator_type node_allocator_type;
			typedef typename node_types::node_allocator_traits node_allocator_traits;
			typedef typename node_types::node_pointer node_pointer;
			typedef typename std::pointer_traits<node_pointer>::template rebind<end_node_type> end_node_pointer;

		public:

			typedef cb_iterator<node_type, value_type, reference, pointer> iterator;
			typedef cb_iterator<node_type, const value_type, const_reference, const_pointer> const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

			typedef typename std::allocator_traits<node_allocator_type>::difference_type difference_type;
			typedef typename std::allocator_traits<node_allocator_type>::size_type size_type;

			cb_tree_base()
			{
				construct_impl();
			}

			explicit cb_tree_base(const allocator_type& allocator)
				: data_(allocator)
			{
				construct_impl();
			}

			template <class Iterator>
			cb_tree_base(Iterator first, Iterator last, const allocator_type& allocator = allocator_type())
				: data_(allocator)
			{
				construct_impl();
				insert_range_impl(first, last);
			}

			cb_tree_base(const cb_tree_base& other)
				: data_(allocator_traits::select_on_container_copy_construction(other.get_allocator()))
			{
				construct_impl();
				copy_impl(other);
			}

			cb_tree_base(const cb_tree_base& other, const allocator_type& allocator)
				: data_(allocator)
			{
				construct_impl();
				copy_impl(other);
			}

			cb_tree_base(cb_tree_base&& other)
				: data_(std::move(other.get_allocator()))
			{
				move_impl(std::move(other));
			}

			cb_tree_base(cb_tree_base&& other, const allocator_type& allocator)
				: data_(allocator)
			{
				move_impl(std::move(other));
			}

			cb_tree_base(std::initializer_list<value_type> values, const allocator_type& allocator = allocator_type())
				: data_(allocator)
			{
				construct_impl();
				insert_range_impl(values.begin(), values.end());
			}

			~cb_tree_base()
			{
				destroy_impl();
			}

			iterator begin()
			{
				return iterator(begin_node());
			}

			const_iterator begin() const
			{
				return cbegin();
			}

			const_iterator cbegin() const
			{
				return const_iterator(begin_node());
			}

			iterator end()
			{
				return iterator(end_node());
			}

			const_iterator end() const
			{
				return cend();
			}

			const_iterator cend() const
			{
				return const_iterator(end_node());
			}

			reverse_iterator rbegin()
			{
				return reverse_iterator(end());
			}

			const_reverse_iterator rbegin() const
			{
				return crbegin();
			}

			const_reverse_iterator crbegin() const
			{
				return const_reverse_iterator(end());
			}

			reverse_iterator rend()
			{
				return reverse_iterator(begin());
			}

			const_reverse_iterator rend() const
			{
				return crend();
			}

			const_reverse_iterator crend() const
			{
				return const_reverse_iterator(begin());
			}

			std::pair<iterator, bool> insert(const value_type& value)
			{
				return insert_impl(value);
			}

			std::pair<iterator, bool> insert(value_type&& value)
			{
				return insert_impl(std::move(value));
			}

			template <class Iterator>
			void insert(Iterator first, Iterator last)
			{
				insert_range_impl(first, last);
			}

			template <class Iterator>
			void assign(Iterator first, Iterator last)
			{
				clear();
				insert_range_impl(first, last);
			}

			iterator find(const key_type& key)
			{
				node_pointer node = find_branch(key);
				if (node == end_node() || !node->leaf())
					return end();

				if (!traits_type::equal(derived().key(node->value()), key))
					return end();

				return iterator(node);
			}

			const_iterator find(const key_type& key) const
			{
				return static_cast<cb_tree_base&>(*this).find(key);
			}

			size_type count(const key_type& key) const
			{
				return find(key) == end() ? 0 : 1;
			}

			iterator lower_bound(const key_type& key)
			{
				size_type size = traits_type::size(key);
				node_pointer branch = find_branch(key, size);
				if (branch == end_node())
					return end();

				node_pointer leaf = branch->any_leaf();
				size_type mismatch = traits_type::mismatch(key, derived().key(leaf->value()));

				while (branch != root_node() && mismatch <= branch->parent()->index())
					branch = branch->parent();

				if (mismatch == branch->index())
				{
					if (size == mismatch)
					{
						if (branch->leaf())
							return iterator(branch);

						return iterator(branch->next_leaf());
					}
					else
					{
						return iterator(branch->min_leaf());
					}
				}

				if (size == mismatch)
					return iterator(branch->next_leaf());

				if (traits_type::test(derived().key(leaf->value()), mismatch))
					return iterator(branch->min_leaf());

				return iterator(branch->next_branch_leaf());
			}

			const_iterator lower_bound(const key_type& key) const
			{
				return static_cast<cb_tree_base&>(*this).lower_bound(key);
			}

			iterator upper_bound(const key_type& key)
			{
				auto it = lower_bound(key);
				if (it == end())
					return it;

				if (!traits_type::equal(derived().key(*it), key))
					return it;

				++it;
				return it;
			}

			const_iterator upper_bound(const key_type& key) const
			{
				return static_cast<cb_tree_base&>(*this).upper_bound(key);
			}

			std::pair<iterator, iterator> equal_range(const key_type& key)
			{
				auto it = lower_bound(key);
				if (it == end())
					return std::make_pair(it, it);

				if (!traits_type::equal(derived().key(*it), key))
					return std::make_pair(it, it);

				return std::make_pair(it, std::next(it));
			}

			std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
			{
				return static_cast<cb_tree_base&>(*this).equal_range(key);
			}

			std::pair<iterator, iterator> longest_match(const key_type& key)
			{
				node_pointer branch = find_branch(key);
				if (branch == end_node())
					return std::make_pair(end(), end());

				node_pointer leaf = branch->any_leaf();
				size_type mismatch = traits_type::mismatch(key, derived().key(leaf->value()));
				mismatch = traits_type::lexical_index(mismatch);

				while (branch != root_node() && mismatch <= branch->parent()->index())
					branch = branch->parent();

				return std::make_pair(iterator(branch->min_leaf()), iterator(branch->next_branch_leaf()));
			}

			std::pair<const_iterator, const_iterator> longest_match(const key_type& key) const
			{
				return static_cast<cb_tree_base&>(*this).longest_match(key);
			}

			std::pair<iterator, iterator> prefix_match(const key_type& key)
			{
				size_type size = traits_type::size(key);
				node_pointer branch = find_branch(key, size);
				if (branch == end_node())
					return std::make_pair(end(), end());

				node_pointer leaf = branch->any_leaf();
				size_type mismatch = traits_type::mismatch(key, derived().key(leaf->value()));
				mismatch = traits_type::lexical_index(mismatch);

				if(mismatch != traits_type::lexical_index(size))
					return std::make_pair(end(), end());

				while (branch != root_node() && mismatch <= branch->parent()->index())
					branch = branch->parent();

				if (mismatch != traits_type::lexical_index(branch->index()))
					return std::make_pair(end(), end());

				return std::make_pair(iterator(branch->min_leaf()), iterator(branch->next_branch_leaf()));
			}

			std::pair<const_iterator, const_iterator> prefix_match(const key_type& key) const
			{
				return static_cast<cb_tree_base&>(*this).prefix_match(key);
			}

			iterator erase(const_iterator it)
			{
				assert(it != end());

				node_pointer node = it.node();
				node_pointer next = node->next_leaf();

				node->destroy_value(node_allocator());
				--size_;

				if (node == begin_node())
					begin_node() = next;

				do
				{
					if (!node->left() && !node->right())
					{
						if (node->is_left_child())
							node->parent()->left() = nullptr;
						else
							node->parent()->right() = nullptr;

						node_pointer parent = node->parent();
						destroy_node(node);
						node = parent;
					}
					else if (!node->left())
					{
						if (node->is_left_child())
							node->parent()->left() = node->right();
						else
							node->parent()->right() = node->right();

						node->right()->parent() = node->parent();

						destroy_node(node);
						break;
					}
					else if (!node->right())
					{
						if (node->is_left_child())
							node->parent()->left() = node->left();
						else
							node->parent()->right() = node->left();

						node->left()->parent() = node->parent();

						destroy_node(node);
						break;
					}
				} while (!node->leaf());

				return iterator(next);
			}

			size_type erase(const key_type& key)
			{
				auto it = find(key);
				if (it == end())
					return 0;

				erase(it);
				return 1;
			}

			iterator erase(const_iterator first, const_iterator last)
			{
				while (first != last)
					first = erase(first);
			}

			void clear()
			{
				destroy_impl();
				construct_impl();
			}

			bool empty() const
			{
				return size_ == 0;
			}

			size_type size() const
			{
				return size_;
			}

			size_type max_size() const
			{
				return node_allocator_traits::max_size(node_allocator());
			}

			void swap(cb_tree_base& other)
			{
				if (&other == this)
					return;

				swap_allocator(other);

				std::swap(begin_node(), other.begin_node());
				std::swap(root_node(), other.root_node());
				std::swap(size_, other.size_);

				fix_swapped();
				other.fix_swapped();
			}

			cb_tree_base& operator =(const cb_tree_base& other)
			{
				if (&other != this)
				{
					cb_tree_base temp;
					temp.copy_assign_allocator(other);
					temp.copy_impl(other);
					swap(temp);
				}

				return *this;
			}

			cb_tree_base& operator =(cb_tree_base&& other)
			{
				if (&other != this)
				{
					destroy_impl();
					move_assign_allocator(other);
					move_impl(std::move(other));
				}

				return *this;
			}

			allocator_type get_allocator() const
			{
				return allocator_type(node_allocator());
			}

		private:

			void construct_impl()
			{
				begin_node() = end_node();
				root_node() = nullptr;
				size_ = 0;
			}

			void copy_impl(const cb_tree_base& other)
			{
				// slow
				insert_range_impl(other.begin(), other.end());
			}

			void move_impl(cb_tree_base&& other)
			{
				begin_node() = other.begin_node();
				root_node() = other.root_node();
				size_ = other.size_;

				other.construct_impl();

				fix_swapped();
			}

			void fix_swapped()
			{
				if (root_node())
					root_node()->parent() = end_node();

				end_node()->left() = root_node();

				if (empty())
					begin_node() = end_node();
			}

			void copy_assign_allocator(const cb_tree_base& other)
			{
				copy_assign_allocator(other, typename node_allocator_traits::propagate_on_container_copy_assignment());
			}

			void copy_assign_allocator(const cb_tree_base& other, std::true_type)
			{
				if (node_allocator() != other.node_allocator())
					node_allocator() = other.node_allocator();
			}

			void copy_assign_allocator(const cb_tree_base& /*other*/, std::false_type)
			{
			}

			void move_assign_allocator(cb_tree_base& other)
			{
				move_assign_allocator(other, typename node_allocator_traits::propagate_on_container_move_assignment());
			}

			void move_assign_allocator(cb_tree_base& other, std::true_type)
			{
				node_allocator() = std::move(other.node_allocator());
			}

			void move_assign_allocator(cb_tree_base& /*other*/, std::false_type)
			{
			}

			void swap_allocator(cb_tree_base& other)
			{
				swap_allocator(other, typename node_allocator_traits::propagate_on_container_swap());
			}

			void swap_allocator(cb_tree_base& other, std::true_type)
			{
				using std::swap;
				swap(node_allocator(), other.node_allocator());
			}

			void swap_allocator(cb_tree_base& /*other*/, std::false_type)
			{
			}

			template <class Value_>
			std::pair<iterator, bool> insert_impl(Value_&& value)
			{
				size_type size = traits_type::size(derived().key(value));

				if (root_node())
				{
					node_pointer branch = find_branch(derived().key(value), size);
					node_pointer leaf = branch->any_leaf();
					size_type mismatch = traits_type::mismatch(derived().key(value), derived().key(leaf->value()));

					while (branch != root_node() && mismatch <= branch->parent()->index())
						branch = branch->parent();

					if (mismatch == branch->index())
					{
						if (size == mismatch)
						{
							if (branch->leaf())
								return std::make_pair(iterator(branch), false);

							branch->construct_value(node_allocator(), std::forward<Value_>(value));
							++size_;

							return std::make_pair(make_new_iterator_to(branch), true);
						}
						else
						{
							assert(size > mismatch);

							node_pointer new_node = construct_node(std::forward<Value_>(value));
							++size_;

							new_node->parent() = branch;
							new_node->index() = size;

							if (traits_type::test(derived().key(new_node->value()), branch->index()))
								branch->right() = new_node;
							else
								branch->left() = new_node;

							return std::make_pair(make_new_iterator_to(new_node), true);
						}
					}
					else if (size == mismatch)
					{
						node_pointer new_node = construct_node(std::forward<Value_>(value));
						++size_;

						new_node->parent() = branch->parent();
						new_node->index() = size;

						if (branch->is_left_child())
							branch->parent()->left() = new_node;
						else
							branch->parent()->right() = new_node;

						branch->parent() = new_node;

						if (traits_type::test(derived().key(leaf->value()), mismatch))
							new_node->right() = branch;
						else
							new_node->left() = branch;

						return std::make_pair(make_new_iterator_to(new_node), true);
					}
					else
					{
						assert(size > mismatch);

						node_pointer branch_node = construct_node();
						node_pointer new_node;

						try
						{
							new_node = construct_node(std::forward<Value_>(value));
						}
						catch (...)
						{
							destroy_node(branch_node);
							throw;
						}

						++size_;

						branch_node->parent() = branch->parent();
						branch_node->index() = mismatch;

						if (branch->is_left_child())
							branch->parent()->left() = branch_node;
						else
							branch->parent()->right() = branch_node;

						branch->parent() = branch_node;

						new_node->parent() = branch_node;
						new_node->index() = size;

						if (traits_type::test(derived().key(leaf->value()), mismatch))
						{
							branch_node->left() = new_node;
							branch_node->right() = branch;
						}
						else
						{
							branch_node->left() = branch;
							branch_node->right() = new_node;
						}

						return std::make_pair(make_new_iterator_to(new_node), true);
					}
				}
				else
				{
					node_pointer new_node = construct_node(std::forward<Value_>(value));
					new_node->parent() = end_node();
					new_node->index() = size;
					begin_node() = new_node;
					root_node() = new_node;
					++size_;
					return std::make_pair(begin(), true);
				}
			}

			node_pointer find_branch(const key_type& key, size_type size) const
			{
				assert(traits_type::size(key) == size);
				node_pointer node = root_node();

				if (!node)
					return end_node();

				while (true)
				{
					if (size <= node->index())
						break;

					if (!node->left() && !node->right())
						break;

					if (traits_type::test(key, node->index()))
					{
						if (!node->right())
							break;

						node = node->right();
					}
					else
					{
						if (!node->left())
							break;

						node = node->left();
					}
				}

				return node;
			}

			node_pointer find_branch(const key_type& key) const
			{
				return find_branch(key, traits_type::size(key));
			}

			iterator make_new_iterator_to(node_pointer node)
			{
				iterator it(node);

				if (std::next(it) == begin())
					begin_node() = node;

				return it;
			}

			template <class Iterator>
			void insert_range_impl(Iterator first, Iterator last)
			{
				while (first != last)
				{
					insert(*first);
					++first;
				}
			}

			void destroy_impl()
			{
				node_pointer node = begin_node();

				while (node != end_node())
				{
					if (node->right())
					{
						node = node->right();
					}
					else if (node->left())
					{
						node = node->left();
					}
					else
					{
						if (node->is_left_child())
						{
							node = node->parent();
							destroy_node(node->left());
							node->left() = nullptr;
						}
						else
						{
							node = node->parent();
							destroy_node(node->right());
							node->right() = nullptr;
						}
					}
				}
			}

			struct compressed_data : node_allocator_type
			{

				compressed_data()
				{
				}

				explicit compressed_data(const allocator_type& allocator)
					: node_allocator_type(allocator)
				{
				}

				explicit compressed_data(allocator_type&& allocator)
					: node_allocator_type(std::move(allocator))
				{
				}

				node_pointer begin_node;
				end_node_type end_node;

			};

			node_pointer& begin_node()
			{
				return data_.begin_node;
			}

			node_pointer begin_node() const
			{
				return data_.begin_node;
			}

			node_pointer& root_node()
			{
				return data_.end_node.left();
			}

			node_pointer root_node() const
			{
				return data_.end_node.left();
			}

			node_pointer end_node() const
			{
				return static_cast<node_pointer>(
					std::pointer_traits<end_node_pointer>::pointer_to(
						const_cast<end_node_type&>(data_.end_node)));
			}

			node_allocator_type& node_allocator()
			{
				return data_;
			}

			const node_allocator_type& node_allocator() const
			{
				return data_;
			}

			template <class... Args>
			node_pointer construct_node(Args&&... args)
			{
				return node_type::construct(node_allocator(), std::forward<Args>(args)...);
			}

			void destroy_node(node_pointer node)
			{
				node_type::destroy(node_allocator(), node);
			}

			derived_type& derived()
			{
				return static_cast<derived_type&>(*this);
			}

			const derived_type& derived() const
			{
				return static_cast<const derived_type&>(*this);
			}

			friend bool operator ==(const derived_type& lhs, const derived_type& rhs)
			{
				if (&lhs == &rhs)
					return true;

				if (lhs.size() != rhs.size())
					return false;

				return std::equal(lhs.begin(), lhs.end(), rhs.begin());
			}

			friend bool operator !=(const derived_type& lhs, const derived_type& rhs)
			{
				return !(lhs == rhs);
			}

			friend bool operator <(const derived_type& lhs, const derived_type& rhs)
			{
				return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
			}

			friend bool operator <=(const derived_type& lhs, const derived_type& rhs)
			{
				return !(rhs < lhs);
			}

			friend bool operator >(const derived_type& lhs, const derived_type& rhs)
			{
				return rhs < lhs;
			}

			friend bool operator >=(const derived_type& lhs, const derived_type& rhs)
			{
				return !(lhs < rhs);
			}

			compressed_data data_;
			size_type size_;

		};

	}
}
