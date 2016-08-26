#pragma once

#include <ratio>

namespace std
{
    namespace units
    {
        namespace si
        {
            template <typename Rep, typename Length>
            struct distance;

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
                        using ToRep = typename ToDistance::rep;
                        return ToDistance{
                            static_cast<ToRep>(static_cast<CommonType>(from.count())
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
                        using ToRep = typename ToDistance::rep;
                        return ToDistance{static_cast<ToRep>(static_cast<CommonType>(from.count()))};
                    }
                };

                template <typename ToDistance, typename Ratio, typename CommonType>
                struct distance_cast<ToDistance, Ratio, CommonType, true, false>
                {
                    template <typename Rep, typename Length>
                    static constexpr ToDistance cast(distance<Rep, Length> from)
                    {
                        using ToRep = typename ToDistance::rep;
                        return ToDistance{
                            static_cast<ToRep>(static_cast<CommonType>(from.count())
                                               * static_cast<CommonType>(Ratio::den))};
                    }
                };

                template <typename ToDistance, typename Ratio, typename CommonType>
                struct distance_cast<ToDistance, Ratio, CommonType, false, true>
                {
                    template <typename Rep, typename Length>
                    static constexpr ToDistance cast(distance<Rep, Length> from)
                    {
                        using ToRep = typename ToDistance::rep;
                        return ToDistance{
                            static_cast<ToRep>(static_cast<CommonType>(from.count())
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
                using CommonType = typename std::common_type<ToRep, Rep, std::intmax_t>::type;
                using Ratio      = std::ratio_divide<ToLength, Length>;

                return detail::distance_cast<ToDistance,
                                             Ratio,
                                             CommonType,
                                             Ratio::num == 1,
                                             Ratio::den == 1>::cast(from);
            }

            template <typename Rep, typename Length = std::ratio<1>>
            struct distance
            {
                using rep    = Rep;
                using length = Length;

                constexpr explicit distance(rep value)
                    : value{value}
                {
                }

                template <typename Rep2, typename Length2>
                constexpr distance(distance<Rep2, Length2> dist)
                    : value{distance_cast<distance>(dist).count()}
                {
                }

                constexpr rep count() const
                {
                    return value;
                }

                distance& operator++()
                {
                    ++value;
                    return *this;
                }

                distance operator++(int)
                {
                    return distance{value + 1};
                }

                distance& operator--()
                {
                    --value;
                    return *this;
                }

                distance operator--(int)
                {
                    return distance{value - 1};
                }

                distance& operator+=(distance const other)
                {
                    value += other.count();
                    return *this;
                }

                distance& operator-=(distance const other)
                {
                    value -= other.count();
                    return *this;
                }

                distance& operator*=(rep const scalar)
                {
                    value *= scalar;
                    return *this;
                }

                distance& operator/=(rep const scalar)
                {
                    value /= scalar;
                    return *this;
                }

                distance& operator%=(rep const scalar)
                {
                    value %= scalar;
                    return *this;
                }

                distance& operator%=(distance const other)
                {
                    value %= other.count();
                    return *this;
                }

            private:
                rep value;
            };

            using nanometres  = distance<long int, std::nano>;
            using micrometres = distance<long int, std::micro>;
            using millimetres = distance<long int, std::milli>;
            using centimetres = distance<long int, std::centi>;
            using decimetres  = distance<long int, std::deci>;
            using metres      = distance<long int>;
            using kilometres  = distance<long int, std::kilo>;

            // American spellings
            using nanometers  = nanometres;
            using micrometers = micrometres;
            using millimeters = millimetres;
            using centimeters = centimetres;
            using decimeters  = decimetres;
            using meters      = metres;
            using kilometers  = kilometres;
        }

        // Non SI units that are interoperable with SI units
        using yards = si::distance<double, std::ratio<9144, 10000>>;
    }

    inline namespace literals
    {
        namespace distance_literals
        {
            constexpr std::units::si::nanometres operator"" _nm(unsigned long long int dist)
            {
                return std::units::si::nanometres{static_cast<std::units::si::nanometres::rep>(dist)};
            }

            constexpr std::units::si::micrometres operator"" _um(unsigned long long int dist)
            {
                return std::units::si::micrometres{static_cast<std::units::si::micrometres::rep>(dist)};
            }

            constexpr std::units::si::millimetres operator"" _mm(unsigned long long int dist)
            {
                return std::units::si::millimetres{static_cast<std::units::si::millimetres::rep>(dist)};
            }

            constexpr std::units::si::centimetres operator"" _cm(unsigned long long int dist)
            {
                return std::units::si::centimetres{static_cast<std::units::si::centimetres::rep>(dist)};
            }

            constexpr std::units::si::decimetres operator"" _dm(unsigned long long int dist)
            {
                return std::units::si::decimetres{static_cast<std::units::si::decimetres::rep>(dist)};
            }

            constexpr std::units::si::metres operator"" _m(unsigned long long int dist)
            {
                return std::units::si::metres{static_cast<std::units::si::metres::rep>(dist)};
            }

            constexpr std::units::si::kilometres operator"" _km(unsigned long long int dist)
            {
                return std::units::si::kilometres{static_cast<std::units::si::kilometres::rep>(dist)};
            }

        }
    }
}
