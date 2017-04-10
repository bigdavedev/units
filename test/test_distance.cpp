#include <gtest/gtest.h>

#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "units.h"

using namespace distance_literals;

using testing::Test;
using testing::Types;
using testing::Values;
using testing::WithParamInterface;

namespace TestDistanceUnits
{
	template<typename T>
	class UnitCastTest : public Test
	{
	};

	TYPED_TEST_CASE_P(UnitCastTest);

#define UNITS_TYPE_COMBINE(T) std::tuple<T, units::nanometres>, \
std::tuple<T, units::micrometres>, \
std::tuple<T, units::millimetres>, \
std::tuple<T, units::centimetres>, \
std::tuple<T, units::decimetres>, \
std::tuple<T, units::metres>, \
std::tuple<T, units::kilometres>, \
std::tuple<T, units::thous>, \
std::tuple<T, units::inches>, \
std::tuple<T, units::links>, \
std::tuple<T, units::feet>, \
std::tuple<T, units::yards>, \
std::tuple<T, units::rods>, \
std::tuple<T, units::chains>, \
std::tuple<T, units::furlongs>, \
std::tuple<T, units::miles>, \
std::tuple<T, units::leagues>, \
std::tuple<T, units::fathoms>, \
std::tuple<T, units::cables>, \
std::tuple<T, units::nautical_miles>

	TYPED_TEST_P(UnitCastTest, UnitCast_WhenConvertedToCompatibleType_WillConvertBackWithoutPrecisionLoss)
	{
		using test_type = typename std::tuple_element_t<0, TypeParam>;
		using convertible_type = typename std::tuple_element_t<1, TypeParam>;

		convertible_type conversion = units::unit_cast<convertible_type>(test_type{ 1 });
		test_type result = units::unit_cast<test_type>(conversion);

		EXPECT_EQ(test_type{ 1 }, result);
	}

	// Metric
	using NanometresTuple = Types<UNITS_TYPE_COMBINE(units::nanometres)>;
	using MicrometresTuple = Types<UNITS_TYPE_COMBINE(units::micrometres)>;
	using MillimetresTuple = Types<UNITS_TYPE_COMBINE(units::millimetres)>;
	using CentimetresTuple = Types<UNITS_TYPE_COMBINE(units::centimetres)>;
	using DecimetresTuple = Types<UNITS_TYPE_COMBINE(units::decimetres)>;
	using MetresTuple = Types<UNITS_TYPE_COMBINE(units::metres)>;
	using KilometresTuple = Types<UNITS_TYPE_COMBINE(units::kilometres)>;

	// Imperial
	using ThousTuple = Types<UNITS_TYPE_COMBINE(units::thous)>;
	using InchesTuple = Types<UNITS_TYPE_COMBINE(units::inches)>;
	using LinksTuple = Types<UNITS_TYPE_COMBINE(units::links)>;
	using FeetTuple = Types<UNITS_TYPE_COMBINE(units::feet)>;
	using YardsTuple = Types<UNITS_TYPE_COMBINE(units::yards)>;
	using RodsTuple = Types<UNITS_TYPE_COMBINE(units::rods)>;
	using ChainsTuple = Types<UNITS_TYPE_COMBINE(units::chains)>;
	using FurlongsTuple = Types<UNITS_TYPE_COMBINE(units::furlongs)>;
	using MilesTuple = Types<UNITS_TYPE_COMBINE(units::miles)>;
	using LeaguesTuple = Types<UNITS_TYPE_COMBINE(units::leagues)>;

	// Maritime
	using FathomsTuple = Types<UNITS_TYPE_COMBINE(units::fathoms)>;
	using CablesTuple = Types<UNITS_TYPE_COMBINE(units::cables)>;
	using NauticalMilesTuple = Types<UNITS_TYPE_COMBINE(units::nautical_miles)>;

#undef UNITS_TYPE_COMBINE

	REGISTER_TYPED_TEST_CASE_P(UnitCastTest,
							   UnitCast_WhenConvertedToCompatibleType_WillConvertBackWithoutPrecisionLoss);

