#include <gtest/gtest.h>

#include "units.h"

using testing::Test;
using testing::WithParamInterface;
using testing::Values;

namespace TestUnits
{
	struct base_unit_type
	{
	};

	using base_unit = units::unit<double, std::ratio<1>, base_unit_type>;
	using kilo_unit = units::unit<double, std::kilo, base_unit_type>;
	using nano_unit = units::unit<double, std::nano, base_unit_type>;

	using base_unit_int = units::unit<int, std::ratio<1>, base_unit_type>;
	using kilo_unit_int = units::unit<int, std::kilo, base_unit_type>;
}

namespace units
{
	namespace detail
	{
		template <>
		inline std::string get_unit<TestUnits::base_unit_type>()
		{
			return "";
		}
	}
}

namespace TestUnits
{
	class UnitConstructorTest : public Test
	{
	};

	TEST_F(UnitConstructorTest, Constructor_WhenInvoked_WillInitialise)
	{
		auto result = base_unit{1};
		EXPECT_EQ(1, result.count());
	}

	TEST_F(UnitConstructorTest, Constructor_WhenInokedWithDifferentCompatibleUnit_WillConvertToCurrentUnit)
	{
		base_unit result{kilo_unit{1}};
		EXPECT_EQ(1000, result.count());
	}

	class UnitBaseTest : public Test
	{
	protected:
		base_unit unit{1};
	};

	TEST_F(UnitBaseTest, Count_WillReturnCorrectValue) { EXPECT_EQ(1, unit.count()); }

	class UnitUnaryOperatorTest : public UnitBaseTest
	{
	};

	TEST_F(UnitUnaryOperatorTest, UnaryMinus_WhenValueIsPositive_WillReturnNegativeValue)
	{
		auto result = -unit;
		EXPECT_EQ(base_unit{-1}, result);
	}

	TEST_F(UnitUnaryOperatorTest, UnaryMinus_WhenValueIsNegative_WillReturnPositiveValue)
	{
		unit        = base_unit{-1};
		auto result = -unit;
		EXPECT_EQ(base_unit{1}, result);
	}

	TEST_F(UnitUnaryOperatorTest, UnaryPlus_WhenValueIsPositive_WillReturnPositiveValue)
	{
		auto result = +unit;
		EXPECT_EQ(base_unit{1}, result);
	}

	TEST_F(UnitUnaryOperatorTest, UnaryPlus_WhenValueIsNegative_WillReturnNegativeValue)
	{
		unit        = base_unit{-1};
		auto result = +unit;
		EXPECT_EQ(base_unit{-1}, result);
	}

	class UnitIncrementDecrementTest : public UnitBaseTest
	{
	};

	TEST_F(UnitIncrementDecrementTest, PreIncrementOperator_WillIncrementDistanceByOne)
	{
		auto result = ++unit;
		EXPECT_EQ(base_unit{2}, unit);
		EXPECT_EQ(base_unit{2}, result);
	}

	TEST_F(UnitIncrementDecrementTest, PostIncrementOperator_WillIncrementDistanceByOne)
	{
		auto result = unit++;
		EXPECT_EQ(base_unit{2}, unit);
		EXPECT_EQ(base_unit{1}, result);
	}

	TEST_F(UnitIncrementDecrementTest, PreDecrementOperator_WillDecrementDistanceByOne)
	{
		auto result = --unit;
		EXPECT_EQ(base_unit{0}, unit);
		EXPECT_EQ(base_unit{0}, result);
	}

	TEST_F(UnitIncrementDecrementTest, PostDecrementOperator_WillDecrementDistanceByOne)
	{
		auto result = unit--;
		EXPECT_EQ(base_unit{0}, unit);
		EXPECT_EQ(base_unit{1}, result);
	}

	class UnitCompoundAssignmentTest : public UnitBaseTest
	{
	};

	TEST_F(UnitCompoundAssignmentTest,
	       CompoundAdditionAssignment_WhenAddingMetresToMetres_WillResultInMetresAddedToCurrentMetres)
	{
		unit += base_unit{1};
		EXPECT_EQ(base_unit{2}, unit);
	}

	TEST_F(UnitCompoundAssignmentTest,
	       CompoundAdditionAssignment_WhenAddingKilometresToMetres_WillResultInKilometresAddedToCurrentMetres)
	{
		unit += kilo_unit{1};
		EXPECT_EQ(base_unit{1001}, unit);
	}

