#include <gtest/gtest.h>

#include "units.h"
using namespace testing;
using namespace distance_literals;

namespace TestDistanceUnits
{
	class UnitConstructorTest : public Test
	{
	protected:
		units::distance<double> distance{1};
	};

	TEST_F(UnitConstructorTest, Constructor_WhenInvoked_WillInitialise)
	{
		units::metres metres{1};
		EXPECT_EQ(1, metres.count());
	}

	TEST_F(UnitConstructorTest, Constructor_WhenInokedWithDifferentCompatibleUnit_WillConvertToCurrentUnit)
	{
		units::metres metres{1_km};
		EXPECT_EQ(1000, metres.count());
	}

	class UnitIncrementDecrementTest : public UnitConstructorTest
	{
	};

	TEST_F(UnitIncrementDecrementTest, PreIncrementOperator_WillIncrementDistanceByOne)
	{
		auto result = ++distance;
		EXPECT_EQ(2, distance.count());
		EXPECT_EQ(2, result.count());
	}

	TEST_F(UnitIncrementDecrementTest, PostIncrementOperator_WillIncrementDistanceByOne)
	{
		auto result = distance++;
		EXPECT_EQ(2, distance.count());
		EXPECT_EQ(1, result.count());
	}

	TEST_F(UnitIncrementDecrementTest, PreDecrementOperator_WillDecrementDistanceByOne)
	{
		auto result = --distance;
		EXPECT_EQ(0, distance.count());
		EXPECT_EQ(0, result.count());
	}

	TEST_F(UnitIncrementDecrementTest, PostDecrementOperator_WillDecrementDistanceByOne)
	{
		auto result = distance--;
		EXPECT_EQ(0, distance.count());
		EXPECT_EQ(1, result.count());
	}

	class UnitCompoundAssignmentTest : public UnitConstructorTest
	{
	};

