/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourkan <hbourkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 18:39:33 by hbourkan          #+#    #+#             */
/*   Updated: 2022/12/09 15:58:53 by hbourkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVL_HPP
#define AVL_HPP

#include "../iterators/map_iterator.hpp"

namespace ft
{
    template <class T>
    struct Node {

        typedef T value_type;
        struct Node *left;
        struct Node *right;
        struct Node *parent;
        bool is_null_node;
        int height;
        value_type data;

        /*=============================TYPEDEFS================================*/
        typedef ft::Node<value_type> node;
        typedef node* node_pointer;

        /*=============================CONSTRUCTOR================================*/
        Node(value_type x) : data(x) {}

        /*=============================MEMBER FUNCTIONS================================*/
        node_pointer max_node(node_pointer root) {
            node_pointer temp = root;
            if (temp)
            {
                while (temp->right)
                    temp = temp->right;
            }
            return temp;
        }
        node_pointer last_element(node_pointer root) {
            node_pointer temp = root;
            if (temp && temp->right)
                while (temp->right->right)
                    temp = temp->right;
            return temp;
        }
        node_pointer first_element(node_pointer root) {
            node_pointer temp = root;
            if (temp && temp->left)
                while (temp->left->left)
                    temp = temp->left;
            return temp;
        }
        node_pointer min_node(node_pointer root) {
            node_pointer temp = root;
            if (temp)
                while (temp->left)
                    temp = temp->left;
            return temp;
        }
        
        bool operator==(const Node& rhs) {return (data == rhs.data);}
    };

    template <class T, class Compare = std::less<T>, class Alloc = std::allocator<Node<T> > > 
    class AVL {
        public:

            /*=============================TYPEDEFS================================*/
            typedef T value_type;
            typedef ft::Node<value_type> node;
            typedef node* node_pointer;
            typedef ft::AVL_iterator<node> iterator;
            typedef ft::AVL_iterator<node> const_iterator;
            typedef ft::reverse_iterator<iterator> reverse_iterator;
            typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

            /*=============================CONSTRUCTOR================================*/
            AVL(const Compare& comp = Compare()) :
            root(nullptr), m_comp(comp), m_Alloc(Alloc()),max_element(nullptr), min_element(nullptr),
            null_node_left(set_null_node()), null_node_right(set_null_node()), inserted(0), deleted(0) {}

            /*=============================DESTRUCTOR================================*/
            ~AVL() {
                clear();
                get_allocator().deallocate(null_node_right, 1);
                get_allocator().deallocate(null_node_left, 1);
                null_node_right = nullptr;
                null_node_left = nullptr;
            }
            
            /*=============================ALLOCATOR================================*/
            Alloc get_allocator() const {return m_Alloc;}

            /*=============================ITERATORS================================*/
            iterator begin() {return iterator(root->first_element(root));}
            iterator end() {return iterator(root->max_node(root));}
            const_iterator begin() const {return const_iterator(root->first_element(root));}
            const_iterator end() const {return const_iterator(root->max_node(root));}
            reverse_iterator rbegin() {return reverse_iterator(root->max_node(root));}
            reverse_iterator rend() {return reverse_iterator(root->first_element(root));}
            const_reverse_iterator rbegin() const {return const_reverse_iterator(root->max_node(root));}
            const_reverse_iterator rend() const {return const_reverse_iterator(root->first_element(root));}
            

            /*=============================MEMBER FUNCTIONS================================*/
            int add_height(node_pointer root)
            {
                if (root->right && root->left)
                {
                    if (root->right->height > root->left->height)
                        return root->right->height + 1;
                    else
                        return root->left->height + 1;
                }
                else if (!root->right && root->left)
                    return root->left->height + 1;
                else if (root->right && !root->left)
                    return root->right->height + 1;
                return 1;
            }
            
            int BF_Calculator(node_pointer root) {
                if (root)
                {
                    if (root->left && root->right)
                        return root->left->height - root->right->height;
                    else if (!root->left && root->right)
                        return -root->right->height;
                    else if (root->left && !root->right)
                        return root->left->height; 
                }
                return 0;
            }
            
            node_pointer set_null_node() {
                node_pointer node = get_allocator().allocate(1);
                node->right = node->left = nullptr;
                node->is_null_node = 1;
                return node;
            }
            
            node_pointer insert(node_pointer root, value_type element)
            {
                if (!root)
                {
                    node_pointer temp = get_allocator().allocate(1);
                    get_allocator().construct(temp, ft::Node<value_type>(element));
                    root = temp;
                    root->height = 1;
                    root->is_null_node = 0;
                    root->left = root->right = root->parent = nullptr;
                    inserted = 1;
                    return root;
                }
                else
                {
                    if (!m_comp(root->data.first, element.first) && !m_comp(element.first, root->data.first))
                        return root;
                    else if (!m_comp(root->data.first, element.first))
                    {
                        root->left = insert(root->left, element);
                        root->left->parent = root;
                    }
                    else if (m_comp(root->data.first, element.first))
                    {
                        root->right = insert(root->right, element);
                        root->right->parent = root;
                    }
                }
                
                root->height = add_height(root);
                int bf = BF_Calculator(root);
                int bf_right = BF_Calculator(root->right);
                int bf_left = BF_Calculator(root->left);
                if (bf == 2 && bf_left == 1)
                    root = left_left_rot(root);
                else if (bf == 2 && bf_left == -1)
                    root = left_right_rot(root);
                else if (bf == -2 && bf_right == -1)
                    root = right_right_rot(root);
                else if (bf == -2 && bf_right == 1)
                    root = right_left_rot(root);
                return root;
            }

            bool insert_node(value_type element)
            {
                if (max_element)
                    max_element->right = nullptr;
                if (min_element)
                    min_element->left = nullptr;
                root = insert(root, element);
                max_element = max_node(root);
                if (max_element)
                {
                    max_element->right = null_node_right;
                    max_element->right->parent = max_element;
                }
                min_element = min_node(root);
                if (min_element)
                {
                    min_element->left = null_node_left;
                    min_element->left->parent = min_element;
                }
                if (inserted == 0)
                    return false;
                inserted = 0;
                return true;
            }
            
            node_pointer left_left_rot(node_pointer root) {
                node_pointer temp = root->left;
                node_pointer temp2 = root->parent;
                
                root->left = temp->right;
                if (temp->right)
                    temp->right->parent = temp->parent;
                temp->right = root;
                root->parent = temp;
                temp->parent = temp2;
                root->height = add_height(root);
                temp->height = add_height(temp);
                return temp;
            }
            
            node_pointer right_right_rot(node_pointer root) {
                node_pointer temp = root->right;
                node_pointer temp2 = root->parent;
                
                root->right = temp->left;
                if (temp->left)
                    temp->left->parent = temp->parent;
                temp->left = root;
                root->parent = temp;
                temp->parent = temp2;
                root->height = add_height(root);
                temp->height = add_height(temp);
                return temp;
            }
            
            node_pointer left_right_rot(node_pointer root) {
                
                node_pointer temp = root->left;
                node_pointer temp2 = root->left->right;
                node_pointer save_parent = root->parent;

                root->left = temp2->right;
                if (temp2->right)
                    temp2->right->parent = root;

                temp->right = temp2->left;
                if (temp2->left)
                    temp2->left->parent = temp;
                temp2->right = root;
                root->parent = temp2;
                temp2->left = temp;
                temp->parent = temp2;
                temp2->parent = save_parent;
                root->height = add_height(root);
                temp->height = add_height(temp);
                temp2->height = add_height(temp2);
   
                return temp2;
            }

            node_pointer right_left_rot(node_pointer root) {
                node_pointer temp = root->right;
                node_pointer temp2 = root->right->left;
                node_pointer save_parent = root->parent;

                root->right = temp2->left;
                if (temp2->left)
                    temp2->left->parent = root;
                
                temp->left = temp2->right;
                if (temp2->right)
                    temp2->right->parent = temp;
                temp2->left = root;
                root->parent = temp2;
                temp2->right = temp;
                temp->parent = temp2;
                temp2->parent = save_parent;
                root->height = add_height(root);
                temp->height = add_height(temp);
                temp2->height = add_height(temp2);
                return temp2;
            }

            node_pointer max_node(node_pointer root) {
                node_pointer temp = root;
                if (temp)
                    while (temp->right)
                        temp = temp->right;
                return temp;
            }
            
            node_pointer min_node(node_pointer root) {
                node_pointer temp = root;
                if (temp)
                    while (temp->left)
                        temp = temp->left;
                return temp;
            }
            
            node_pointer find_key(node_pointer root, value_type key) const {
                node_pointer temp = root;
                if (temp)
                {
                    if (!m_comp(temp->data.first, key.first) && !m_comp(key.first, temp->data.first))
                        return temp;
                    else if (!m_comp(temp->data.first, key.first))
                        temp = find_key(temp->left, key);
                    else if (m_comp(temp->data.first, key.first))
                        temp = find_key(temp->right, key);
                }
                return temp;
            }
            
            node_pointer find(value_type key) const {
                return find_key(root, key);
            }
            
            node_pointer delete_node(node_pointer root, value_type key) {
                if (!root)
                    return root;
                if (!root->left && !root->right && (!m_comp(key.first, root->data.first) && !m_comp(root->data.first, key.first)))
                {
                    node_pointer temp = root;
                    get_allocator().destroy(root);
                    root = nullptr;
                    get_allocator().deallocate(temp, 1);
                    deleted = 1;
                    return root;
                }
                
                if (!m_comp(key.first, root->data.first) && !m_comp(root->data.first, key.first))
                {
                    if (!root->left && root->right)
                    {
                        node_pointer temp = root->right;
                        node_pointer temp2 = root;

                        temp->parent = root->parent;
                        get_allocator().destroy(root);
                        root = nullptr;
                        get_allocator().deallocate(temp2, 1);
                        deleted = 1;
                        return temp;
                    }
                    else if (!root->right && root->left)
                    {
                        node_pointer temp = root->left;
                        node_pointer temp2 = root;

                        temp->parent = root->parent;
                        get_allocator().destroy(root);
                        root = nullptr;
                        get_allocator().deallocate(temp2, 1);
                        deleted = 1;
                        return temp;
                    }
                    else
                    {
                        node_pointer temp = max_node(root->left);
                        node_pointer temp2 = get_allocator().allocate(1);
                        get_allocator().construct(temp2, *temp);
                        
                        temp2->height = root->height;
                        temp2->right = root->right;
                        temp2->left = root->left;
                        temp2->parent = root->parent;
                        temp2->is_null_node = root->is_null_node;
                        
                        if (root->right)
                            root->right->parent = temp2;
                        if (root->left)
                            root->left->parent = temp2;
                        if (root->parent && root == root->parent->right)
                            root->parent->right = temp2;
                        else if (root->parent && root == root->parent->left)
                            root->parent->left = temp2;
                            
                        get_allocator().destroy(root);
                        get_allocator().deallocate(root, 1);
                        root = temp2;

                        root->left = delete_node(root->left, temp->data);
                    }
                }
                else if (!m_comp(root->data.first, key.first))
                    root->left = delete_node(root->left, key);
                else if (m_comp(root->data.first, key.first))
                    root->right = delete_node(root->right, key);
                int bf = BF_Calculator(root);
                int bf_right = BF_Calculator(root->right);
                int bf_left = BF_Calculator(root->left);
                if (bf == 2 && bf_left >= 0)
                    root = left_left_rot(root);
                else if (bf == 2 && bf_left == -1)
                    root = left_right_rot(root);
                else if (bf == -2 && bf_right <= 0)
                    root = right_right_rot(root);
                else if (bf == -2 && bf_right == 1)
                    root = right_left_rot(root);
                return root;
            }
            
            bool delete_key(value_type key) {
                if (max_element)
                    max_element->right = nullptr;
                if (min_element)
                    min_element->left = nullptr;
                root = delete_node(root, key);
                max_element = max_node(root);
                if (max_element)
                {
                    max_element->right = null_node_right;
                    max_element->right->parent = max_element;
                }
                
                min_element = min_node(root);
                if (min_element)
                {
                    min_element->left = null_node_left;
                    min_element->left->parent = min_element;
                }
                if (deleted == 0)
                    return false;
                deleted = 0;
                return true;
            }
            
            void swap(AVL & x) {
                node_pointer temp_root = x.root;
                bool temp_inserted = x.inserted;
                bool temp_deleted = x.deleted;
                node_pointer temp_max_element = x.max_element;
                node_pointer temp_min_element = x.min_element;
                node_pointer temp_null_node_left = x.null_node_left;
                node_pointer temp_null_node_right = x.null_node_right;
                
                x.root = root;
                x.inserted = inserted;
                x.deleted = deleted;
                x.max_element = max_element;
                x.min_element = min_element;
                x.null_node_left = null_node_left;
                x.null_node_right = null_node_right;

                root = temp_root;
                inserted = temp_inserted;
                deleted = temp_deleted;
                max_element = temp_max_element;
                min_element = temp_min_element;
                null_node_left = temp_null_node_left;
                null_node_right = temp_null_node_right;
            }

            node_pointer clear_tree(node_pointer root) {
                if (root)
                {
                    root->left = clear_tree(root->left);
                    root->right = clear_tree(root->right);
                    if (root->left)
                        root->left = nullptr;
                    if (root->right)
                        root->right = nullptr;
                    node_pointer temp = root;
                    get_allocator().destroy(root);
                    root = nullptr;
                    get_allocator().deallocate(temp, 1);
                }
                return root;
            }
            void clear() {
                if (max_element)
                    max_element->right = nullptr;
                if (min_element)
                    min_element->left = nullptr;
                root = clear_tree(root);
                max_element = max_node(root);
                if (max_element)
                {
                    max_element->right = null_node_right;
                    max_element->right->parent = max_element;
                }
                min_element = min_node(root);
                if (min_element)
                {
                    min_element->left = null_node_left;
                    min_element->left->parent = min_element;
                }
            }

                
        private:
            node_pointer root;
            Compare m_comp;
            Alloc m_Alloc;
            node_pointer max_element;
            node_pointer min_element;
            node_pointer null_node_left;
            node_pointer null_node_right;
            bool inserted;
            bool deleted;
    };

}

#endif