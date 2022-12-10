#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "../utility/type_traits.hpp"
#include "../iterators/vector_iterator.hpp"
#include "../utility/algorithms.hpp"
#include <limits>

namespace ft
{
    template <class T, class Allocator = std::allocator<T> >
    class vector {
        public:
            /*=============================TYPEDEFS================================*/
            typedef ft::my_random_access_iterator<T> iterator;
            typedef ft::my_random_access_iterator<const T> const_iterator;
            typedef ft::reverse_iterator<iterator> reverse_iterator;
            typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
            typedef Allocator allocator_type;
            typedef typename allocator_type::value_type value_type;
            typedef typename allocator_type::reference reference;
            typedef typename allocator_type::const_reference const_reference;
            typedef typename allocator_type::pointer pointer;
            typedef typename allocator_type::const_pointer const_pointer;
            typedef typename allocator_type::difference_type difference_type;
            typedef typename allocator_type::size_type size_type;

            /*=============================CONSTRUCTORS================================*/
            explicit vector (const allocator_type& alloc = allocator_type()) : m_ptr(nullptr), m_size(0), m_capacity(0), m_Alloc(alloc) {
            }
            explicit vector (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()): m_ptr(nullptr), m_size(n), m_capacity(n), m_Alloc(alloc) {
                m_ptr = m_Alloc.allocate(n);
                pointer temp = m_ptr;
                for (size_type i(0); i < m_size; i++)
                    m_Alloc.construct(temp++, val);
            }
            template <class InputIterator>
            vector (InputIterator first,
            typename enable_if<std::__is_input_iterator<InputIterator>::value &&
            !is_integral<InputIterator>::value, InputIterator>::type last, const allocator_type& alloc = allocator_type())
            : m_ptr(nullptr), m_size(0), m_capacity(0), m_Alloc(alloc) {
                if (!std::__is_input_iterator<InputIterator>::value)
                    throw InputIteratorException("Not a valid iterator");
                if (std::__is_input_iterator<InputIterator>::value && !std::__is_forward_iterator<InputIterator>::value)
                    for (InputIterator it = first; it != last; ++it)
                        push_back(*it);
                else
                {
                    difference_type dist = ft::distance(first, last);
                    m_size = m_capacity = dist;
                    m_ptr = m_Alloc.allocate(m_size);
                    pointer temp = m_ptr;
                    for (InputIterator it = first ; it != last ; ++it)
                        m_Alloc.construct(temp++, *it);
                }
            }
            vector (const vector& src): m_ptr(nullptr), m_size(src.size()), m_capacity(src.capacity()) {
                m_ptr = m_Alloc.allocate(src.capacity());
                pointer temp = m_ptr;
                for (const_iterator it = src.begin(); it != src.end(); ++it)
                    m_Alloc.construct(temp++, *it);
            }

            /*=============================ASSIGNMENT OVERLOAD================================*/
            vector& operator=(const vector& rhs) {
                if (this == &rhs)
                    return *this;
                clear();
                pointer temp = m_ptr;
                m_ptr = m_Alloc.allocate(rhs.capacity());
                if (temp)
                    m_Alloc.deallocate(temp, m_capacity);
                m_capacity = rhs.capacity();
                temp = m_ptr;
                for (const_iterator it = rhs.begin(); it != rhs.end(); ++it)
                    m_Alloc.construct(temp++, *it);
                m_size = rhs.size();
                return *this;
            }
            
            /*=============================DESTRUCTOR================================*/
            ~vector() {
                clear();
                if (m_ptr)
                    m_Alloc.deallocate(m_ptr, m_capacity);
            }

            /*=============================ITERATORS================================*/
            iterator begin() { return iterator(m_ptr); }
            iterator end() { return iterator(m_ptr + m_size); }
            const_iterator begin() const {return const_iterator(m_ptr);}
            const_iterator end() const {return const_iterator(m_ptr + m_size);}
            reverse_iterator rbegin() {return reverse_iterator(m_ptr + m_size);}
            reverse_iterator rend() {return reverse_iterator(m_ptr);}
            const_reverse_iterator rbegin() const {return const_reverse_iterator(m_ptr + m_size);}
            const_reverse_iterator rend() const {return const_reverse_iterator(m_ptr);}
            
