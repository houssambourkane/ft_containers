/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_traits.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourkan <hbourkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 19:58:15 by hbourkan          #+#    #+#             */
/*   Updated: 2022/12/09 16:03:34 by hbourkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <iostream>
#include <type_traits>


namespace ft
{
    /*=============================ITERATOR TRAITS================================*/
    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::difference_type   difference_type;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
        typedef typename Iterator::iterator_category iterator_category;
    };

    template <class T>
    struct iterator_traits<T*> {
      typedef T                                 value_type;
      typedef ptrdiff_t                         difference_type;
      typedef T*                                pointer;
      typedef T&                                reference;
      typedef std::random_access_iterator_tag iterator_category;
    };

    template <class T>
    struct iterator_traits<const T*> {
    typedef T                                   value_type;
    typedef ptrdiff_t                           difference_type;
    typedef const T*                            pointer;
    typedef const T&                            reference;
      typedef std::random_access_iterator_tag   iterator_category;
    };


    /*=============================ITERATOR================================*/
    template<class Category, class T, class Distance = ptrdiff_t,
        class Pointer = T*, class Reference = T&>
    struct iterator
    {
        typedef T         value_type;
        typedef Distance  difference_type;
        typedef Pointer   pointer;
        typedef Reference reference;
        typedef Category  iterator_category;
    };

    /*=============================EXCEPTIONS================================*/
    class InputIteratorException : public std::exception {
        private:
	        std::string m_error;
        public:
            InputIteratorException(std::string error)
                : m_error(error) {}

            virtual const char* what() const _NOEXCEPT { return m_error.c_str(); }
            virtual ~InputIteratorException() throw() {}
    };


    /*=============================REVERSE ITERATOR================================*/
    template <class Iterator>
    class reverse_iterator
    {
        public:
            /*=============================TYPEDEFS================================*/
            typedef Iterator iterator_type;
            typedef typename iterator_traits<iterator_type>::value_type value_type;
            typedef typename iterator_traits<iterator_type>::iterator_category iterator_category;
            typedef typename iterator_traits<iterator_type>::difference_type difference_type;
            typedef typename iterator_traits<iterator_type>::pointer pointer;
            typedef typename iterator_traits<iterator_type>::reference reference;
            
            /*=============================CONSTRUCTOR================================*/
            reverse_iterator() : p() {}
            explicit reverse_iterator(iterator_type x) : p(x) {}
            template <class U> reverse_iterator(const reverse_iterator<U>& u) : p(u.base()){}
            /*=============================DESTRUCTOR================================*/
            virtual ~reverse_iterator() {}
            /*=============================MEMBER FUNCTIONS================================*/
            iterator_type base() const {return p;}
            reverse_iterator& operator++() {--p; return *this;}
            reverse_iterator  operator++(int) {iterator_type temp = p; operator++(); return reverse_iterator(temp);}
            reverse_iterator& operator--() {++p; return *this;}
            reverse_iterator  operator--(int) {iterator_type temp = p; operator--(); return reverse_iterator(temp);}
            reverse_iterator  operator+(difference_type x) const {return reverse_iterator(p - x);}
            reverse_iterator  operator-(difference_type x) const {return reverse_iterator(p + x);}
            reverse_iterator& operator+=(difference_type x) {p -= x; return *this;}
            reverse_iterator& operator-=(difference_type x) {p += x; return *this;}
            reference operator*() const {iterator_type temp(p); return *--temp;}
            pointer operator->() const {return &(operator*());}
            reference operator[](difference_type x) const {return base()[- x - 1];}
        protected:
            iterator_type p;
    };

    /*=============================COMPARISON OPERATORS================================*/
    
    template <class Iter1, class Iter2>
    bool operator==(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
        return lhs.base() == rhs.base();
    }

    template <class Iter1, class Iter2>
    bool operator!=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
        return lhs.base() != rhs.base();
    }

    template <class Iter1, class Iter2>
    bool operator<(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
        return lhs.base() > rhs.base();
    }

    template <class Iter1, class Iter2>
    bool operator>(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
        return lhs.base() < rhs.base();
    }

    template <class Iter1, class Iter2>
    bool operator<=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
        return lhs.base() >= rhs.base();
    }

    template <class Iter1, class Iter2>
    bool operator>=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
        return lhs.base() <= rhs.base();
    }
    
    /*=============================REVERSE ITERATOR OPERATORS================================*/

    template <class Iter1, class Iter2>
    typename reverse_iterator<Iter1>::difference_type operator-(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
        return rhs.base() - lhs.base();
    }

    template <class Iter>
    typename reverse_iterator<Iter>::difference_type operator-(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs) {
        return rhs.base() - lhs.base();
    }
    template <class Iter>
    reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& x) {
        return reverse_iterator<Iter>(x.base() - n);
    }


    /*=============================DISTANCE================================*/
    template<class InputIterator>  
    typename iterator_traits<InputIterator>::difference_type distance (InputIterator first, InputIterator last) {
        typename iterator_traits<InputIterator>::difference_type r(0);
        for (InputIterator it = first ; it != last; ++it)
            ++r;
        return r;
    }
}

#endif