	TEST_F(UnitCompoundAssignmentTest, CompoundAdditionAssignment_WhenAddingMetresToMetres_WillResultInMetresAddedToCurrentMetres)
	{
		distance += 1_m;
		EXPECT_EQ(2, distance.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundAdditionAssignment_WhenAddingKilometresToMetres_WillResultInKilometresAddedToCurrentMetres)
	{
		distance += 1_km;
		EXPECT_EQ(1001, distance.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundSubtractionAssignment_WhenSubtractingMetresToMetres_WillResultInMetresSubtractedToCurrentMetres)
	{
		distance -= 1_m;
		EXPECT_EQ(0, distance.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundSubtractionAssignment_WhenSubtractingKilometresToMetres_WillResultInKilometresSubtractedToCurrentMetres)
	{
		distance -= 1_km;
		EXPECT_EQ(-999, distance.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundMultiplicationAssignment_WhenMultiplyingByScalar_WillUpdateValue)
	{
		distance *= 10;
		EXPECT_EQ(10, distance.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundDivisionAssignment_WhenDividingByScalar_WillUpdateValue)
	{
		distance = 10_m;
		distance /= 2;
		EXPECT_EQ(5, distance.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundRemainderAssignment_WhenGettingRemainderByScalar_WillUpdateValue)
	{
		distance = 2531_m;
		distance %= 1000;
		EXPECT_EQ(531, distance.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundRemainderAssignment_WhenGettingRemainderByDistance_WillUpdateValue)
	{
		distance = 2531_m;
		distance %= 1_km;
		EXPECT_EQ(531, distance.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundRemainderAssignment_WhenGettingRemainderByIntScalar_WillUpdateValue)
	{
		distance = units::distance<int>{2531};
		distance %= 1000;
		EXPECT_EQ(531, distance.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundRemainderAssignment_WhenGettingRemainderByIntDistance_WillUpdateValue)
	{
		distance = units::distance<int>{2531};
		distance %= 1_km;
		EXPECT_EQ(531, distance.count());
	}

	class UnitArithmeticOperationsTest : public Test
	{
	};

	TEST_F(UnitArithmeticOperationsTest, OperatorPlus_WhenUsed_WillAddMetresToMetres)
	{
		auto result = 1_m + 1_m;
		EXPECT_EQ(2, result.count());
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorPlus_WhenUsed_WillAddKilometresToMetres)
	{
		auto result = 1_m + 1_km;
		EXPECT_EQ(1001, result.count());
	}

	TEST_F(UnitArithmeticOperationsTest, CreatingMetresFromKilometres_WillYieldMetres)
	{
		EXPECT_EQ(1000, units::metres{1_km}.count());
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorMinus_WhenUsed_WillSubtractMetresFromMetres)
	{
		auto result = 1_m - 1_m;
		EXPECT_EQ(0, result.count());
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorMinus_WhenUsed_WillSubtractMetresFromKilometres)
	{
		auto result = 1_km - 1_m;
		EXPECT_EQ(999, result.count());
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorMultiply_WhenUsed_WillMultiplyMetresToScalar)
	{
		auto result = 1_m * 2;
		EXPECT_EQ(2, result.count());
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorMultiply_WhenUsed_WillMultiplyScalarToMetres)
	{
		auto result = 2 * 1_m;
		EXPECT_EQ(2, result.count());
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorDivide_WhenUsed_WillDivideMetresByScalar)
	{
		auto result = 2_m / 2;
		EXPECT_EQ(1, result.count());
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorRemainderInt_WhenGettingRemainderByDistance_WillGiveRemainder)
	{
		auto const result = units::distance<int>{2531} % units::distance<int, std::kilo>{1};
		EXPECT_EQ(531, result.count());
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorRemainderInt_WhenGettingRemainderByScalar_WillGiveRemainder)
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
	class BritishVsAmericanSpellingTest : public Test
	{
	};

	TYPED_TEST_CASE_P(BritishVsAmericanSpellingTest);

	TYPED_TEST_P(BritishVsAmericanSpellingTest, MetresVsMetersYieldsSameType)
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

	REGISTER_TYPED_TEST_CASE_P(BritishVsAmericanSpellingTest, MetresVsMetersYieldsSameType);

	INSTANTIATE_TYPED_TEST_CASE_P(BritishVsAmericanSpelling, BritishVsAmericanSpellingTest, BritishVsAmericanSpellingTuple);

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

	using ImperialLiteralsTuple = Types<std::tuple<units::inches, decltype(1_in)>,
	                                    std::tuple<units::feet, decltype(1_ft)>,
	                                    std::tuple<units::yards, decltype(1_yd)>,
	                                    std::tuple<units::chains, decltype(1_ch)>,
	                                    std::tuple<units::furlongs, decltype(1_fur)>,
	                                    std::tuple<units::miles, decltype(1_mi)>,
	                                    std::tuple<units::leagues, decltype(1_lea)>,
	                                    std::tuple<units::thous, decltype(1_th)>>;

	using MaritimeLiteralsTuple = Types<std::tuple<units::fathoms, decltype(1_ftm)>>;

	using AstronomicalUnitsLiteralsTuple = Types<std::tuple<units::earth_radii, decltype(1_R)>,
	                                             std::tuple<units::lunar_distances, decltype(1_LD)>,
	                                             std::tuple<units::astronimical_units, decltype(1_AU)>,
	                                             std::tuple<units::light_years, decltype(1_ly)>,
	                                             std::tuple<units::parsecs, decltype(1_pc)>>;

	REGISTER_TYPED_TEST_CASE_P(LiteralsTest, LiteralGeneratesCorrectType);

	INSTANTIATE_TYPED_TEST_CASE_P(Metric, LiteralsTest, MetricLiteralsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Imperial, LiteralsTest, ImperialLiteralsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(Maritime, LiteralsTest, MaritimeLiteralsTuple);
	INSTANTIATE_TYPED_TEST_CASE_P(AstronomicalUnits, LiteralsTest, AstronomicalUnitsLiteralsTuple);

	class UtilityTest : public Test
	{
	};

	TEST_F(UtilityTest, FMod_WhenGivenZeroForY_WillThrowDomainError)
	{
		EXPECT_THROW(units::detail::fmod(1.0, 0.0), std::domain_error);
	}
}
