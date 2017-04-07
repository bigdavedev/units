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
#include <numeric>

#include <cmath>
#include <cstdint>

namespace units
{
	template <typename Rep, typename Ratio, typename UnitType>
	struct unit;

	template <typename T>
	struct is_distance : std::false_type
	{
	};

	template <typename Rep, typename Ratio, typename UnitType>
	struct is_distance<unit<Rep, Ratio, UnitType>>
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
		    : std::integral_constant<intmax_t, integer_abs<Numerator>::value>
		{
		};

		template <intmax_t Quotient>
		struct greatest_common_divisor<0, Quotient>
		    : std::integral_constant<intmax_t, integer_abs<Quotient>::value>
		{
		};

		template <typename T>
		auto fmod(T x, T y)
		    -> typename std::enable_if<std::is_floating_point<T>::value, long long int>::type
		{
			return y != 0 ? x - std::trunc(x / y) * y : throw std::domain_error{"Dividing by zero!"};
		};

		template <class CommonRep,
		          class Ratio,
		          class UnitType,
		          class Rep2,
		          bool = std::is_convertible<Rep2, CommonRep>::value>
		struct distance_div_mod_base
		{ // return type for unit / rep and unit % rep
			using type = units::unit<CommonRep, Ratio, UnitType>;
		};

		template <class CommonRep,
		          class Ratio,
		          class UnitType,
		          class Rep2>
		struct distance_div_mod_base<CommonRep, Ratio, UnitType, Rep2, false>
		{ // no return type
		};

		template <class Rep1,
		          class Ratio1,
		          class UnitType1,
		          class Rep2,
		          bool = is_distance<Rep2>::value>
		struct distance_div_mod
		{ // no return type
		};

		template <class Rep1,
		          class Ratio1,
		          class UnitType1,
		          class Rep2>
		struct distance_div_mod<Rep1, Ratio1, UnitType1, Rep2, false>
		    : distance_div_mod_base<typename std::common_type<Rep1, Rep2>::type,
		                            Ratio1,
		                            UnitType1,
		                            Rep2>
		{ // return type for unit / rep and unit % rep
		};
	}
}

namespace std
{
	template <typename CommonRep, typename Ratio1, typename Ratio2, typename UnitType>
	struct distance_common_type
	{
	private:
		using gcd_num    = units::detail::greatest_common_divisor<Ratio1::num, Ratio2::num>;
		using gcd_den    = units::detail::greatest_common_divisor<Ratio1::den, Ratio2::den>;
		using common_rep = typename CommonRep::type;
		using ratio      = std::ratio<gcd_num::value, (Ratio1::den / gcd_den::value) * Ratio2::den>;
		using unit_type  = UnitType;

	public:
		using type = units::unit<common_rep, ratio, unit_type>;
	};

	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	struct common_type<units::unit<Rep1, Ratio1, UnitType1>,
	                   units::unit<Rep2, Ratio2, UnitType2>>
	{
		static_assert(std::is_same<UnitType1, UnitType2>::value, "Incompatible unit types");
		using type = typename distance_common_type<std::common_type<Rep1, Rep2>,
		                                           Ratio1,
		                                           Ratio2,
		                                           UnitType1>::type;
	};
}

namespace units
{
	namespace unit_type
	{
		struct distance
		{
		};
	}

	template <typename ToDistance, typename Rep, typename Ratio, typename UnitType>
	constexpr auto unit_cast(unit<Rep, Ratio, UnitType> from)
	    -> typename std::enable_if<is_distance<ToDistance>::value, ToDistance>::type;

	template <typename Rep, typename Ratio, typename UnitType>
	struct unit
	{
		using rep       = Rep;
		using ratio     = Ratio;
		using unit_type = UnitType;

		constexpr explicit unit(rep value)
		    : value{value}
		{
		}

		template <typename Rep2, typename Ratio2, typename UnitType2>
		constexpr unit(unit<Rep2, Ratio2, UnitType2> dist)
		    : value{unit_cast<unit<rep, ratio, unit_type>>(dist).count()}
		{
			static_assert(std::is_same<unit_type, UnitType2>::value, "Unit types are not compatible");
		}

