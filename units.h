#pragma once

#include <ratio>
#include <type_traits>
#include <cstdint>

namespace units
{
	template <typename Rep, typename Length>
	struct distance;

	namespace detail
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
}

namespace std
{
	template <typename CommonRep, typename Length1, typename Length2>
	struct distance_common_type
	{
	private:
		using gcd_num    = units::detail::greatest_common_divisor<Length1::num, Length2::num>;
		using gcd_den    = units::detail::greatest_common_divisor<Length1::den, Length2::den>;
		using common_rep = typename CommonRep::type;
		using ratio      = std::ratio<gcd_num::value, (Length1::den / gcd_den::value) * Length2::den>;

	public:
		using type = units::distance<common_rep, ratio>;
	};

	template <typename Rep1, typename Length1, typename Rep2, typename Length2>
	struct common_type<units::distance<Rep1, Length1>,
	                   units::distance<Rep2, Length2>>
	{
		using type = typename distance_common_type<std::common_type<Rep1, Rep2>,
		                                           Length1,
		                                           Length2>::type;
	};
}

namespace units
{
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
		using CommonType = typename std::common_type<ToRep, Rep, intmax_t>::type;
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
			value = fmod(value, scalar);
			return *this;
		}
		distance& operator%=(distance const other)
		{
			value = fmod(value, other.count());
			return *this;
		}

	private:
		rep value;
	};

	// Useful aliases
	using nanometres  = distance<double, std::nano>;
	using micrometres = distance<double, std::micro>;
	using millimetres = distance<double, std::milli>;
	using centimetres = distance<double, std::centi>;
	using decimetres  = distance<double, std::deci>;
	using metres      = distance<double>;
	using kilometres  = distance<double, std::kilo>;

	// American spellings
	using nanometers  = nanometres;
	using micrometers = micrometres;
	using millimeters = millimetres;
	using centimeters = centimetres;
	using decimeters  = decimetres;
	using meters      = metres;
	using kilometers  = kilometres;

	// Imperial
	using feet     = distance<double, std::ratio<3048, 10000>>;
	using yards    = distance<double, std::ratio_multiply<std::ratio<3>, feet::length>>;
	using chains   = distance<double, std::ratio_multiply<std::ratio<66>, feet::length>>;
	using furlongs = distance<double, std::ratio_multiply<std::ratio<660>, feet::length>>;
	using miles    = distance<double, std::ratio_multiply<std::ratio<5280>, feet::length>>;
	using league   = distance<double, std::ratio_multiply<std::ratio<15840>, feet::length>>;
	using inches   = distance<double, std::ratio_divide<feet::length, std::ratio<12>>>;
	using thou     = distance<double, std::ratio_divide<feet::length, std::ratio<12000>>>;

	// Maritime
	using fathom        = distance<double, std::ratio_multiply<std::ratio<608, 100>, feet::length>>;
	using cable         = distance<double, std::ratio_multiply<std::ratio<608>, feet::length>>;
	using nautical_mile = distance<double, std::ratio_multiply<std::ratio<6080>, feet::length>>;

	// Astronimical Units
	using earth_radii        = distance<double, std::ratio_multiply<std::ratio<6371>, kilometres::length>>;
	using lunar_distances    = distance<double, std::ratio_multiply<std::ratio<384402>, kilometres::length>>;
	using astronimical_units = distance<double, std::ratio<149597870700>>;
	using light_years        = distance<double, std::ratio_multiply<std::ratio<94607304725808, 10>, kilometres::length>>;
	using parsecs            = distance<double, std::ratio_multiply<std::ratio<308567758146719, 10>, kilometres::length>>;

	// Arithmetic operations
	template <
	    typename Rep1,
	    typename Length1,
	    typename Rep2,
	    typename Length2>
	constexpr auto operator+(distance<Rep1, Length1> lhs, distance<Rep2, Length2> rhs)
	    -> typename std::common_type<distance<Rep1, Length1>, distance<Rep2, Length2>>::type
	{
		using distance1   = distance<Rep1, Length1>;
		using distance2   = distance<Rep2, Length2>;
		using common_type = typename std::common_type<distance1, distance2>::type;

		return static_cast<common_type>(static_cast<common_type>(lhs).count()
		                                + static_cast<common_type>(rhs).count());
	}

	template <
	    typename Rep1,
	    typename Length1,
	    typename Rep2,
	    typename Length2>
	constexpr auto operator-(distance<Rep1, Length1> lhs, distance<Rep2, Length2> rhs)
	    -> typename std::common_type<distance<Rep1, Length1>, distance<Rep2, Length2>>::type
	{
		using distance1   = distance<Rep1, Length1>;
		using distance2   = distance<Rep2, Length2>;
		using common_type = typename std::common_type<distance1, distance2>::type;

		return static_cast<common_type>(static_cast<common_type>(lhs).count()
		                                - static_cast<common_type>(rhs).count());
	}

	template <
	    typename Rep1,
	    typename Length,
	    typename Rep2>
	constexpr auto operator*(distance<Rep1, Length> lhs, Rep2 const scalar)
	    -> distance<typename std::common_type<Rep1, Rep2>::type, Length>
	{
		using result_type = distance<typename std::common_type<Rep1, Rep2>::type, Length>;
		return static_cast<result_type>(static_cast<result_type>(lhs).count() * scalar);
	}

	template <
	    typename Rep1,
	    typename Rep2,
	    typename Length>
	constexpr auto operator*(Rep1 const scalar, distance<Rep2, Length> rhs)
	    -> distance<typename std::common_type<Rep1, Rep2>::type, Length>
	{
		using result_type = distance<typename std::common_type<Rep1, Rep2>::type, Length>;
		return rhs * scalar;
	}

	template <
	    typename Rep1,
	    typename Length,
	    typename Rep2>
	constexpr auto operator/(distance<Rep1, Length> lhs, Rep2 const scalar)
	    -> distance<typename std::common_type<Rep1, Rep2>::type, Length>
	{
		using result_type = distance<typename std::common_type<Rep1, Rep2>::type, Length>;
		return static_cast<result_type>(static_cast<result_type>(lhs).count() / scalar);
	}

	template <
	    typename Rep1,
	    typename Length1,
	    typename Rep2,
	    typename Length2>
	constexpr auto operator%(distance<Rep1, Length1> lhs, distance<Rep2, Length2> rhs)
	    -> typename std::common_type<distance<Rep1, Length1>, distance<Rep2, Length2>>::type
	{
		using distance1   = distance<Rep1, Length1>;
		using distance2   = distance<Rep2, Length2>;
		using common_type = typename std::common_type<distance1, distance2>::type;

		return static_cast<common_type>(fmod(static_cast<common_type>(lhs).count(),
		                                static_cast<common_type>(rhs).count()));
	}

	template <
	    typename Rep1,
	    typename Length,
	    typename Rep2>
	constexpr auto operator%(distance<Rep1, Length> lhs, Rep2 const scalar)
	    -> distance<typename std::common_type<Rep1, Rep2>::type, Length>
	{
		using result_type = distance<typename std::common_type<Rep1, Rep2>::type, Length>;
		return static_cast<result_type>(fmod(static_cast<result_type>(lhs).count(), scalar));
	}
}