            /*=============================CAPACITY================================*/
            size_type size() const { return m_size; }
            size_type capacity() const { return m_capacity;}
            bool empty() const {return m_size == 0;}
            size_type max_size() const {
                return std::min<size_type>(m_Alloc.max_size(),
                                 std::numeric_limits<difference_type>::max());
            }
            void resize (size_type n, value_type val = value_type()) {
                if (n > max_size())
                    throw std::length_error("length not supported by allocator");
                if (n < m_size)
                {
                    for (size_type i(n) ; i < m_size ; i++)
                        m_Alloc.destroy(m_ptr + i);
                    m_size = n;
                }
                if (n > m_size && n < m_capacity)
                {
                    pointer temp = m_ptr + m_size;
                    for (size_type i(0) ; i < n - m_size ; i++)
                        m_Alloc.construct(temp++, val);
                    m_size = n;
                }
                if (n > m_size && n >= m_capacity)
                {
                    if (n > m_capacity && m_capacity > m_size)
                        reserve(m_capacity * 2);
                    else
                        reserve(n);
                    pointer temp = m_ptr + m_size;
                    for (size_type i(0); i < n - m_size; i++)
                        m_Alloc.construct(temp++, val);
                    m_size = n;
                }
            }
            void reserve (size_type n) {
                if (n > max_size())
                    throw std::length_error("length not supported by allocator");
                if (n > m_capacity)
                {
                    pointer temp = m_ptr;
                    pointer forwarder;
                    
                    m_ptr = m_Alloc.allocate(n);
                    forwarder = m_ptr;
                    for (size_type i(0); i < m_size ; i++)
                            m_Alloc.construct(forwarder++, temp[i]);
                    forwarder = temp;
                    for (size_type i(0); i < m_size ; i++)
                        m_Alloc.destroy(forwarder++);
                    if (temp)
                        m_Alloc.deallocate(temp, m_capacity);
                    m_capacity = n;
                }
            }

            /*=============================ELEMENT ACCESS================================*/
            reference operator[] (size_type n) {
                return m_ptr[n];
            }
            const_reference operator[] (size_type n) const {
                return m_ptr[n];
            }
            reference at (size_type n) {
                if (n < m_size)
                    return m_ptr[n];
                else
                    throw std::out_of_range("out of range");
            }
            const_reference at (size_type n) const {
                 if (n < m_size)
                    return m_ptr[n];
                else
                    throw std::out_of_range("out of range");
            }
            reference front() {
                return *(begin());
            }
            const_reference front() const {
                return *(begin());
            }
            reference back() {
                return *(end() - 1);
            }
            const_reference back() const {
                return *(end() - 1);
            }

            /*=============================MODIFIERS================================*/
            template <class InputIterator>  void assign (InputIterator first,
            typename enable_if<std::__is_input_iterator<InputIterator>::value &&
            !is_integral<InputIterator>::value, InputIterator>::type last) {
                if (!std::__is_input_iterator<InputIterator>::value)
                    throw InputIteratorException("Not a valid iterator");
                if (std::__is_input_iterator<InputIterator>::value && !std::__is_forward_iterator<InputIterator>::value)
                    for (InputIterator it = first; it != last; ++it)
                        push_back(*it);
                else
                {
                    difference_type gap = ft::distance(first, last);
                    size_type i = 0;
                    pointer tmp = m_Alloc.allocate(gap);
                    for (InputIterator it = first; it != last ; ++it, i++)
                        m_Alloc.construct(tmp + i, *it);
                    clear();
                    reserve(gap);
                    pointer forwarder = m_ptr;
                    for (size_type i(0); i < static_cast<unsigned long>(gap) ; i++)
                        m_Alloc.construct(forwarder++, tmp[i]);
                    m_size = gap;
                    for (size_type i(0); i < static_cast<unsigned long>(gap) ; i++)
                        m_Alloc.destroy(tmp + i);
                    m_Alloc.deallocate(tmp, gap);
                }
            }

            
            void assign (size_type n, const value_type& val) {
                if (n > max_size())
                    throw std::length_error("length not supported by allocator");
                reserve(n);
                pointer temp = m_ptr;
                for (size_type i(0); i < m_size; i++)
                    m_Alloc.destroy(temp++);
                temp = m_ptr;
                for (size_type i(0); i < n; i++)
                    m_Alloc.construct(temp++, val);
                m_size = n;
            }
            
            void push_back (const value_type& val) {
                if (empty())
                {
                    reserve(1);
                    m_Alloc.construct(m_ptr, val);
                    m_size++;
                }
                else if (m_size < m_capacity)
                    m_Alloc.construct(m_ptr + m_size++, val);
                else
                {
                    reserve(size() * 2);
                    m_Alloc.construct(m_ptr + m_size, val);
                    m_size++;
                }
            }
            
            void pop_back() {
                if (!empty())
                {
                    m_Alloc.destroy(&back());       
                    --m_size;
                }
            }

