#pragma once

#include <ratio>

namespace std
{
    namespace units
    {
        namespace si
        {
            template <typename Rep, typename Length = std::ratio<1>>
            struct distance
            {
                using rep = Rep;

                constexpr distance(rep value)
                    : value{value}
                {
                }

                constexpr rep count() const { return value; }

            private:
                rep value;
            };

            using metres      = distance<long int>;
            using kilometres  = distance<long int, std::kilo>;
        }
    }

    namespace literals
    {
        namespace distance_literals
        {
            constexpr std::units::si::metres operator"" _m(unsigned long long int dist)
            {
                return std::units::si::metres{static_cast<std::units::si::metres::rep>(dist)};
            }
        }
    }
}