	TEST_F(UnitCompoundAssignmentTest,
	       CompoundSubtractionAssignment_WhenSubtractingMetresFromMetres_WillResultInMetresSubtractedToCurrentMetres)
	{
		unit -= base_unit{1};
		EXPECT_EQ(base_unit{0}, unit);
	}

	TEST_F(
	    UnitCompoundAssignmentTest,
	    CompoundSubtractionAssignment_WhenSubtractingKilometresFromMetres_WillResultInKilometresSubtractedToCurrentMetres)
	{
		unit -= kilo_unit{1};
		EXPECT_EQ(-999, unit.count());
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundMultiplicationAssignment_WhenMultiplyingByScalar_WillUpdateValue)
	{
		unit *= 10;
		EXPECT_EQ(base_unit{10}, unit);
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundDivisionAssignment_WhenDividingByScalar_WillUpdateValue)
	{
		unit = base_unit{10};
		unit /= 2;
		EXPECT_EQ(base_unit{5}, unit);
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundRemainderAssignment_WhenGettingRemainderByScalar_WillUpdateValue)
	{
		unit = base_unit{2531};
		unit %= 1000;
		EXPECT_EQ(base_unit{531}, unit);
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundRemainderAssignment_WhenGettingRemainderByDistance_WillUpdateValue)
	{
		unit = base_unit{2531};
		unit %= kilo_unit{1};
		EXPECT_EQ(base_unit{531}, unit);
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundRemainderAssignment_WhenGettingRemainderByIntScalar_WillUpdateValue)
	{
		unit = base_unit_int{2531};
		unit %= 1000;
		EXPECT_EQ(base_unit_int{531}, unit);
	}

	TEST_F(UnitCompoundAssignmentTest, CompoundRemainderAssignment_WhenGettingRemainderByIntDistance_WillUpdateValue)
	{
		unit = base_unit_int{2531};
		unit %= kilo_unit{1};
		EXPECT_EQ(base_unit_int{531}, unit);
	}

	class UnitArithmeticOperationsTest : public Test
	{
	};

	TEST_F(UnitArithmeticOperationsTest, OperatorPlus_WillAddMetresToMetres)
	{
		auto result = base_unit{1} + base_unit{1};
		EXPECT_EQ(base_unit{2}, result);
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorPlus_WillAddKilometresToMetres)
	{
		auto result = base_unit{1} + kilo_unit{1};
		EXPECT_EQ(base_unit{1001}, result);
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorMinus_WillSubtractMetresFromMetres)
	{
		auto result = base_unit{1} - base_unit{1};
		EXPECT_EQ(base_unit{0}, result);
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorMinus_WillSubtractMetresFromKilometres)
	{
		auto result = kilo_unit{1} - base_unit{1};
		EXPECT_EQ(base_unit{999}, result);
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorMultiply_WillMultiplyMetresToScalar)
	{
		auto result = base_unit{1} * 2;
		EXPECT_EQ(base_unit{2}, result);
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorMultiply_WillMultiplyScalarToMetres)
	{
		auto result = 2 * base_unit{1};
		EXPECT_EQ(base_unit{2}, result);
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorDivide_WillDivideMetresByScalar)
	{
		auto result = base_unit{2} / 2;
		EXPECT_EQ(base_unit{1}, result);
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorRemainderInt_WhenGettingRemainderByDistance_WillGiveRemainder)
	{
		auto const result = base_unit_int{2531} % kilo_unit_int{1};
		EXPECT_EQ(base_unit_int{531}, result);
	}

	TEST_F(UnitArithmeticOperationsTest, OperatorRemainderInt_WhenGettingRemainderByScalar_WillGiveRemainder)
	{
		auto const result = base_unit_int{2531} % 1000;
		EXPECT_EQ(base_unit_int{531}, result);
	}

	class UnitRelationalOperationsTest : public Test
	{
	};

	TEST_F(UnitRelationalOperationsTest, EqualityOperator_WhenOperandsAreEqual_WillReturnTrue)
	{
		EXPECT_TRUE(base_unit{1} == base_unit{1});
	}

	TEST_F(UnitRelationalOperationsTest, EqualityOperator_WhenOperandsArIneEqual_WillReturnFalse)
	{
		EXPECT_FALSE(base_unit{1} == nano_unit{999999999});
	}

	TEST_F(UnitRelationalOperationsTest, InqualityOperator_WhenOperandsAreInequal_WillReturnTrue)
	{
		EXPECT_TRUE(base_unit{1} != nano_unit{999999999});
	}

	TEST_F(UnitRelationalOperationsTest, InqualityOperator_WhenOperandsAreEqual_WillReturnFalse)
	{
		EXPECT_FALSE(base_unit{1} != base_unit{1});
	}

