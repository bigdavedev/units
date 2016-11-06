#include <gtest/gtest.h>

#include "units.h"
using namespace testing;
using namespace distance_literals;

namespace TestDistanceUnits
{
	class DistanceTest : public Test
	{
	protected:
		units::distance<double> distance{1};
	};

	TEST_F(DistanceTest, Constructor_WhenInvoked_WillInitialise)
	{
		EXPECT_EQ(1, distance.count());
	}

	TEST_F(DistanceTest, PreIncrementOperator_WillIncrementDistanceByOne)
	{
		++distance;
		EXPECT_EQ(2, distance.count());
	}

	TEST_F(DistanceTest, PostIncrementOperator_WillIncrementDistanceByOne)
	{
		auto result = distance++;
		EXPECT_EQ(2, result.count());
	}

	TEST_F(DistanceTest, PreDecrementOperator_WillDecrementDistanceByOne)
	{
		--distance;
		EXPECT_EQ(0, distance.count());
	}

	TEST_F(DistanceTest, PostDecrementOperator_WillDecrementDistanceByOne)
	{
		auto result = distance--;
		EXPECT_EQ(0, result.count());
	}

	TEST_F(DistanceTest, ConstructFromMetresFromKilometres_WillCreateMetresContaining1000)
	{
		units::metres metres = 1_km;
		EXPECT_NEAR(1000.0, metres.count(), 0.000000001);
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenAddingMetresToMetres_WillResultInMetresAddedToCurrentMetres)
	{
		distance += 1_m;
		EXPECT_EQ(2, distance.count());
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenAddingKilometresToMetres_WillResultInKilometresAddedToCurrentMetres)
	{
		distance += 1_km;
		EXPECT_EQ(1001, distance.count());
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenSubtractingMetresToMetres_WillResultInMetresSubtractedToCurrentMetres)
	{
		distance -= 1_m;
		EXPECT_EQ(0, distance.count());
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenSubtractingKilometresToMetres_WillResultInKilometresSubtractedToCurrentMetres)
	{
		distance -= 1_km;
		EXPECT_EQ(-999, distance.count());
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenMultiplyingByScalar_WillUpdateValue)
	{
		distance *= 10;
		EXPECT_EQ(10, distance.count());
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenDividingByScalar_WillUpdateValue)
	{
		distance = 10_m;
		distance /= 2;
		EXPECT_EQ(5, distance.count());
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenGettingRemainderByScalar_WillUpdateValue)
	{
		distance = 2531_m;
		distance %= 1000;
		EXPECT_EQ(531, distance.count());
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenGettingRemainderByDistance_WillUpdateValue)
	{
		distance = 2531_m;
		distance %= 1_km;
		EXPECT_EQ(531, distance.count());
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenGettingRemainderByIntScalar_WillUpdateValue)
	{
		distance = units::distance<int>{2531};
		distance %= 1000;
		EXPECT_EQ(531, distance.count());
	}

	TEST_F(DistanceTest, CompoundAssignment_WhenGettingRemainderByIntDistance_WillUpdateValue)
	{
		distance = units::distance<int>{2531};
		distance %= 1_km;
		EXPECT_EQ(531, distance.count());
	}

	TEST_F(DistanceTest, Meters_IsIdenticalToMetres)
	{
		constexpr bool is_same = std::is_same<units::metres, units::meters>::value;
		EXPECT_TRUE(is_same);
	}

	TEST_F(DistanceTest, OperatorPlus_WhenUsed_WillAddMetresToMetres)
	{
		auto result = distance + 1_m;
		EXPECT_EQ(2, result.count());
	}

	TEST_F(DistanceTest, OperatorPlus_WhenUsed_WillAddKilometresToMetres)
	{
		auto result = distance + 1_km;
		EXPECT_EQ(1001, result.count());
	}

	TEST_F(DistanceTest, OperatorMinus_WhenUsed_WillSubtractMetresFromMetres)
	{
		auto result = distance - 1_m;
		EXPECT_EQ(0, result.count());
	}

	TEST_F(DistanceTest, OperatorMinus_WhenUsed_WillSubtractMetresFromKilometres)
	{
		auto result = 1_km - distance;
		EXPECT_EQ(999, result.count());
	}

	TEST_F(DistanceTest, OperatorMultiply_WhenUsed_WillMultiplyMetresToScalar)
	{
		auto result = distance * 2;
		EXPECT_EQ(2, result.count());
	}

