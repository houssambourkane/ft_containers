/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourkan <hbourkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 13:45:21 by hbourkan          #+#    #+#             */
/*   Updated: 2022/12/09 15:59:31 by hbourkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
#define STACK_HPP

#include "vector.hpp"

namespace ft
{
    template <class T, class Container = ft::vector<T> >
    class stack {
        public:
            /*=============================TYPEDEFS================================*/
            typedef Container container_type;
            typedef typename Container::iterator iterator;
            typedef typename Container::value_type value_type;
            typedef typename Container::size_type size_type;
            typedef typename Container::reference reference;
            typedef typename Container::const_reference const_reference;
            typedef typename Container::pointer pointer;
            typedef typename Container::const_pointer const_pointer;
            /*=============================CONSTRUCTOR================================*/
            stack (const container_type& ctnr = container_type()) : c(ctnr) {}
            /*=============================DESTRUCTOR================================*/
            ~stack() {}
            /*=============================MEMBER FUNTIONS================================*/
            size_type size() const { return c.size(); }
            void push (const value_type& val) {c.push_back(val);}
            void pop() {c.pop_back();}
            bool empty() const {return c.empty();}
            value_type& top() {return c.back();}
            const value_type& top() const {return c.back();}
            friend bool operator== (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {return lhs.c == rhs.c;}
            friend bool operator!= (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {return lhs.c != rhs.c;}
            friend bool operator<  (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {return lhs.c < rhs.c;}
            friend bool operator<= (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {return lhs.c <= rhs.c;}
            friend bool operator>  (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {return lhs.c > rhs.c;}
            friend bool operator>= (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {return lhs.c >= rhs.c;}
            void swap (stack& x) {c.swap(x.c);}
        protected:
            Container c;
    };

}

#endif