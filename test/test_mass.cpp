#include <gtest/gtest.h>

#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "units.h"

using testing::Test;
using testing::Types;
using testing::Values;
using testing::WithParamInterface;

namespace TestMassUnits
{
	namespace
	{
		auto imperial_to_metric_lookup = std::unordered_map<std::type_index, double>{
		    // Imperial to Metric
		    {std::type_index{typeid(units::grains)}, 0.06479891},
		    {std::type_index{typeid(units::drams)}, 1.7718451953125},
		    {std::type_index{typeid(units::pounds)}, 453.59237}
		    // End of lookup table
		};
	}

	class MassConstructionTest : public Test
	{
	};

	class NonMetricToMetricConstruction : public MassConstructionTest,
	                                      public WithParamInterface<std::tuple<units::kilograms, units::kilograms>>
	{
	};

	TEST_P(NonMetricToMetricConstruction, Contructor_WhenGivenConvertibleType_WillYieldCorrectConversion)
	{
		EXPECT_EQ(std::get<1>(GetParam()), std::get<0>(GetParam()));
	}

	INSTANTIATE_TEST_CASE_P(ImperialToMetric,
	                        NonMetricToMetricConstruction,
	                        Values(std::make_tuple(units::grains{1}, units::kilograms{0.00006479891}),
	                               std::make_tuple(units::drams{1}, units::kilograms{0.0017718451953125}),
	                               std::make_tuple(units::ounces{1}, units::kilograms{0.028349523125}),
	                               std::make_tuple(units::pounds{1}, units::kilograms{0.45359237}),
	                               std::make_tuple(units::us_hundredweight{1}, units::kilograms{45.359237}),
	                               std::make_tuple(units::long_hundredweight{1}, units::kilograms{50.80234544}),
	                               std::make_tuple(units::short_ton{1}, units::kilograms{907.18474}),
	                               std::make_tuple(units::long_ton{1}, units::kilograms{1016.0469088})),);

	using namespace mass_literals;
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

	using MetricMassLiteralsTuple = Types<std::tuple<units::picograms, decltype(1_pg)>,
	                                          std::tuple<units::nanograms, decltype(1_ng)>,
	                                          std::tuple<units::micrograms, decltype(1_ug)>,
	                                          std::tuple<units::milligrams, decltype(1_mg)>,
	                                          std::tuple<units::grams, decltype(1_g)>,
	                                          std::tuple<units::kilograms, decltype(1_kg)>>;

	using ImperialMassLiteralsTuple = Types<std::tuple<units::grains, decltype(1_gr)>,
	                                            std::tuple<units::drams, decltype(1_dr)>,
	                                            std::tuple<units::ounces, decltype(1_oz)>,
	                                            std::tuple<units::pounds, decltype(1_lb)>,
	                                            std::tuple<units::us_hundredweight, decltype(1_cwt)>>;

	REGISTER_TYPED_TEST_CASE_P(LiteralsTest, LiteralGeneratesCorrectType);

	INSTANTIATE_TYPED_TEST_CASE_P(MetricMass, LiteralsTest, MetricMassLiteralsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(ImperialMass, LiteralsTest, ImperialMassLiteralsTuple);
}
