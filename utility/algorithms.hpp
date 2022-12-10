/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithms.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourkan <hbourkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 18:26:02 by hbourkan          #+#    #+#             */
/*   Updated: 2022/12/09 16:03:57 by hbourkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

namespace ft
{
    /*=============================EQUAL================================*/
    template <class Iter1, class Iter2>
    bool equal(
        Iter1 lhs_it,
        Iter1 lhs_it_last,
        Iter2 rhs_it)
    {
        while (lhs_it != lhs_it_last)
        {
            if (*lhs_it != *rhs_it)
                return false;
            lhs_it++;
            rhs_it++;
        }
        return true;
    }

    /*=============================LEXICOGRAPHICAL COMPARE================================*/
    template <class Iter1, class Iter2>
    bool lexicographical_compare(
        Iter1 lhs_it,
        Iter1 lhs_it_last,
        Iter2 rhs_it,
        Iter2 rhs_it_last)
    {
        while (lhs_it != lhs_it_last)
        {
            if (rhs_it == rhs_it_last || *lhs_it > *rhs_it)
                return false;
            else if (*lhs_it < *rhs_it)
                return true;
            lhs_it++;
            rhs_it++;
        }
        return (rhs_it != rhs_it_last);
    }
    
}

#endif