            iterator insert (iterator position, const value_type& val) {
                if (position == end() - 1 || empty())
                {
                    push_back(val);
                    return begin();
                }
                else if (m_size < m_capacity)
                {
                    reverse_iterator r_it = rbegin() - 1;
                    for ( ; r_it != position + 1; ++r_it)
                    {
                        m_Alloc.construct(&(*r_it), *(r_it + 1));
                        m_Alloc.destroy(&(*(r_it + 1)));
                    }
                    m_Alloc.construct(&(*r_it), val);
                    m_size++;
                }
                else
                {
                    size_type previous_size = m_size;
                    pointer tmp = m_Alloc.allocate(m_size);
                    for (size_type i(0); i < m_size ; i++)
                        m_Alloc.construct(tmp + i, m_ptr[i]);
                    difference_type pos = ft::distance(begin(), position);
                    reserve(m_size * 2);
                    position = begin() + pos;
                    size_type i(pos);
                    if (static_cast<unsigned long>(pos) < m_size)
                        m_Alloc.destroy(&(*position));
                    m_Alloc.construct(&(*position), val);
                    ++m_size;
                    iterator it = position + 1;
                    iterator it_end = end();
                    for (; it != it_end ; ++it, i++)
                    {
                        if (it != it_end - 1)
                            m_Alloc.destroy(&(*it));
                        m_Alloc.construct(&(*it), tmp[i]);
                    }
                    for (size_type i(0); i < previous_size ; i++)
                        m_Alloc.destroy(tmp + i);
                    m_Alloc.deallocate(tmp, previous_size);
                }
                return position;
            }
            void insert (iterator position, size_type n, const value_type& val) {
                if (n > max_size())
                    throw std::length_error("length not supported by allocator");
                if (empty())
                    assign(n, val);
                else if (m_size + n < m_capacity)
                {
                    reverse_iterator r_it = rbegin() - n;
                    for ( ; r_it != position + n; ++r_it)
                    {
                        m_Alloc.construct(&(*r_it), *(r_it + n));
                        m_Alloc.destroy(&(*(r_it + n)));
                    }
                    for (iterator it = position ; it != position + n; ++it)
                        m_Alloc.construct(&(*it), val);
                    m_size += n;
                }
                else
                {
                    size_type previous_size = m_size;
                    pointer tmp = m_Alloc.allocate(m_size);
                    for (size_type i(0); i < m_size ; i++)
                        m_Alloc.construct(tmp + i, m_ptr[i]);
                    difference_type pos = ft::distance(begin(), position);
                    if (m_size > n)
                        reserve(m_size * 2);
                    else
                        reserve(m_size + n);
                    position = begin() + pos;
                    size_type i(pos);
                    iterator it = position;
                    for (size_type j(0) ; j < n ; ++it, j++, pos++)
                    {
                        if (static_cast<unsigned long>(pos) < m_size)
                            m_Alloc.destroy(&(*it));
                        m_Alloc.construct(&(*it), val);
                    }
                    m_size += n;
                    it = position + n;
                    iterator it_end = end();
                    for (; it != it_end ; ++it, i++)
                    {
                        if (it < it_end - n)
                            m_Alloc.destroy(&(*it));
                        m_Alloc.construct(&(*it), tmp[i]);
                    }
                    for (size_type i(0); i < previous_size ; i++)
                        m_Alloc.destroy(tmp + i);
                    m_Alloc.deallocate(tmp, previous_size);
                }
            }
            template <class InputIterator> void insert (iterator position, InputIterator first,
            typename enable_if<std::__is_input_iterator<InputIterator>::value &&
            !is_integral<InputIterator>::value, InputIterator>::type last) {
                if (!std::__is_input_iterator<InputIterator>::value)
                    throw InputIteratorException("Not a valid iterator");
                 if (std::__is_input_iterator<InputIterator>::value && !std::__is_forward_iterator<InputIterator>::value)
                 {
                    if (position == end())
                        for (InputIterator it = first; it != last; ++it)
                            push_back(*it);
                    else
                    {
                        size_type previous_size = m_size;
                        pointer tmp = m_Alloc.allocate(m_size);
                        for (size_type i(0); i < m_size ; i++)
                            m_Alloc.construct(tmp + i, m_ptr[i]);
                        difference_type pos = ft::distance(begin(), position);
                        pointer forwarder = m_ptr;
                        size_type diff(m_size - pos);
                        for (size_type i(pos); i < m_size + 1 ; i++)
                        {
                            m_Alloc.destroy(forwarder++);
                            m_size--;
                        }
                        for (InputIterator it = first; it != last; ++it)
                            push_back(*it);
                        for (size_type i(pos) ; i < diff + pos ; i++)
                            push_back(tmp[i]);
                        for (size_type i(0); i < previous_size ; i++)
                            m_Alloc.destroy(tmp + i);
                        m_Alloc.deallocate(tmp, previous_size);
                    }
                 }
                else
                {
                    difference_type gap = ft::distance(first, last);
                    if (static_cast<unsigned long>(gap) > max_size())
                        throw std::length_error("length not supported by allocator");
                    if (empty())
                        assign(first, last);
                    else if (m_size + gap < m_capacity)
                    {
                        reverse_iterator r_it = rbegin() - gap;
                        for ( ; r_it != position + gap; ++r_it)
                        {
                            m_Alloc.construct(&(*r_it), *(r_it + gap));
                            m_Alloc.destroy(&(*(r_it + gap)));
                        }
                        InputIterator input_it = first;
                        for (iterator it = position ; input_it != last ; ++it, ++input_it)
                            m_Alloc.construct(&(*it), *input_it);
                        m_size += gap;
                    }
                    else
                    {
                        size_type previous_size = m_size;
                        pointer tmp = m_Alloc.allocate(m_size);
                        for (size_type i(0); i < m_size ; i++)
                            m_Alloc.construct(tmp + i, m_ptr[i]);
                        difference_type pos = ft::distance(begin(), position);
                        if (m_size > static_cast<unsigned long>(gap))
                            reserve(m_size * 2);
                        else
                            reserve(m_size + gap);
                        position = begin() + pos;
                        size_type i(pos);
                        iterator it = position;
                        for (InputIterator input_it = first ; input_it != last ; ++input_it, ++it, pos++)
                        {
                            if (static_cast<unsigned long>(pos) < m_size)
                                m_Alloc.destroy(&(*it));
                            m_Alloc.construct(&(*it), *input_it);
                        }
                        m_size += gap;
                        it = position + gap;
                        iterator it_end = end();
                        for (; it != end() ; ++it, i++)
                        {
                            if (it < it_end - gap)
                                m_Alloc.destroy(&(*it));
                            m_Alloc.construct(&(*it), tmp[i]);
                        }
                        for (size_type i(0); i < previous_size ; i++)
                            m_Alloc.destroy(tmp + i);
                        m_Alloc.deallocate(tmp, previous_size);
                    }
                }
            }
            iterator erase (iterator position) {
                if (empty())
                    return begin();
                if (position == end() - 1)
                    pop_back();
                else
                {
                    difference_type pos = ft::distance(begin(), position);
                    iterator it_end = end() - 1;
                    
                    for (iterator it = position ; it != it_end ; ++it, pos++)
                        m_ptr[pos] = m_ptr[pos + 1];
                    m_Alloc.destroy(m_ptr + pos);
                    m_size--;
                }
                return position;
            }
            iterator erase (iterator first, iterator last) {
                if (first != last)
                {
                    difference_type gap = ft::distance(first, last);
                    size_type i = ft::distance(begin(), first);
                    iterator it_end_gap = end() - gap;
                    iterator it_end = end();
                    for (iterator it = first ; it != it_end_gap ; ++it, ++i)
                        m_ptr[i] = m_ptr[i + gap];
                    for (iterator it = it_end_gap ; it != it_end ; ++it)
                        m_Alloc.destroy(&(*it));
                    m_size -= gap;
                    return first;
                }
                return begin();
            }
            void swap (vector& x) {
                if (this == &x)
                    return;
                pointer temp_ptr = x.m_ptr;
                size_type temp_size = x.m_size;
                size_type temp_capacity = x.m_capacity;

                x.m_ptr = m_ptr;
                x.m_size = m_size;
                x.m_capacity = m_capacity;

                m_ptr = temp_ptr;
                m_size = temp_size;
                m_capacity = temp_capacity;
            }
            void clear() {
                if (m_ptr)
                {
                    pointer temp = m_ptr;
                    for (size_type i(0) ; i < m_size ; i++)
                        m_Alloc.destroy(temp++);
                }
                m_size = 0;
            }

            /*=============================ALLOCATOR================================*/
            allocator_type get_allocator() const {return m_Alloc;}
            
            /*=============================COMPARISON OPERATORS OVERLOAD================================*/
            friend bool operator== (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
                if (lhs.size() != rhs.size())
                    return false;
                if (ft::equal(lhs.begin(), lhs.end(), rhs.begin()))
                    return true;
                return false;
            }
            friend bool operator!= (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {return !(operator==(lhs, rhs));}
            friend bool operator< (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());}
            friend bool operator> (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {return operator<(rhs, lhs);}
            friend bool operator<= (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {return !(operator>(lhs, rhs));}
            friend bool operator>= (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {return !(operator<(lhs, rhs));}
            
            private:
                value_type* m_ptr;
                size_type m_size;
                size_type m_capacity;
                size_type to_deallocate;
                allocator_type m_Alloc;
    };

    /*=============================NON-MEMBER SWAP================================*/
    template <class T, class Alloc>
    void swap (vector<T, Alloc>& x, vector<T, Alloc>& y) {
        x.swap(y);
    }
}

#endif