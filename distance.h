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

                constexpr distance(int value)
                    : value{value}
                {
                }

                constexpr int count() const { return value; }

            private:
                rep value;
            };

            using metres     = distance<int>;
            using kilometres = distance<int, std::ratio<1000, 1>>;
        }
    }
}