		constexpr rep count() const;

		constexpr std::common_type_t<unit> operator+() const;
		constexpr std::common_type_t<unit> operator-() const;

		unit& operator++();
		unit operator++(int);
		unit& operator--();
		unit operator--(int);

		unit& operator+=(unit const other);
		unit& operator-=(unit const other);
		unit& operator*=(rep const scalar);
		unit& operator/=(rep const scalar);
		unit& operator%=(rep const scalar);
		unit& operator%=(unit const other);

	private:
		rep value;
	};

	template <typename Rep, typename Ratio = std::ratio<1>>
	using distance = unit<Rep, Ratio, unit_type::distance>;

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
	using feet     = distance<double, std::ratio_multiply<std::ratio<381, 1250>, metres::ratio>>;
	using thous    = distance<double, std::ratio_divide<feet::ratio, std::ratio<12000>>>;
	using inches   = distance<double, std::ratio_divide<feet::ratio, std::ratio<12>>>;
	using links    = distance<double, std::ratio_multiply<std::ratio<33, 50>, feet::ratio>>;
	using yards    = distance<double, std::ratio_multiply<std::ratio<3>, feet::ratio>>;
	using rods     = distance<double, std::ratio_multiply<std::ratio<25>, links::ratio>>;
	using chains   = distance<double, std::ratio_multiply<std::ratio<4>, rods::ratio>>;
	using furlongs = distance<double, std::ratio_multiply<std::ratio<10>, chains::ratio>>;
	using miles    = distance<double, std::ratio_multiply<std::ratio<5280>, feet::ratio>>;
	using leagues  = distance<double, std::ratio_multiply<std::ratio<15840>, feet::ratio>>;

	// Maritime
	using fathoms        = distance<double, std::ratio_multiply<std::ratio<608, 100>, feet::ratio>>;
	using cables         = distance<double, std::ratio_multiply<std::ratio<608>, feet::ratio>>;
	using nautical_miles = distance<double, std::ratio_multiply<std::ratio<6080>, feet::ratio>>;

	// Astronimical Units
	//
	// Unfortunately, std::ratio uses std::intmax_t which does not have enough precision for defining a hubble ratio
	// which is 14.4 billion lightyears.
	using earth_radii        = distance<long double, std::ratio_multiply<std::ratio<6371>, kilometres::ratio>>;
	using lunar_distances    = distance<long double, std::ratio_multiply<std::ratio<384402>, kilometres::ratio>>;
	using astronimical_units = distance<long double, std::ratio<149597870700>>;
	using light_years        = distance<long double, std::ratio_multiply<std::ratio<94607304725808, 10>, kilometres::ratio>>;
	using parsecs            = distance<long double, std::ratio_multiply<std::ratio<308567758146719, 10>, kilometres::ratio>>;