inline namespace literals
{
	namespace distance_literals
	{
		constexpr units::nanometres operator"" _nm(unsigned long long int dist)
		{
			return units::nanometres{static_cast<units::nanometres::rep>(dist)};
		}

		constexpr units::micrometres operator"" _um(unsigned long long int dist)
		{
			return units::micrometres{static_cast<units::micrometres::rep>(dist)};
		}

		constexpr units::millimetres operator"" _mm(unsigned long long int dist)
		{
			return units::millimetres{static_cast<units::millimetres::rep>(dist)};
		}

		constexpr units::centimetres operator"" _cm(unsigned long long int dist)
		{
			return units::centimetres{static_cast<units::centimetres::rep>(dist)};
		}

		constexpr units::decimetres operator"" _dm(unsigned long long int dist)
		{
			return units::decimetres{static_cast<units::decimetres::rep>(dist)};
		}

		constexpr units::metres operator"" _m(unsigned long long int dist)
		{
			return units::metres{static_cast<units::metres::rep>(dist)};
		}

		constexpr units::kilometres operator"" _km(unsigned long long int dist)
		{
			return units::kilometres{static_cast<units::kilometres::rep>(dist)};
		}

		constexpr units::inches operator"" _in(unsigned long long int dist)
		{
			return units::inches{static_cast<units::inches::rep>(dist)};
		}
	}
}
