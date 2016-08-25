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
                using rep    = Rep;
                using length = Length;

                constexpr distance(rep value)
                    : value{value}
                {
                }

                constexpr rep count() const { return value; }

                distance& operator++()
                {
                    ++value;
                    return *this;
                }
                distance operator++(int) { return distance{value + 1}; }
                distance& operator--()
                {
                    --value;
                    return *this;
                }
                distance operator--(int) { return distance{value - 1}; }

            private:
                rep value;
            };

            using metres     = distance<long int>;
            using kilometres = distance<long int, std::kilo>;
            using yards      = distance<double, std::ratio<9144, 10000>>;

            namespace detail
            {
                template <
                    typename ToDistance,
                    typename Ratio,
                    typename CommonType,
                    bool RatioNumIsOne = false,
                    bool RatioDenIsOne = false>
                struct distance_cast
                {
                    template <typename Rep, typename Length>
                    static constexpr ToDistance cast(distance<Rep, Length> from)
                    {
                        return ToDistance{
                            static_cast<typename ToDistance::rep>(static_cast<CommonType>(from.count())
                                                                  / static_cast<CommonType>(Ratio::num)
                                                                  * static_cast<CommonType>(Ratio::den))};
                    }
                };

                template <typename ToDistance, typename Ratio, typename CommonType>
                struct distance_cast<ToDistance, Ratio, CommonType, true, true>
                {
                    template <typename Rep, typename Length>
                    static constexpr ToDistance cast(distance<Rep, Length> from)
                    {
                        return ToDistance{
                            static_cast<typename ToDistance::rep>(static_cast<CommonType>(from.count()))};
                    }
                };

                template <typename ToDistance, typename Ratio, typename CommonType>
                struct distance_cast<ToDistance, Ratio, CommonType, true, false>
                {
                    template <typename Rep, typename Length>
                    static constexpr ToDistance cast(distance<Rep, Length> from)
                    {
                        return ToDistance{
                            static_cast<typename ToDistance::rep>(static_cast<CommonType>(from.count())
                                                                  * static_cast<CommonType>(Ratio::den))};
                    }
                };

                template <typename ToDistance, typename Ratio, typename CommonType>
                struct distance_cast<ToDistance, Ratio, CommonType, false, true>
                {
                    template <typename Rep, typename Length>
                    static constexpr ToDistance cast(distance<Rep, Length> from)
                    {
                        return ToDistance{
                            static_cast<typename ToDistance::rep>(static_cast<CommonType>(from.count())
                                                                  / static_cast<CommonType>(Ratio::num))};
                    }
                };
            }

            template <typename ToDistance, typename Rep, typename Length>
            constexpr auto distance_cast(distance<Rep, Length> from)
                -> typename std::enable_if<true, ToDistance>::type
            {
                using ToLength   = typename ToDistance::length;
                using ToRep      = typename ToDistance::rep;
                using Ratio      = std::ratio_divide<ToLength, Length>;
                using CommonType = typename std::common_type<ToRep, Rep, std::intmax_t>::type;

                return detail::distance_cast<ToDistance, Ratio, CommonType, Ratio::num == 1, Ratio::den == 1>::cast(from);
            }
        }
    }

    namespace literals
    {
        namespace distance_literals
        {
            constexpr std::units::si::metres operator"" m(unsigned long long int dist)
            {
                return std::units::si::metres{static_cast<std::units::si::metres::rep>(dist)};
            }

            constexpr std::units::si::kilometres operator"" km(unsigned long long int dist)
            {
                return std::units::si::kilometres{static_cast<std::units::si::kilometres::rep>(dist)};
            }
        }
    }
}
