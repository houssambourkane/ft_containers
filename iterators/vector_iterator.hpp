/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_iterator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourkan <hbourkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 22:36:42 by hbourkan          #+#    #+#             */
/*   Updated: 2022/12/09 16:03:20 by hbourkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP

#include <iostream>
#include "iterator_traits.hpp"

namespace ft
{
    template <class T>
    class my_random_access_iterator : iterator<std::random_access_iterator_tag, T>
    {
        public:
            /*=============================TYPEDEFS================================*/
            typedef typename iterator<std::random_access_iterator_tag, T>::value_type value_type;
            typedef typename iterator<std::random_access_iterator_tag, T>::iterator_category iterator_category;
            typedef typename iterator<std::random_access_iterator_tag, T>::difference_type difference_type;
            typedef T* pointer;
            typedef T& reference;

            /*=============================CONSTRUCTORS================================*/
            my_random_access_iterator() : p(nullptr) {}
            my_random_access_iterator(pointer x) : p(x) {}
            my_random_access_iterator(const my_random_access_iterator& src) : p(src.p) {}
            /*=============================ASSIGNMENT OPERATOR================================*/
            my_random_access_iterator& operator=(const my_random_access_iterator& rhs) {
                if (this == &rhs)
                    return *this;
                p = rhs.base();
                return *this;
            }
            /*=============================DESTRUCTOR================================*/
            virtual ~my_random_access_iterator() {}

            /*=============================MEMBER FUNCTIONS================================*/
            pointer base() const {return p;}
            my_random_access_iterator& operator++() {++p; return *this;}
            my_random_access_iterator  operator++(int) {pointer tmp = p; operator++(); return tmp;}
            my_random_access_iterator& operator--() {--p; return *this;}
            my_random_access_iterator  operator--(int) {pointer tmp = p; operator--(); return tmp;}
            my_random_access_iterator  operator+(difference_type x) const {return p + x;}
            my_random_access_iterator& operator+=(difference_type x) {p += x; return *this;}
            my_random_access_iterator  operator-(difference_type x) const {return p - x;}
            my_random_access_iterator& operator-=(difference_type x) {p -= x; return *this;}
            reference operator*() {return *p;}
            reference operator[](difference_type x) {return *(operator+(x));}
            pointer operator->() {return &(operator*());}
            operator my_random_access_iterator<const T> () const {  return (my_random_access_iterator<const T>(p)); }
            bool operator==(const my_random_access_iterator& rhs) const {return p == rhs.p;}
            bool operator!=(const my_random_access_iterator& rhs) const {return p != rhs.p;}
        private:
            pointer p;
    };
    
    /*=============================COMPARISON OPERATORS================================*/
    template <class T, class U>
    bool operator==(const my_random_access_iterator<T>& lhs, const my_random_access_iterator<U>& rhs) {
        return lhs.base() == rhs.base();
    }

    template <class T, class U>
    bool operator!=(const my_random_access_iterator<T>& lhs, const my_random_access_iterator<U>& rhs) {
        return !(lhs == rhs);
    }

    template <class T, class U>
    bool operator<(const my_random_access_iterator<T>& lhs, const my_random_access_iterator<U>& rhs) {
        return lhs.base() < rhs.base();
    }

    template <class T, class U>
    bool operator>(const my_random_access_iterator<T>& lhs, const my_random_access_iterator<U>& rhs) {
        return rhs < lhs;
    }

    template <class T, class U>
    bool operator<=(const my_random_access_iterator<T>& lhs, const my_random_access_iterator<U>& rhs) {
        return !(lhs > rhs);
    }

    template <class T, class U>
    bool operator>=(const my_random_access_iterator<T>& lhs, const my_random_access_iterator<U>& rhs) {
        return !(lhs < rhs);
    }

    /*=============================ITERATOR OPERATORS================================*/

    template <class T, class U>
    typename my_random_access_iterator<T>::difference_type operator-(const my_random_access_iterator<T>& lhs, const my_random_access_iterator<U>& rhs) {
        return lhs.base() - rhs.base();
    }

    template <class T>
    typename my_random_access_iterator<T>::difference_type operator-(const my_random_access_iterator<T>& lhs, const my_random_access_iterator<T>& rhs) {
        return lhs.base() - rhs.base();
    }
    
    template <class T>
    my_random_access_iterator<T> operator+(typename my_random_access_iterator<T>::difference_type n, const my_random_access_iterator<T>& x) {
        return my_random_access_iterator<T>(x.base() + n);
    }

   template<class T>
    bool operator!=(const reverse_iterator<my_random_access_iterator<T> >& lhs, const my_random_access_iterator<T>& rhs) {
        return lhs.base() != rhs.base();
    }
}

#endif