	TEST_F(UnitRelationalOperationsTest, LessThanOperator_WhenLeftHandOperandIsSmaller_WillReturnTrue)
	{
		EXPECT_TRUE(nano_unit{999999999} < base_unit{1});
	}

	TEST_F(UnitRelationalOperationsTest, LessThanOperator_WhenLeftHandOperandIsGreater_WillReturnFalse)
	{
		EXPECT_FALSE(base_unit{1} < nano_unit{999999999});
	}

	TEST_F(UnitRelationalOperationsTest, LessThanEqualOperator_WhenLeftHandOperandIsSmaller_WillReturnTrue)
	{
		EXPECT_TRUE(nano_unit{999999999} <= base_unit{1});
	}

	TEST_F(UnitRelationalOperationsTest, LessThanEqualOperator_WhenOperandsAreEqual_WillReturnTrue)
	{
		EXPECT_TRUE(base_unit{1} <= base_unit{1});
	}

	TEST_F(UnitRelationalOperationsTest, LessThanEqualOperator_WhenLeftHandOperandIsGreater_WillReturnFalse)
	{
		EXPECT_FALSE(base_unit{1} <= nano_unit{999999999});
	}

	TEST_F(UnitRelationalOperationsTest, GreaterThanOperator_WhenLeftHandOperandIsSmaller_WillReturnFalse)
	{
		EXPECT_FALSE(nano_unit{999999999} > base_unit{1});
	}

	TEST_F(UnitRelationalOperationsTest, GreatersThanOperator_WhenLeftHandOperandIsGreater_WillReturnTrue)
	{
		EXPECT_TRUE(base_unit{1} > nano_unit{999999999});
	}

	TEST_F(UnitRelationalOperationsTest, GreaterThanEqualOperator_WhenOperandsAreEqual_WillReturnTrue)
	{
		EXPECT_TRUE(base_unit{1} >= base_unit{1});
	}

	TEST_F(UnitRelationalOperationsTest, GreaterThanEqualOperator_WhenLeftHandOperandIsGreater_WillReturnTrue)
	{
		EXPECT_TRUE(base_unit{1} >= nano_unit{999999999});
	}

	TEST_F(UnitRelationalOperationsTest, GreaterThanEqualOperator_WhenLeftHandOperandIsSmaller_WillReturnFalse)
	{
		EXPECT_FALSE(nano_unit{999999999} >= base_unit{1});
	}

	class UnitCastTest : public UnitBaseTest
	{
	};

	TEST_F(UnitCastTest, UnitCast_WhenCastingToSameType_WillYieldNoConversion)
	{
		auto result = units::unit_cast<base_unit>(base_unit{1000});

		EXPECT_TRUE((std::is_same<decltype(result), base_unit>::value));
		EXPECT_EQ(1000, result.count());
	}

	TEST_F(UnitCastTest, UnitCast_WhenCastingToUnitWithDifferentRatio_WillYieldConversionToThatType)
	{
		auto result = units::unit_cast<kilo_unit>(base_unit_int{1000});

		EXPECT_TRUE((std::is_same<decltype(result), kilo_unit>::value));
		EXPECT_EQ(kilo_unit{1}, result);
	}

	TEST_F(UnitCastTest, UnitCast_WhenCastingToFinerUnit_WillYielConversionToThatType)
	{
		auto result = units::unit_cast<base_unit>(kilo_unit{1});

		EXPECT_TRUE((std::is_same<decltype(result), base_unit>::value));
		EXPECT_EQ(base_unit{1000}, result);
	}

	TEST_F(UnitCastTest, UnitCast_WhenCastingFromHalfUnitToWhole_WillRoundDown)
	{
		auto result = units::unit_cast<kilo_unit_int>(base_unit_int{999});

		EXPECT_TRUE((std::is_same<decltype(result), kilo_unit_int>::value));
		EXPECT_EQ(kilo_unit_int{0}, result);
	}

	TEST_F(UnitCastTest, UnitCast_WillCastToUnderlyingType)
	{
		auto result = units::unit_cast<base_unit::rep>(base_unit{1});

		EXPECT_TRUE((std::is_same<decltype(result), base_unit::rep>::value));
		EXPECT_EQ(1, result);
	}

	TEST_F(UnitCastTest, UnitCast_WillCastToTypeCompatibleWithUnderlyingType)
	{
		auto result = units::unit_cast<int>(base_unit{1});

		EXPECT_TRUE((std::is_same<decltype(result), int>::value));
		EXPECT_EQ(1, result);
	}

