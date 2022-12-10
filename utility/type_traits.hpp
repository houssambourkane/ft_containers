/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourkan <hbourkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 19:32:13 by hbourkan          #+#    #+#             */
/*   Updated: 2022/12/09 16:05:19 by hbourkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP
#include <type_traits>
namespace ft
{
    /*=============================ENABLE IF================================*/
    template <bool Cond, class T = void> struct enable_if;

    /*=============================ENABLE IF SPECIALIZATION================================*/
    template<class T>
    struct enable_if<true, T> { typedef T type; };
    
    /*=============================IS INTEGRAL================================*/
    template <class T>   struct is_integral                      : std::false_type {};

    /*=============================IS INTEGRAL SPECIALIZATIONS================================*/
    template <>          struct is_integral<bool>                : std::true_type {};
    template <>          struct is_integral<char>                : std::true_type {};
    template <>          struct is_integral<char16_t>            : std::true_type {};
    template <>          struct is_integral<char32_t>            : std::true_type {};
    template <>          struct is_integral<wchar_t>             : std::true_type {};
    template <>          struct is_integral<signed char>         : std::true_type {};
    template <>          struct is_integral<short>               : std::true_type {};
    template <>          struct is_integral<int>                 : std::true_type {};
    template <>          struct is_integral<long>                : std::true_type {};
    template <>          struct is_integral<long long>           : std::true_type {};
    template <>          struct is_integral<unsigned char>       : std::true_type {};
    template <>          struct is_integral<unsigned short>      : std::true_type {};
    template <>          struct is_integral<unsigned int>        : std::true_type {};
    template <>          struct is_integral<unsigned long>       : std::true_type {};
    template <>          struct is_integral<unsigned long long>  : std::true_type {};
    template <>          struct is_integral<__int128_t>          : std::true_type {};
    template <>          struct is_integral<__uint128_t>         : std::true_type {};
}

#endif