	INSTANTIATE_TYPED_TEST_CASE_P(Nanometres, UnitCastTest, NanometresTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Micrometres, UnitCastTest, MicrometresTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Millimetres, UnitCastTest, MillimetresTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Centimetres, UnitCastTest, CentimetresTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(DeciMetres, UnitCastTest, DecimetresTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Metres, UnitCastTest, MetresTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Kilometres, UnitCastTest, KilometresTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Thous, UnitCastTest, ThousTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Inches, UnitCastTest, InchesTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Links, UnitCastTest, LinksTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Feet, UnitCastTest, FeetTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Yards, UnitCastTest, YardsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Rods, UnitCastTest, RodsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Chains, UnitCastTest, ChainsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Furlongs, UnitCastTest, FurlongsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Miles, UnitCastTest, MilesTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Leagues, UnitCastTest, LeaguesTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Fathoms, UnitCastTest, FathomsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Cables, UnitCastTest, CablesTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(NauticalMiles, UnitCastTest, NauticalMilesTuple);

	template <typename T>
	class BritishVsAmericanSpellingTest : public Test
	{
	};

	TYPED_TEST_CASE_P(BritishVsAmericanSpellingTest);

	TYPED_TEST_P(BritishVsAmericanSpellingTest, EitherSpellingYieldsSameType)
	{
		constexpr bool is_same = std::is_same<typename std::tuple_element<0, TypeParam>::type,
		                                      typename std::tuple_element<1, TypeParam>::type>::value;
		EXPECT_TRUE(is_same);
	}

	using BritishVsAmericanSpellingTuple = Types<std::tuple<units::nanometres, units::nanometers>,
	                                             std::tuple<units::micrometres, units::micrometers>,
	                                             std::tuple<units::millimetres, units::millimeters>,
	                                             std::tuple<units::centimetres, units::centimeters>,
	                                             std::tuple<units::decimetres, units::decimeters>,
	                                             std::tuple<units::metres, units::meters>,
	                                             std::tuple<units::kilometres, units::kilometers>>;

	REGISTER_TYPED_TEST_CASE_P(BritishVsAmericanSpellingTest, EitherSpellingYieldsSameType);

	INSTANTIATE_TYPED_TEST_CASE_P(BritishVsAmericanSpelling,
	                              BritishVsAmericanSpellingTest,
	                              BritishVsAmericanSpellingTuple);

	template <typename T>
	class LiteralsTest : public Test
	{
	};

	TYPED_TEST_CASE_P(LiteralsTest);

	TYPED_TEST_P(LiteralsTest, LiteralGeneratesCorrectType)
	{
		using expected_type    = typename std::tuple_element<0, TypeParam>::type;
		using actual_type      = typename std::tuple_element<1, TypeParam>::type;
		constexpr auto is_same = std::is_same<expected_type, actual_type>::value;
		EXPECT_TRUE(is_same) << "Incompatible types";
	}

	using MetricDistanceLiteralsTuple = Types<std::tuple<units::nanometres, decltype(1_nm)>,
	                                          std::tuple<units::micrometres, decltype(1_um)>,
	                                          std::tuple<units::millimetres, decltype(1_mm)>,
	                                          std::tuple<units::centimetres, decltype(1_cm)>,
	                                          std::tuple<units::metres, decltype(1_m)>,
	                                          std::tuple<units::kilometres, decltype(1_km)>>;

	using ImperialDistanceLiteralsTuple = Types<std::tuple<units::inches, decltype(1_in)>,
	                                            std::tuple<units::feet, decltype(1_ft)>,
	                                            std::tuple<units::yards, decltype(1_yd)>,
	                                            std::tuple<units::chains, decltype(1_ch)>,
	                                            std::tuple<units::furlongs, decltype(1_fur)>,
	                                            std::tuple<units::miles, decltype(1_mi)>,
	                                            std::tuple<units::leagues, decltype(1_lea)>,
	                                            std::tuple<units::thous, decltype(1_th)>>;

	using MaritimeDistanceLiteralsTuple = Types<std::tuple<units::fathoms, decltype(1_ftm)>,
	                                            std::tuple<units::cables, decltype(1_cb)>,
	                                            std::tuple<units::nautical_miles, decltype(1_NM)>,
	                                            std::tuple<units::nautical_miles, decltype(1_nmi)>>;

	using AstronomicalUnitsDistanceLiteralsTuple = Types<std::tuple<units::earth_radii, decltype(1_R)>,
	                                                     std::tuple<units::lunar_distances, decltype(1_LD)>,
	                                                     std::tuple<units::astronimical_units, decltype(1_AU)>,
	                                                     std::tuple<units::light_years, decltype(1_ly)>,
	                                                     std::tuple<units::parsecs, decltype(1_pc)>>;

	REGISTER_TYPED_TEST_CASE_P(LiteralsTest, LiteralGeneratesCorrectType);

	INSTANTIATE_TYPED_TEST_CASE_P(MetricDistance, LiteralsTest, MetricDistanceLiteralsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(ImperialDistance, LiteralsTest, ImperialDistanceLiteralsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(MaritimeDistance, LiteralsTest, MaritimeDistanceLiteralsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(AstronomicalUnitsDistance, LiteralsTest, AstronomicalUnitsDistanceLiteralsTuple);
}