	class UtilityTest : public Test
	{
	};

	TEST_F(UtilityTest, FMod_WhenGivenZeroForY_WillThrowDomainError)
	{
		EXPECT_THROW(units::detail::fmod(1.0, 0.0), std::domain_error);
	}

	TEST_F(UtilityTest, Abs_WhenValueIsPositive_WillReturnPositiveValue) { EXPECT_EQ(1, units::detail::abs(1)); }

	TEST_F(UtilityTest, Abs_WhenValueIsNegative_WillReturnPositiveValue) { EXPECT_EQ(1, units::detail::abs(-1)); }

	TEST_F(UtilityTest, Abs_WhenValueIsNegativeZero_WillReturnPositiveValue)
	{
		EXPECT_EQ(double{0}, units::detail::abs(double{-0}));
	}

	using UnitCompareTuple = std::tuple<double, double, double, double>;

	class UnitCompareEqualTest : public UtilityTest, public WithParamInterface<UnitCompareTuple>
	{
	};

	TEST_P(UnitCompareEqualTest, UnitCompare_WhenValuesAreEquivalent_WillReturnTrue)
	{
		auto const lhs               = std::get<0>(GetParam());
		auto const rhs               = std::get<1>(GetParam());
		auto const max_diff          = std::get<2>(GetParam());
		auto const max_relative_diff = std::get<3>(GetParam());

		EXPECT_TRUE(units::detail::unit_compare(lhs, rhs, max_diff, max_relative_diff));
	}

	INSTANTIATE_TEST_CASE_P(,
	                        UnitCompareEqualTest,
	                        Values(std::make_tuple(1, 1, 0.000001, std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(1.0000001, 1.0000001, 0.1, std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(1.0,
	                                               1.0 + std::numeric_limits<double>::epsilon(),
	                                               0.1,
	                                               std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(1.0,
	                                               1.0 + std::numeric_limits<double>::epsilon(),
	                                               std::numeric_limits<double>::epsilon(),
	                                               std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(1.0,
	                                               1.0 + (std::numeric_limits<double>::epsilon() * 2),
	                                               std::numeric_limits<double>::epsilon() * 2,
	                                               std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(-1, -1, 0.000001, std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(-1.0000001, -1.0000001, 0.1, std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(-1.0,
	                                               -1.0 + std::numeric_limits<double>::epsilon(),
	                                               0.1,
	                                               std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(-1.0,
	                                               -1.0 + std::numeric_limits<double>::epsilon(),
	                                               std::numeric_limits<double>::epsilon(),
	                                               std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(-1.0,
	                                               -1.0 + (std::numeric_limits<double>::epsilon() * 2),
	                                               std::numeric_limits<double>::epsilon() * 2,
	                                               std::numeric_limits<double>::epsilon())),);

	class UnitCompareNotEqualTest : public UtilityTest, public WithParamInterface<UnitCompareTuple>
	{
	};

	TEST_P(UnitCompareNotEqualTest, UnitCompare_WhenValuesAreNotEquivalent_WillReturnFalse)
	{
		auto const lhs               = std::get<0>(GetParam());
		auto const rhs               = std::get<1>(GetParam());
		auto const max_diff          = std::get<2>(GetParam());
		auto const max_relative_diff = std::get<3>(GetParam());

		EXPECT_FALSE(units::detail::unit_compare(lhs, rhs, max_diff, max_relative_diff));
	}

	INSTANTIATE_TEST_CASE_P(,
	                        UnitCompareNotEqualTest,
	                        Values(std::make_tuple(1, 0, 0.000001, std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(1.0001, 1.0002, 0.00001, std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(1.0,
	                                               1.0 + (std::numeric_limits<double>::epsilon() * 2),
	                                               std::numeric_limits<double>::epsilon(),
	                                               std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(-1, 0, 0.000001, std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(-1.0001, -1.0002, 0.00001, std::numeric_limits<double>::epsilon()),
	                               std::make_tuple(-1.0,
	                                               -1.0 + (std::numeric_limits<double>::epsilon() * 2),
	                                               std::numeric_limits<double>::epsilon(),
	                                               std::numeric_limits<double>::epsilon())),);

	class IsUnitTest : public UtilityTest
	{
	};

	TEST_F(IsUnitTest, IsUnit_WhenGivenUnitType_WillReturnTrue) { EXPECT_TRUE(units::is_unit<base_unit>::value); }

	TEST_F(IsUnitTest, IsUnit_WhenGivenNonUnitType_WillReturnFalse) { EXPECT_FALSE(units::is_unit<double>::value); }
}
