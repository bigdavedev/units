#pragma once

/**
 * MIT License
 *
 * Copyright (c) 2016-2017 David Brown
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <ratio>
#include <stdexcept>
#include <type_traits>

#include <cmath>
#include <cstdint>

namespace units
{
	template <typename Rep, typename Length>
	struct distance;

	template <typename T>
	struct is_distance : std::false_type
	{
	};

	template <typename Rep, typename Length>
	struct is_distance<distance<Rep, Length>>
	    : std::true_type
	{
	};

	namespace detail
	{
		template <intmax_t Numerator>
		struct integer_sign
		    : std::integral_constant<intmax_t, (Numerator < 0) ? -1 : 1>
		{
		};

		template <intmax_t Numerator>
		struct integer_abs
		    : std::integral_constant<intmax_t, Numerator * integer_sign<Numerator>::value>
		{
		};

		template <intmax_t Numerator, intmax_t Quotient>
		struct greatest_common_divisor;

		template <intmax_t Numerator, intmax_t Quotient>
		struct greatest_common_divisor
		    : greatest_common_divisor<Quotient, (Numerator % Quotient)>
		{
		};

		template <intmax_t Numerator>
		struct greatest_common_divisor<Numerator, 0>
		    : std::integral_constant<intmax_t, integer_sign<Numerator>::value>
		{
		};

		template <intmax_t Quotient>
		struct greatest_common_divisor<0, Quotient>
		    : std::integral_constant<intmax_t, integer_sign<Quotient>::value>
		{
		};

		template <typename T>
		auto fmod(T x, T y)
		    ->typename std::enable_if<std::is_floating_point<T>::value, long long int>::type
		{
			return y != 0 ? x - std::trunc(x / y) * y : throw std::domain_error{"Dividing by zero!"};
		};

		template <class CommonRep,
		          class Length,
		          class Rep2,
		          bool = std::is_convertible<Rep2, CommonRep>::value>
		struct distance_div_mod_base
		{ // return type for distance / rep and distance % rep
			using type = units::distance<CommonRep, Length>;
		};

		template <class CommonRep,
		          class Length,
		          class Rep2>
		struct distance_div_mod_base<CommonRep, Length, Rep2, false>
		{ // no return type
		};

		template <class Rep1,
		          class Length,
		          class Rep2,
		          bool = is_distance<Rep2>::value>
		struct distance_div_mod
		{ // no return type
		};

		template <class Rep1,
		          class Length,
		          class Rep2>
		struct distance_div_mod<Rep1, Length, Rep2, false>
		    : distance_div_mod_base<typename std::common_type<Rep1, Rep2>::type,
		                            Length,
		                            Rep2>
		{ // return type for distance / rep and distance % rep
		};
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
	template <typename ToDistance, typename Rep, typename Length>
	constexpr auto distance_cast(distance<Rep, Length> from)
	    -> typename std::enable_if<is_distance<ToDistance>::value, ToDistance>::type;

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
		    : value{distance_cast<distance<rep, length>>(dist).count()}
		{
		}

		constexpr rep count() const;

		distance& operator++();
		distance operator++(int);
		distance& operator--();
		distance operator--(int);

		distance& operator+=(distance const other);
		distance& operator-=(distance const other);
		distance& operator*=(rep const scalar);
		distance& operator/=(rep const scalar);
		distance& operator%=(rep const scalar);
		distance& operator%=(distance const other);

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
	using leagues  = distance<double, std::ratio_multiply<std::ratio<15840>, feet::length>>;
	using inches   = distance<double, std::ratio_divide<feet::length, std::ratio<12>>>;
	using thous    = distance<double, std::ratio_divide<feet::length, std::ratio<12000>>>;

	// Maritime
	using fathoms        = distance<double, std::ratio_multiply<std::ratio<608, 100>, feet::length>>;
	using cables         = distance<double, std::ratio_multiply<std::ratio<608>, feet::length>>;
	using nautical_miles = distance<double, std::ratio_multiply<std::ratio<6080>, feet::length>>;

	// Astronimical Units
	//
	// Unfortunately, std::ratio uses std::intmax_t which does not have enough precision for defining a hubble length
	// which is 14.4 billion lightyears.
	using earth_radii        = distance<long double, std::ratio_multiply<std::ratio<6371>, kilometres::length>>;
	using lunar_distances    = distance<long double, std::ratio_multiply<std::ratio<384402>, kilometres::length>>;
	using astronimical_units = distance<long double, std::ratio<149597870700>>;
	using light_years        = distance<long double, std::ratio_multiply<std::ratio<94607304725808, 10>, kilometres::length>>;
	using parsecs            = distance<long double, std::ratio_multiply<std::ratio<308567758146719, 10>, kilometres::length>>;

	// Arithmetic operations
	template <typename Rep1, typename Length1, typename Rep2, typename Length2>
	constexpr auto operator+(distance<Rep1, Length1> lhs, distance<Rep2, Length2> rhs)
	    -> typename std::common_type<distance<Rep1, Length1>, distance<Rep2, Length2>>::type;

	template <typename Rep1, typename Length1, typename Rep2, typename Length2>
	constexpr auto operator-(distance<Rep1, Length1> lhs, distance<Rep2, Length2> rhs)
	    -> typename std::common_type<distance<Rep1, Length1>, distance<Rep2, Length2>>::type;

	template <typename Rep1, typename Length, typename Rep2>
	constexpr auto operator*(distance<Rep1, Length> lhs, Rep2 const scalar)
	    -> distance<typename std::common_type<Rep1, Rep2>::type, Length>;

	template <typename Rep1, typename Rep2, typename Length>
	constexpr auto operator*(Rep1 const scalar, distance<Rep2, Length> rhs)
	    -> distance<typename std::common_type<Rep1, Rep2>::type, Length>;

	template <typename Rep1, typename Length, typename Rep2>
	constexpr auto operator/(distance<Rep1, Length> lhs, Rep2 const scalar)
	    -> distance<typename std::common_type<Rep1, Rep2>::type, Length>;

	template <typename Rep1, typename Length1, typename Rep2, typename Length2>
	constexpr auto operator%(distance<Rep1, Length1> lhs, distance<Rep2, Length2> rhs)
	    -> typename std::common_type<distance<Rep1, Length1>, distance<Rep2, Length2>>::type;

	template <typename Rep1, typename Length, typename Rep2>
	constexpr auto operator%(distance<Rep1, Length> lhs, Rep2 const scalar)
	    -> typename detail::distance_div_mod<Rep1, Length, Rep2>::type;
}

inline namespace literals
{
	namespace distance_literals
	{
		// Metric
		constexpr units::nanometres operator"" _nm(unsigned long long int dist);
		constexpr units::micrometres operator"" _um(unsigned long long int dist);
		constexpr units::millimetres operator"" _mm(unsigned long long int dist);
		constexpr units::centimetres operator"" _cm(unsigned long long int dist);
		constexpr units::decimetres operator"" _dm(unsigned long long int dist);
		constexpr units::metres operator"" _m(unsigned long long int dist);
		constexpr units::kilometres operator"" _km(unsigned long long int dist);

		// Imperial
		constexpr units::inches operator"" _in(unsigned long long int dist);
		constexpr units::feet operator"" _ft(unsigned long long int dist);
		constexpr units::yards operator"" _yd(unsigned long long int dist);
		constexpr units::chains operator"" _ch(unsigned long long int dist);
		constexpr units::furlongs operator"" _fur(unsigned long long int dist);
		constexpr units::miles operator"" _mi(unsigned long long int dist);
		constexpr units::leagues operator"" _lea(unsigned long long int dist);
		constexpr units::thous operator"" _th(unsigned long long int dist);

		// Maritime
		constexpr units::fathoms operator"" _ftm(unsigned long long int dist);

		// Astronomical Units
		constexpr units::earth_radii operator"" _R(unsigned long long int dist);
		constexpr units::lunar_distances operator"" _LD(unsigned long long int dist);
		constexpr units::astronimical_units operator"" _AU(unsigned long long int dist);
		constexpr units::light_years operator"" _ly(unsigned long long int dist);
		constexpr units::parsecs operator"" _pc(unsigned long long int dist);
	}
}

namespace units
{
	// Implementation
	template <typename Rep, typename Length>
	constexpr typename distance<Rep, Length>::rep distance<Rep, Length>::count() const
	{
		return value;
	}

	template <typename Rep, typename Length>
	distance<Rep, Length>& distance<Rep, Length>::operator++()
	{
		++value;
		return *this;
	}

	template <typename Rep, typename Length>
	distance<Rep, Length> distance<Rep, Length>::operator++(int)
	{
		auto const temp = value;
		++value;
		return distance<Rep, Length>{temp};
	}

	template <typename Rep, typename Length>
	distance<Rep, Length>& distance<Rep, Length>::operator--()
	{
		--value;
		return *this;
	}

	template <typename Rep, typename Length>
	distance<Rep, Length> distance<Rep, Length>::operator--(int)
	{
		auto const temp = value;
		--value;
		return distance<Rep, Length>{temp};
	}

	template <typename Rep, typename Length>
	distance<Rep, Length>& distance<Rep, Length>::operator+=(distance const other)
	{
		value += other.count();
		return *this;
	}

	template <typename Rep, typename Length>
	distance<Rep, Length>& distance<Rep, Length>::operator-=(distance const other)
	{
		value -= other.count();
		return *this;
	}

	template <typename Rep, typename Length>
	distance<Rep, Length>& distance<Rep, Length>::operator*=(rep const scalar)
	{
		value *= scalar;
		return *this;
	}

	template <typename Rep, typename Length>
	distance<Rep, Length>& distance<Rep, Length>::operator/=(rep const scalar)
	{
		value /= scalar;
		return *this;
	}

	template <typename Rep, typename Length>
	distance<Rep, Length>& distance<Rep, Length>::operator%=(rep const scalar)
	{
		value = detail::fmod(value, scalar);
		return *this;
	}

	template <typename Rep, typename Length>
	distance<Rep, Length>& distance<Rep, Length>::operator%=(distance const other)
	{
		value = detail::fmod(value, other.count());
		return *this;
	}

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

		return common_type{common_type{lhs}.count() % common_type{rhs}.count()};
	}

	template <
	    typename Rep1,
	    typename Length,
	    typename Rep2>
	constexpr auto operator%(distance<Rep1, Length> lhs, Rep2 const scalar)
	    -> typename detail::distance_div_mod<Rep1, Length, Rep2>::type
	{
		using result_type = distance<typename std::common_type<Rep1, Rep2>::type, Length>;
		return result_type{result_type{lhs}.count() % result_type{static_cast<typename result_type::rep>(scalar)}.count()};
	}

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
	    -> typename std::enable_if<is_distance<ToDistance>::value, ToDistance>::type
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
}

inline namespace literals
{
	namespace distance_literals
	{
		// Metric
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

		// Imperial
		constexpr units::inches operator"" _in(unsigned long long int dist)
		{
			return units::inches{static_cast<units::inches::rep>(dist)};
		}

		constexpr units::feet operator"" _ft(unsigned long long int dist)
		{
			return units::feet{static_cast<units::feet::rep>(dist)};
		}

		constexpr units::yards operator"" _yd(unsigned long long int dist)
		{
			return units::yards{static_cast<units::yards::rep>(dist)};
		}

		constexpr units::chains operator"" _ch(unsigned long long int dist)
		{
			return units::chains{static_cast<units::chains::rep>(dist)};
		}

		constexpr units::furlongs operator"" _fur(unsigned long long int dist)
		{
			return units::furlongs{static_cast<units::furlongs::rep>(dist)};
		}

		constexpr units::miles operator"" _mi(unsigned long long int dist)
		{
			return units::miles{static_cast<units::miles::rep>(dist)};
		}

		constexpr units::leagues operator"" _lea(unsigned long long int dist)
		{
			return units::leagues{static_cast<units::leagues::rep>(dist)};
		}

		constexpr units::thous operator"" _th(unsigned long long int dist)
		{
			return units::thous{static_cast<units::thous::rep>(dist)};
		}

		// Maritime
		constexpr units::fathoms operator"" _ftm(unsigned long long int dist)
		{
			return units::fathoms{static_cast<units::fathoms::rep>(dist)};
		}

		// Astronomical Units
		constexpr units::earth_radii operator"" _R(unsigned long long int dist)
		{
			return units::earth_radii{static_cast<units::earth_radii::rep>(dist)};
		}

		constexpr units::lunar_distances operator"" _LD(unsigned long long int dist)
		{
			return units::lunar_distances{static_cast<units::lunar_distances::rep>(dist)};
		}

		constexpr units::astronimical_units operator"" _AU(unsigned long long int dist)
		{
			return units::astronimical_units{static_cast<units::astronimical_units::rep>(dist)};
		}

		constexpr units::light_years operator"" _ly(unsigned long long int dist)
		{
			return units::light_years{static_cast<units::light_years::rep>(dist)};
		}

		constexpr units::parsecs operator"" _pc(unsigned long long int dist)
		{
			return units::parsecs{static_cast<units::parsecs::rep>(dist)};
		}
	}
}