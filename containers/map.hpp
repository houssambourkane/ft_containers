/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourkan <hbourkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 13:45:19 by hbourkan          #+#    #+#             */
/*   Updated: 2022/12/09 15:59:16 by hbourkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
#define MAP_HPP

#include "../iterators/iterator_traits.hpp"
#include "../utility/pair.hpp"
#include "../utility/type_traits.hpp"
#include "../data_structures/AVL.hpp"

namespace ft
{ 
    template < class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator<pair<const Key, T> > >
    class map {
        public:
        /*=============================TYPEDEFS================================*/
            typedef Key key_type;
            typedef T mapped_type;
            typedef pair<const key_type, mapped_type> value_type;
            typedef Compare key_compare;
            typedef Alloc allocator_type;
            typedef typename allocator_type::reference reference;
            typedef typename allocator_type::const_reference const_reference;
            typedef typename allocator_type::pointer pointer;
            typedef typename allocator_type::const_pointer const_pointer;
            typedef typename allocator_type::difference_type difference_type;
            typedef typename allocator_type::size_type size_type;
            typedef AVL<value_type, key_compare> tree;
            typedef typename tree::iterator iterator;
            typedef typename tree::const_iterator const_iterator;
            typedef typename tree::reverse_iterator reverse_iterator;
            typedef typename tree::const_reverse_iterator const_reverse_iterator;
            
            /*=============================COMPARE NESTED CLASS================================*/
            class value_compare : std::binary_function<value_type,value_type,bool> {
                friend class map;
                protected:
                    Compare comp;
                    value_compare (Compare c) : comp(c) {}
                public:
                    bool operator() (const value_type& x, const value_type& y) const {
                        return comp(x.first, y.first);
                }
            };

            /*=============================CONSTRUCTORS================================*/
            explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) :
                m_tree(tree()), m_Alloc(alloc), m_comp(comp), m_size(0) {}
            template <class InputIterator> map
                        (InputIterator first,
                        typename enable_if<
                        std::__is_input_iterator<InputIterator>::value &&
                        !is_integral<InputIterator>::value, InputIterator>::type last,
                        const key_compare& comp = key_compare(),
                        const allocator_type& alloc = allocator_type())
                        : m_tree(tree()), m_Alloc(alloc), m_comp(comp), m_size(0) {
                if (!std::__is_input_iterator<InputIterator>::value)
                    throw InputIteratorException("Not a valid iterator");
                insert(first, last);
            }
            map (const map& x): m_Alloc(x.get_allocator()), m_comp(x.key_comp()), m_size(0) {
                if (!(x.empty()))
                    insert(x.begin(), x.end());
            }
            /*=============================ASSIGNMENT OVERLOAD================================*/
            map& operator= (const map& x) {
                if (this == &x)
                    return *this;
                if (!(empty()))
                    clear();
                if (!(x.empty()))
                    insert(x.begin(), x.end());

                return *this;
            }

            /*=============================DESTRUCTOR================================*/
            ~map() {}
            
            /*=============================ITERATORS================================*/
            iterator begin() {return m_tree.begin();}
            iterator end() {return m_tree.end();}
            const_iterator begin() const {return m_tree.begin();}
            const_iterator end() const {return m_tree.end();}
            reverse_iterator rbegin() {return m_tree.rbegin();}
            reverse_iterator rend() {return m_tree.rend();}
            const_reverse_iterator rbegin() const {return m_tree.rbegin();}
            const_reverse_iterator rend() const {return m_tree.rend();}
            
            /*=============================CAPACITY================================*/
            bool empty() const {return size() == 0;}
            size_type size() const {return m_size;}
            size_type max_size() const {return m_tree.get_allocator().max_size();}
            
            /*=============================ELEMENT ACCESS================================*/
            mapped_type& operator[] (const key_type& k) {
                return (*(insert(ft::make_pair(k, mapped_type())).first)).second;
            }
            
            /*=============================MODIFIERS================================*/
            pair<iterator, bool> insert (const value_type& val) {
                bool b = m_tree.insert_node(val);
                if (b)
                    m_size++;
                return pair<iterator, bool>(iterator(find(val.first)), b);
            }
            iterator insert (iterator position, const value_type& val) {
                (void)position;
                insert(val);
                return find(val.first);
            }
            template <class InputIterator>  void insert (InputIterator first,
                    typename enable_if<
                    std::__is_input_iterator<InputIterator>::value &&
                    !is_integral<InputIterator>::value, InputIterator>::type last) {
                if (!std::__is_input_iterator<InputIterator>::value)
                    throw InputIteratorException("Not a valid iterator");
                for (;first != last; ++first)
                    insert(*first);
            }

            void erase (iterator position) {
                erase(position->first);
            }
            size_type erase (const key_type& k) {
                if (empty())
                    return 0;
                if (!(m_tree.delete_key(ft::make_pair(k, mapped_type()))))
                    return 0;
                m_size--;
                return 1;
            }
            void erase (iterator first, iterator last) {
                iterator temp = first;
                if (!empty())
                {
                    while (first != last)
                    {
                        ++first;
                        if (first == last)
                        {
                            erase(temp);
                            break;
                        }
                        erase(temp);
                        temp = first;
                    }
                }
            }
            void clear() {
                m_tree.clear();
                m_size = 0;
            }

            /*=============================OBSERVERS================================*/
            value_compare value_comp() const {
                return value_compare(key_compare());
            }
            key_compare key_comp() const {return m_comp;}
            
            /*=============================OPERATIONS================================*/
            iterator find (const key_type& k) {
                Node<value_type> * temp = m_tree.find(ft::make_pair(k, mapped_type()));
                if (temp)
                    return temp;
                return end();
            }
            const_iterator find (const key_type& k) const {
                Node<value_type> * temp = m_tree.find(ft::make_pair(k, mapped_type()));
                if (temp)
                    return temp;
                return end();
            }
            size_type count (const key_type& k) const {
                if (find(k) != end())
                    return 1;
                return 0;
            }
            iterator lower_bound (const key_type& k) {
                iterator it = begin();
                iterator it_end = end();
                for (; it != it_end ; ++it)
                    if (!m_comp(it->first, k))
                        break;
                return it;
            }
            const_iterator lower_bound (const key_type& k) const {
                const_iterator it = begin();
                const_iterator it_end = end();
                for (; it != it_end ; ++it)
                    if (!m_comp(it->first, k))
                        break;
                return it;
            }
            iterator upper_bound (const key_type& k) {
                iterator it = begin();
                iterator it_end = end();
                for (; it != it_end ; ++it)
                    if (m_comp(k, it->first))
                        break;
                return it;
            }
            const_iterator upper_bound (const key_type& k) const {
                const_iterator it = begin();
                const_iterator it_end = end();
                for (; it != it_end ; ++it)
                    if (m_comp(k, it->first))
                        break;
                return it;
            }
            pair<iterator, iterator> equal_range (const key_type& k) {
                if (find(k) != end())
                    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
                return pair<iterator, iterator>(lower_bound(k), lower_bound(k));
            }
            pair<const_iterator, const_iterator> equal_range (const key_type& k) const {
                if (find(k) != end())
                    return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
                return pair<const_iterator, const_iterator>(lower_bound(k), lower_bound(k));
            }

            /*=============================SWAP================================*/
            void swap (map& x) {
                if (this == &x)
                    return;
                m_tree.swap(x.m_tree);
                size_type temp_size = x.m_size;
                x.m_size = m_size;
                m_size = temp_size;
            }

            /*=============================ALLOCATOR================================*/
            allocator_type get_allocator() const {return m_Alloc;}
            
            /*=============================COMPARISON OPERATORS OVERLOAD================================*/
            friend bool operator== ( const map& lhs, const map& rhs ) {
                if (lhs.size() != rhs.size())
                    return false;
                const_iterator lhs_it = lhs.begin();
                const_iterator rhs_it = rhs.begin();
                const_iterator lhs_it_last = lhs.end();
                for ( ; lhs_it != lhs_it_last ; ++lhs_it, ++rhs_it)
                    if (*lhs_it != *rhs_it)
                        return false;
                return true;
            }
            friend bool operator!= ( const map& lhs, const map& rhs ) {return !(operator==(lhs, rhs));}
            friend bool operator<  ( const map& lhs, const map& rhs ) {
                const_iterator lhs_it = lhs.begin();
                const_iterator rhs_it = rhs.begin();
                const_iterator lhs_it_last = lhs.end();
                const_iterator rhs_it_last = rhs.end();
                for ( ; lhs_it != lhs_it_last ; ++lhs_it, ++rhs_it)
                {
                    if (rhs_it == rhs_it_last || *lhs_it > *rhs_it)
                        return false;
                    else if (*lhs_it < *rhs_it)
                        return true;
                }
                return (rhs_it != rhs_it_last);
            }
            friend bool operator>  ( const map& lhs, const map& rhs ) {return operator<(rhs, lhs);}
            friend bool operator<= ( const map& lhs, const map& rhs ) {return !(operator>(lhs, rhs));}
            friend bool operator>= ( const map& lhs, const map& rhs ) {return !(operator<(lhs, rhs));}

        private:
            tree m_tree;
            allocator_type m_Alloc;
            key_compare m_comp;
            size_type m_size;
    };

    /*=============================NON-MEMBER SWAP================================*/
    template <class Key, class T, class Compare, class Alloc> 
    void swap (map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y) {
        x.swap(y);
    }
   
}

#endif