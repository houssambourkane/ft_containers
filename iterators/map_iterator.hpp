/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_iterator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourkan <hbourkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 22:37:56 by hbourkan          #+#    #+#             */
/*   Updated: 2022/11/25 22:27:13 by hbourkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MAP_ITERATOR_HPP
#define MAP_ITERATOR_HPP

#include <iostream>
#include "../utility/pair.hpp"
#include "../data_structures/AVL.hpp"

namespace ft
{
    template <class T>
    class AVL_iterator : iterator<std::bidirectional_iterator_tag, T>
    {
        public:
            /*=============================TYPEDEFS================================*/
            typedef typename T::value_type value_type;
            typedef typename iterator<std::bidirectional_iterator_tag, value_type>::iterator_category iterator_category;
            typedef typename iterator<std::bidirectional_iterator_tag, value_type>::difference_type difference_type;
            typedef typename iterator<std::bidirectional_iterator_tag, value_type>::pointer pointer;
            typedef typename iterator<std::bidirectional_iterator_tag, value_type>::reference reference;

            /*=============================CONSTURCTORS================================*/
            AVL_iterator() : p(nullptr) {}
            AVL_iterator(T* x) : p(x) {}
            AVL_iterator(const AVL_iterator& src) : p(src.p) {}

            /*=============================ASSIGNMENT OPERATOR================================*/
            AVL_iterator& operator=(const AVL_iterator& rhs) {
                if (this == &rhs)
                    return *this;
                p = rhs.p;
                return *this;
            }
            /*=============================DESTRUCTOR================================*/
            virtual ~AVL_iterator() {}

            /*=============================MEMBER FUNCTIONS================================*/
            T* base() const {return p;}
            AVL_iterator& operator++() {
                T* temp = p;
                if (temp) {
                    if (temp->right)
                    {
                        temp = p->min_node(temp->right);
                        p = temp;
                    }
                    else if (temp->parent && temp == temp->parent->right)
                    {
                        while (temp->parent && temp != temp->parent->left)
                            temp = temp->parent;
                        if (temp->parent)
                            temp = temp->parent;
                        p = temp;
                    }
                    else
                    {
                        if (temp->parent)
                            temp = temp->parent;
                        p = temp;
                    }
                }
                else
                    p = p->min_node(p);
                return *this;
            }
            AVL_iterator  operator++(int) {T* tmp = p; operator++(); return tmp;}
            AVL_iterator& operator--() {
                if (p->is_null_node)
                {
                    p = p->parent;
                    return *this;
                }
                T* temp = p;
                if (temp) {
                    if (temp->left)
                    {
                        if (temp->left->right)
                            temp = p->max_node(temp->left->right);
                        else
                            temp = temp->left;
                        p = temp;
                    }
                    else if (temp == temp->parent->left)
                    {

                        while (temp->parent && temp != temp->parent->right)
                            temp = temp->parent;
                        if (temp->parent)
                            temp = temp->parent;
                        p = temp;
                    }
                    else
                    {
                        if (temp->parent)
                            temp = temp->parent;
                        p = temp;
                    }
                }
                else 
                    p = p->last_element(p);
                return *this;
            }
            AVL_iterator  operator--(int) {T* tmp = p; operator--(); return tmp;}
            reference operator*() const {return p->data;}
            pointer operator->() const {return &p->data;}
            operator AVL_iterator<const T> () const {  return (AVL_iterator<const T>(p));}
            bool operator==(const AVL_iterator& rhs) const {return p == rhs.p;}
            bool operator!=(const AVL_iterator& rhs) const {return p != rhs.p;}
        private:
            T* p;
    };
}
#endif