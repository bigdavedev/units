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
	namespace conversion_tables
	{
		std::unordered_map<std::type_index, double> metric_to_imperial_lengths =
		    {{std::type_index{typeid(units::thous)}, 39370.078740158},
		     {std::type_index{typeid(units::inches)}, 39.37007874},
		     {std::type_index{typeid(units::links)}, 4.970969538},
		     {std::type_index{typeid(units::feet)}, 3.280839895},
		     {std::type_index{typeid(units::yards)}, 1.093613298},
		     {std::type_index{typeid(units::rods)}, 0.198838782},
		     {std::type_index{typeid(units::chains)}, 0.049709695},
		     {std::type_index{typeid(units::furlongs)}, 0.004970969},
		     {std::type_index{typeid(units::miles)}, 0.000621372},
		     {std::type_index{typeid(units::leagues)}, 0.000207124},

		     // Maritime
		     {std::type_index{typeid(units::fathoms)}, 0.539611825},
		     {std::type_index{typeid(units::cables)}, 0},
		     {std::type_index{typeid(units::nautical_miles)}, 0}};

		std::unordered_map<std::type_index, units::metres> imperial_to_metric =
		    {{std::type_index{typeid(units::thous)}, units::metres{0.0000254}},
		     {std::type_index{typeid(units::inches)}, units::metres{0.0254}},
		     {std::type_index{typeid(units::feet)}, units::metres{0.3048}},
		     {std::type_index{typeid(units::yards)}, units::metres{0.9144}},
		     {std::type_index{typeid(units::chains)}, units::metres{20.1168}},
		     {std::type_index{typeid(units::furlongs)}, units::metres{201.168}},
		     {std::type_index{typeid(units::miles)}, units::metres{1609.344}},
		     {std::type_index{typeid(units::leagues)}, units::metres{4828.032}}};
	}

	class DistanceConstructionTest : public Test
	{
	};

	class NonMetricToMetricConstruction : public DistanceConstructionTest,
	                                      public WithParamInterface<std::tuple<units::metres, units::metres>>
	{
	};

	TEST_P(NonMetricToMetricConstruction, Contructor_WhenGivenConvertibleType_WillYieldCorrectConversion)
	{
		EXPECT_NEAR(std::get<1>(GetParam()).count(), std::get<0>(GetParam()).count(), 0.0000001);
	}

	INSTANTIATE_TEST_CASE_P(ImperialToMetric,
	                        NonMetricToMetricConstruction,
	                        Values(std::make_tuple(1_th, units::metres{0.0000254}),
	                               std::make_tuple(1_in, units::metres{0.0254}),
	                               std::make_tuple(1_ft, units::metres{0.3048}),
	                               std::make_tuple(1_yd, units::metres{0.9144}),
	                               std::make_tuple(1_ch, units::metres{20.1168}),
	                               std::make_tuple(1_fur, units::metres{201.168}),
	                               std::make_tuple(1_mi, units::metres{1609.344}),
	                               std::make_tuple(1_lea, units::metres{4828.032})));

	INSTANTIATE_TEST_CASE_P(MaritimeToMetric,
	                        NonMetricToMetricConstruction,
	                        Values(std::make_tuple(1_ftm, units::metres{1.853184}),
	                               std::make_tuple(1_cb, units::metres{185.3184}),
	                               std::make_tuple(1_NM, units::metres{1853.184}),
	                               std::make_tuple(1_nmi, units::metres{1853.184})));

	INSTANTIATE_TEST_CASE_P(AstronomicalToMetric,
	                        NonMetricToMetricConstruction,
	                        Values(std::make_tuple(1_R, 6371000_m),
	                               std::make_tuple(1_LD, 384402000_m),
	                               std::make_tuple(1_AU, 149597870700_m),
	                               std::make_tuple(1_ly, 9460730472580800_m),
	                               std::make_tuple(1_pc, 30856775814671900_m)));

	template <typename T>
	class MetricToNonMetricConstruction : public DistanceConstructionTest
	{
	};

	TYPED_TEST_CASE_P(MetricToNonMetricConstruction);

	TYPED_TEST_P(MetricToNonMetricConstruction, Contructor_WhenGivenConvertibleType_WillYieldCorrectConversion)
	{
		auto expected = TypeParam{conversion_tables::metric_to_imperial_lengths[std::type_index{typeid(TypeParam)}]};
		auto actual   = TypeParam{1_m};
		EXPECT_EQ(expected, actual) << expected << " != " << actual;
	}

	using ImperialDistanceTypesTuple = Types<units::thous,
	                                         units::inches,
	                                         units::links,
	                                         units::feet,
	                                         units::yards,
	                                         units::rods,
	                                         units::chains,
	                                         units::furlongs,
	                                         units::miles,
	                                         units::leagues>;

	using MaritimeDistanceTypesTuple = Types<units::fathoms, units::cables, units::nautical_miles>;

	REGISTER_TYPED_TEST_CASE_P(MetricToNonMetricConstruction,
	                           Contructor_WhenGivenConvertibleType_WillYieldCorrectConversion);

	INSTANTIATE_TYPED_TEST_CASE_P(Imperial, MetricToNonMetricConstruction, ImperialDistanceTypesTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Maritime, MetricToNonMetricConstruction, MaritimeDistanceTypesTuple);

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

	class UtilityTest : public Test
	{
	};

	TEST_F(UtilityTest, FMod_WhenGivenZeroForY_WillThrowDomainError)
	{
		EXPECT_THROW(units::detail::fmod(1.0, 0.0), std::domain_error);
	}
}