	// Arithmetic operations
	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	constexpr auto operator+(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	    -> typename std::common_type<unit<Rep1, Ratio1, UnitType1>, unit<Rep2, Ratio2, UnitType2>>::type;

	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	constexpr auto operator-(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	    -> typename std::common_type<unit<Rep1, Ratio1, UnitType1>, unit<Rep2, Ratio2, UnitType2>>::type;

	template <typename Rep1, typename Ratio, typename UnitType, typename Rep2>
	constexpr auto operator*(unit<Rep1, Ratio, UnitType> lhs, Rep2 const scalar)
	    -> unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>;

	template <typename Rep1, typename Rep2, typename Ratio, typename UnitType>
	constexpr auto operator*(Rep1 const scalar, unit<Rep2, Ratio, UnitType> rhs)
	    -> unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>;

	template <typename Rep1, typename Ratio, typename UnitType, typename Rep2>
	constexpr auto operator/(unit<Rep1, Ratio, UnitType> lhs, Rep2 const scalar)
	    -> unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>;

	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	constexpr auto operator%(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	    -> typename std::common_type<unit<Rep1, Ratio1, UnitType1>, unit<Rep2, Ratio2, UnitType2>>::type;

	template <typename Rep1, typename Ratio, typename UnitType, typename Rep2>
	constexpr auto operator%(unit<Rep1, Ratio, UnitType> lhs, Rep2 const scalar)
	    -> typename detail::distance_div_mod<Rep1, Ratio, UnitType, Rep2>::type;

	// Relational operations
	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	constexpr bool operator==(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs);

	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	constexpr bool operator!=(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs);

	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	constexpr bool operator<(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs);

	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	constexpr bool operator<=(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs);

	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	constexpr bool operator>(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs);

	template <typename Rep1, typename Ratio1, typename UnitType1, typename Rep2, typename Ratio2, typename UnitType2>
	constexpr bool operator>=(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs);
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
		constexpr units::thous operator"" _th(unsigned long long int dist);
		constexpr units::inches operator"" _in(unsigned long long int dist);
		constexpr units::links operator"" _li(unsigned long long int dist);
		constexpr units::feet operator"" _ft(unsigned long long int dist);
		constexpr units::yards operator"" _yd(unsigned long long int dist);
		constexpr units::rods operator"" _rd(unsigned long long int dist);
		constexpr units::chains operator"" _ch(unsigned long long int dist);
		constexpr units::furlongs operator"" _fur(unsigned long long int dist);
		constexpr units::miles operator"" _mi(unsigned long long int dist);
		constexpr units::leagues operator"" _lea(unsigned long long int dist);

		// Maritime
		constexpr units::fathoms operator"" _ftm(unsigned long long int dist);
		constexpr units::cables operator"" _cb(unsigned long long int dist);
		constexpr units::nautical_miles operator"" _NM(unsigned long long int dist);
		constexpr units::nautical_miles operator"" _nmi(unsigned long long int dist);

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
	template <typename Rep, typename Ratio, typename UnitType>
	constexpr typename unit<Rep, Ratio, UnitType>::rep unit<Rep, Ratio, UnitType>::count() const
	{
		return value;
	}

	template <typename Rep, typename Ratio, typename UnitType>
	constexpr std::common_type_t<unit<Rep, Ratio, UnitType>> unit<Rep, Ratio, UnitType>::operator+() const
	{
		return (*this);
	}

	template <typename Rep, typename Ratio, typename UnitType>
	constexpr std::common_type_t<unit<Rep, Ratio, UnitType>> unit<Rep, Ratio, UnitType>::operator-() const
	{
		return unit<Rep, Ratio, UnitType>{0 - value};
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType>& unit<Rep, Ratio, UnitType>::operator++()
	{
		++value;
		return *this;
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType> unit<Rep, Ratio, UnitType>::operator++(int)
	{
		auto const temp = value;
		++value;
		return unit<Rep, Ratio, UnitType>{temp};
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType>& unit<Rep, Ratio, UnitType>::operator--()
	{
		--value;
		return *this;
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType> unit<Rep, Ratio, UnitType>::operator--(int)
	{
		auto const temp = value;
		--value;
		return unit<Rep, Ratio, UnitType>{temp};
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType>& unit<Rep, Ratio, UnitType>::operator+=(unit const other)
	{
		value += other.count();
		return *this;
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType>& unit<Rep, Ratio, UnitType>::operator-=(unit const other)
	{
		value -= other.count();
		return *this;
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType>& unit<Rep, Ratio, UnitType>::operator*=(rep const scalar)
	{
		value *= scalar;
		return *this;
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType>& unit<Rep, Ratio, UnitType>::operator/=(rep const scalar)
	{
		value /= scalar;
		return *this;
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType>& unit<Rep, Ratio, UnitType>::operator%=(rep const scalar)
	{
		value = detail::fmod(value, scalar);
		return *this;
	}

	template <typename Rep, typename Ratio, typename UnitType>
	unit<Rep, Ratio, UnitType>& unit<Rep, Ratio, UnitType>::operator%=(unit const other)
	{
		value = detail::fmod(value, other.count());
		return *this;
	}

	// Arithmetic operations
	template <typename Rep1,
	          typename Ratio1,
	          typename UnitType1,
	          typename Rep2,
	          typename Ratio2,
	          typename UnitType2>
	constexpr auto operator+(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	    -> typename std::common_type<unit<Rep1, Ratio1, UnitType1>, unit<Rep2, Ratio2, UnitType2>>::type
	{
		using unit1       = unit<Rep1, Ratio1, UnitType1>;
		using unit2       = unit<Rep2, Ratio2, UnitType2>;
		using common_type = typename std::common_type<unit1, unit2>::type;

		return static_cast<common_type>(static_cast<common_type>(lhs).count()
		                                + static_cast<common_type>(rhs).count());
	}

	template <typename Rep1,
	          typename Ratio1,
	          typename UnitType1,
	          typename Rep2,
	          typename Ratio2,
	          typename UnitType2>
	constexpr auto operator-(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	    -> typename std::common_type<unit<Rep1, Ratio1, UnitType1>, unit<Rep2, Ratio2, UnitType2>>::type
	{
		using unit1       = unit<Rep1, Ratio1, UnitType1>;
		using unit2       = unit<Rep2, Ratio2, UnitType2>;
		using common_type = typename std::common_type<unit1, unit2>::type;

		return static_cast<common_type>(static_cast<common_type>(lhs).count()
		                                - static_cast<common_type>(rhs).count());
	}

	template <
	    typename Rep1,
	    typename Ratio,
	    typename UnitType,
	    typename Rep2>
	constexpr auto operator*(unit<Rep1, Ratio, UnitType> lhs, Rep2 const scalar)
	    -> unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>
	{
		using result_type = unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>;
		return static_cast<result_type>(static_cast<result_type>(lhs).count() * scalar);
	}

	template <
	    typename Rep1,
	    typename Rep2,
	    typename Ratio,
	    typename UnitType>
	constexpr auto operator*(Rep1 const scalar, unit<Rep2, Ratio, UnitType> rhs)
	    -> unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>
	{
		using result_type = unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>;
		return rhs * scalar;
	}

	template <
	    typename Rep1,
	    typename Ratio,
	    typename UnitType,
	    typename Rep2>
	constexpr auto operator/(unit<Rep1, Ratio, UnitType> lhs, Rep2 const scalar)
	    -> unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>
	{
		using result_type = unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>;
		return static_cast<result_type>(static_cast<result_type>(lhs).count() / scalar);
	}

	template <
	    typename Rep1,
	    typename Ratio1,
	    typename UnitType1,
	    typename Rep2,
	    typename Ratio2,
	    typename UnitType2>
	constexpr auto operator%(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	    -> typename std::common_type<unit<Rep1, Ratio1, UnitType1>, unit<Rep2, Ratio2, UnitType2>>::type
	{
		using unit1       = unit<Rep1, Ratio1, UnitType1>;
		using unit2       = unit<Rep2, Ratio2, UnitType2>;
		using common_type = typename std::common_type<unit1, unit2>::type;

		return common_type{common_type{lhs}.count() % common_type{rhs}.count()};
	}

	template <
	    typename Rep1,
	    typename Ratio,
	    typename UnitType,
	    typename Rep2>
	constexpr auto operator%(unit<Rep1, Ratio, UnitType> lhs, Rep2 const scalar)
	    -> typename detail::distance_div_mod<Rep1, Ratio, UnitType, Rep2>::type
	{
		using result_type = unit<typename std::common_type<Rep1, Rep2>::type, Ratio, UnitType>;
		return result_type{result_type{lhs}.count() % result_type{static_cast<typename result_type::rep>(scalar)}.count()};
	}

	namespace detail
	{
		template<typename T>
		constexpr auto abs(const T& value)
		{
			return (T{} > value) ? -value : value;
		}

		constexpr bool is_negative(double a)
		{
			return a <= 0.0;
		}

		constexpr bool unit_compare(double lhs,
									double rhs,
									double max_diff = 0.000000001,
									double max_relative_diff = std::numeric_limits<double>::epsilon())
		{
			return (abs(lhs - rhs) <= max_diff) ||
				(is_negative(lhs) != is_negative(rhs)) ||
				abs(lhs - rhs) <= (((abs(rhs) > abs(lhs)) ? abs(rhs) : abs(lhs)) * max_relative_diff);
		}
	}

	// Relational operations
	template <
	    typename Rep1,
	    typename Ratio1,
	    typename UnitType1,
	    typename Rep2,
	    typename Ratio2,
	    typename UnitType2>
	constexpr bool operator==(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	{
		using unit1       = unit<Rep1, Ratio1, UnitType1>;
		using unit2       = unit<Rep2, Ratio2, UnitType2>;
		using common_type = typename std::common_type<unit1, unit2>::type;
		using common_rep  = typename common_type::rep;

		return detail::unit_compare(unit_cast<common_type>(lhs).count(),
									unit_cast<common_type>(rhs).count());
	}

	template <
	    typename Rep1,
	    typename Ratio1,
	    typename UnitType1,
	    typename Rep2,
	    typename Ratio2,
	    typename UnitType2>
	constexpr bool operator!=(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	{
		using unit1       = unit<Rep1, Ratio1, UnitType1>;
		using unit2       = unit<Rep2, Ratio2, UnitType2>;
		using common_type = typename std::common_type<unit1, unit2>::type;

		return !(lhs == rhs);
	}

	template <
	    typename Rep1,
	    typename Ratio1,
	    typename UnitType1,
	    typename Rep2,
	    typename Ratio2,
	    typename UnitType2>
	constexpr bool operator<(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	{
		using unit1 = unit<Rep1, Ratio1, UnitType1>;
		using unit2 = unit<Rep2, Ratio2, UnitType2>;
		using common_type = typename std::common_type<unit1, unit2>::type;

		return std::isless(unit_cast<common_type>(lhs).count(), unit_cast<common_type>(rhs).count());
	}

	template <
	    typename Rep1,
	    typename Ratio1,
	    typename UnitType1,
	    typename Rep2,
	    typename Ratio2,
	    typename UnitType2>
	constexpr bool operator<=(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	{
		using unit1 = unit<Rep1, Ratio1, UnitType1>;
		using unit2 = unit<Rep2, Ratio2, UnitType2>;
		using common_type = typename std::common_type<unit1, unit2>::type;

		return std::islessequal(unit_cast<common_type>(lhs).count(), unit_cast<common_type>(rhs).count());
	}

	template <
	    typename Rep1,
	    typename Ratio1,
	    typename UnitType1,
	    typename Rep2,
	    typename Ratio2,
	    typename UnitType2>
	constexpr bool operator>(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	{
		using unit1 = unit<Rep1, Ratio1, UnitType1>;
		using unit2 = unit<Rep2, Ratio2, UnitType2>;
		using common_type = typename std::common_type<unit1, unit2>::type;

		return std::isgreater(unit_cast<common_type>(lhs).count(), unit_cast<common_type>(rhs).count());
	}

	template <
	    typename Rep1,
	    typename Ratio1,
	    typename UnitType1,
	    typename Rep2,
	    typename Ratio2,
	    typename UnitType2>
	constexpr bool operator>=(unit<Rep1, Ratio1, UnitType1> lhs, unit<Rep2, Ratio2, UnitType2> rhs)
	{
		using unit1 = unit<Rep1, Ratio1, UnitType1>;
		using unit2 = unit<Rep2, Ratio2, UnitType2>;
		using common_type = typename std::common_type<unit1, unit2>::type;

		return std::isgreaterequal(unit_cast<common_type>(lhs).count(), unit_cast<common_type>(rhs).count());
	}

	namespace detail
	{
		template <
		    typename ToDistance,
		    typename Ratio,
		    typename CommonType,
		    bool RatioNumIsOne = false,
		    bool RatioDenIsOne = false>
		struct unit_cast
		{
			template <typename Rep, typename Length, typename UnitType>
			static constexpr ToDistance cast(unit<Rep, Length, UnitType> from)
			{
				using ToRep = typename ToDistance::rep;
				return ToDistance{
				    static_cast<ToRep>(static_cast<CommonType>(from.count())
				                       / static_cast<CommonType>(Ratio::num)
				                       * static_cast<CommonType>(Ratio::den))};
			}
		};

		template <typename ToDistance, typename Ratio, typename CommonType>
		struct unit_cast<ToDistance, Ratio, CommonType, true, true>
		{
			template <typename Rep, typename Length, typename UnitType>
			static constexpr ToDistance cast(unit<Rep, Length, UnitType> from)
			{
				using ToRep = typename ToDistance::rep;
				return ToDistance{static_cast<ToRep>(static_cast<CommonType>(from.count()))};
			}
		};

		template <typename ToDistance, typename Ratio, typename CommonType>
		struct unit_cast<ToDistance, Ratio, CommonType, true, false>
		{
			template <typename Rep, typename Length, typename UnitType>
			static constexpr ToDistance cast(unit<Rep, Length, UnitType> from)
			{
				using ToRep = typename ToDistance::rep;
				return ToDistance{
				    static_cast<ToRep>(static_cast<CommonType>(from.count())
				                       * static_cast<CommonType>(Ratio::den))};
			}
		};

		template <typename ToDistance, typename Ratio, typename CommonType>
		struct unit_cast<ToDistance, Ratio, CommonType, false, true>
		{
			template <typename Rep, typename Length, typename UnitType>
			static constexpr ToDistance cast(unit<Rep, Length, UnitType> from)
			{
				using ToRep = typename ToDistance::rep;
				return ToDistance{
				    static_cast<ToRep>(static_cast<CommonType>(from.count())
				                       / static_cast<CommonType>(Ratio::num))};
			}
		};
	}

	template <typename ToUnit, typename Rep, typename Ratio, typename UnitType>
	constexpr auto unit_cast(unit<Rep, Ratio, UnitType> from)
	    -> typename std::enable_if<is_distance<ToUnit>::value, ToUnit>::type
	{
		static_assert(std::is_same<typename ToUnit::unit_type, UnitType>::value, "Incompatible types");

		using ToRatio     = typename ToUnit::ratio;
		using ToRep       = typename ToUnit::rep;
		using CommonType  = typename std::common_type<ToRep, Rep, intmax_t>::type;
		using CommonRatio = std::ratio_divide<ToRatio, Ratio>;

		return detail::unit_cast<ToUnit,
		                         CommonRatio,
		                         CommonType,
		                         CommonRatio::num == 1,
		                         CommonRatio::den == 1>::cast(from);
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
		constexpr units::thous operator"" _th(unsigned long long int dist)
		{
			return units::thous{ static_cast<units::thous::rep>(dist) };
		}

		constexpr units::inches operator"" _in(unsigned long long int dist)
		{
			return units::inches{static_cast<units::inches::rep>(dist)};
		}

		constexpr units::links operator"" _li(unsigned long long int dist)
		{
			return units::links{ static_cast<units::links::rep>(dist) };
		}

		constexpr units::feet operator"" _ft(unsigned long long int dist)
		{
			return units::feet{static_cast<units::feet::rep>(dist)};
		}

		constexpr units::yards operator"" _yd(unsigned long long int dist)
		{
			return units::yards{static_cast<units::yards::rep>(dist)};
		}

		constexpr units::rods operator"" _rd(unsigned long long int dist)
		{
			return units::rods{ static_cast<units::rods::rep>(dist) };
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

		// Maritime
		constexpr units::fathoms operator"" _ftm(unsigned long long int dist)
		{
			return units::fathoms{static_cast<units::fathoms::rep>(dist)};
		}

		constexpr units::cables operator"" _cb(unsigned long long int dist)
		{
			return units::cables{static_cast<units::cables::rep>(dist)};
		}

		constexpr units::nautical_miles operator"" _NM(unsigned long long int dist)
		{
			return units::nautical_miles{static_cast<units::nautical_miles::rep>(dist)};
		}

		constexpr units::nautical_miles operator"" _nmi(unsigned long long int dist)
		{
			return units::nautical_miles{static_cast<units::nautical_miles::rep>(dist)};
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