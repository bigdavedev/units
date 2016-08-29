#pragma once

#include <type_traits>
#include <cstdint>

namespace utility
{
    template<intmax_t Numerator>
    struct integer_sign
        : std::integral_constant<intmax_t, (Numerator < 0) ? -1 : 1>
    {};

    template<intmax_t Numerator>
    struct integer_abs
        : std::integral_constant<intmax_t, Numerator * integer_sign<Numerator>::value>
    {};

    template<intmax_t Numerator, intmax_t Quotient>
    struct greatest_common_divisor;

    template<intmax_t Numerator, intmax_t Quotient>
    struct greatest_common_divisor
        : greatest_common_divisor<Quotient, (Numerator % Quotient)>
    {};

    template<intmax_t Numerator>
    struct greatest_common_divisor<Numerator, 0>
        : std::integral_constant<intmax_t, integer_sign<Numerator>::value>
    {};

    template<intmax_t Quotient>
    struct greatest_common_divisor<0, Quotient>
        : std::integral_constant<intmax_t, integer_sign<Quotient>::value>
    {};


}