	TEST_F(DistanceTest, OperatorMultiply_WhenUsed_WillMultiplyScalarToMetres)
	{
		auto result = 2 * distance;
		EXPECT_EQ(2, result.count());
	}

	TEST_F(DistanceTest, OperatorDivide_WhenUsed_WillDivideMetresByScalar)
	{
		auto result = 2_m / 2;
		EXPECT_EQ(1, result.count());
	}

	TEST_F(DistanceTest, OperatorRemainderInt_WhenGettingRemainderByDistance_WillGiveRemainder)
	{
		auto const result = units::distance<int>{ 2531 } % units::distance<int, std::kilo>{1};
		EXPECT_EQ(531, result.count());
	}

	TEST_F(DistanceTest, OperatorRemainderInt_WhenGettingRemainderByScalar_WillGiveRemainder)
	{
		auto const result = units::distance<int>{2531} % 1000;
		EXPECT_EQ(531, result.count());
	}

	class DistanceCastTest : public Test
	{
	};

	TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToMetres_WillYieldMetres)
	{
		auto metres = units::distance_cast<units::metres>(1000_m);
		EXPECT_NEAR(1000, metres.count(), 0.0000001);
	}

	TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToKiloMetres_WillYieldKilometres)
	{
		auto kilometres = units::distance_cast<units::kilometres>(1000_m);
		EXPECT_NEAR(1, kilometres.count(), 0.0000001);
	}

	TEST_F(DistanceCastTest, Cast_WhenCastingFromKilometresToMetres_WillYieldMetres)
	{
		auto metres = units::distance_cast<units::metres>(1_km);
		EXPECT_NEAR(1000, metres.count(), 0.0000001);
	}

	TEST_F(DistanceCastTest, Cast_WhenCastingFromHalfUnitToWhole_WillRoundDown)
	{
		auto kilometres = units::distance_cast<units::distance<int, std::kilo>>(999_m);
		EXPECT_EQ(0, kilometres.count());
	}

	TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToYards_WillYieldYards)
	{
		auto yards = units::distance_cast<units::yards>(1_m);
		EXPECT_NEAR(1.09361, yards.count(), 0.00001);
	}

	TEST_F(DistanceCastTest, Cast_WhenCastingFromKiloMetresToYards_WillYieldYards)
	{
		auto yards = units::distance_cast<units::yards>(1_km);
		EXPECT_NEAR(1093.6133, yards.count(), 0.0001);
	}

	TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToFeet_WillYieldFeet)
	{
		auto feet = units::distance_cast<units::feet>(1_m);
		EXPECT_NEAR(3.28084, feet.count(), 0.00001);
	}

	TEST_F(DistanceCastTest, Cast_WhenCastingFromYardToFeet_WillYieldFeet)
	{
		auto feet = units::distance_cast<units::feet>(units::yards{1});
		EXPECT_NEAR(3.0, feet.count(), 0.0001);
	}

	TEST_F(DistanceCastTest, Cast_WhenCastingFromYardsToInches_WillYieldInches)
	{
		auto inches = units::distance_cast<units::inches>(units::yards{1});
		EXPECT_NEAR(36.0, inches.count(), 0.0001);
	}

	TEST_F(DistanceCastTest, Cast_WhenCastingFromKilometresToNanometres_WillYieldNanometres)
	{
		auto nanometres = units::distance_cast<units::nanometres>(1_km);
		EXPECT_NEAR(1000000000000, nanometres.count(), 0.0000001);
	}

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

	using MetricLiteralsTuple = Types<std::tuple<units::nanometres, decltype(1_nm)>,
	                                  std::tuple<units::micrometres, decltype(1_um)>,
	                                  std::tuple<units::millimetres, decltype(1_mm)>,
	                                  std::tuple<units::centimetres, decltype(1_cm)>,
	                                  std::tuple<units::metres, decltype(1_m)>,
	                                  std::tuple<units::kilometres, decltype(1_km)>>;

	using ImperialLiteralsTuple = Types<std::tuple<units::inches, decltype(1_in)>>;
	REGISTER_TYPED_TEST_CASE_P(LiteralsTest, LiteralGeneratesCorrectType);

	INSTANTIATE_TYPED_TEST_CASE_P(Metric, LiteralsTest, MetricLiteralsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Imperial, LiteralsTest, ImperialLiteralsTuple);

	class UtilityTest : public Test
	{
	};

	TEST_F(UtilityTest, FMod_WhenGivenZeroForY_WillThrowDomainError)
	{
		EXPECT_THROW(units::detail::fmod(1.0, 0.0), std::domain_error);
	}
}
