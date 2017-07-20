#include <gtest/gtest.h>

#include <typeinfo>

#include "units.h"

using testing::Test;

using namespace distance_literals;

namespace TestAreaUnits
{

	class AreaBaseTest : public Test
	{
	};

	TEST_F(AreaBaseTest, MetricMultiplication_WhenUnitsAreCompatible_WillReturnUnitsSquared)
	{
		constexpr auto result = 1_m * 1_m;

		std::cout << result << std::endl;

		EXPECT_EQ(units::square_centimetres{10000}, result);
		EXPECT_TRUE(typeid(result) == typeid(units::square_metres));
	}

	TEST_F(AreaBaseTest, ImperialMultiplication_WhenUnitsAreCompatible_WillReturnUnitsSquared)
	{
		constexpr auto result = 2_ft * 2_ft;

		EXPECT_EQ(units::square_feet{4}, result);
		EXPECT_TRUE(typeid(result) == typeid(units::square_feet));
	}

	TEST_F(AreaBaseTest, Multiplication_WhenUnitsAreConvertible_WillReturnUnitsSquared)
	{
		auto result = 2_m * units::feet{2_m};

		EXPECT_EQ(units::square_metres{4}, result);
		EXPECT_TRUE(typeid(result) == typeid(units::square_metres));
	}

	TEST_F(AreaBaseTest, Arithmetic_WhenComplex_WillResultInCorrectType)
	{
		units::square_metres result = 2_m * 2_m + 4_cm * 4_cm;

		EXPECT_EQ(units::square_metres{4.0016}, result);
		EXPECT_TRUE(typeid(result) == typeid(units::square_metres));
	}
}
