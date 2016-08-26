#include <gtest/gtest.h>

#include "distance.h"
using namespace testing;
using namespace distance_literals;

namespace TestDistanceUnits
{
    class DistanceTest : public Test
    {
    protected:
        units::si::distance<long int> distance{1};
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
        units::si::metres metres = 1_km;
        EXPECT_EQ(1000, metres.count());
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

    TEST_F(DistanceTest, Meters_IsIdenticalToMetres)
    {
        constexpr bool is_same = std::is_same<units::si::metres, units::si::meters>::value;
        EXPECT_TRUE(is_same);
    }

    class DistanceCastTest : public Test
    {
    };

    TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToMetres_WillYieldMetres)
    {
        auto metres = units::si::distance_cast<units::si::metres>(1000_m);
        EXPECT_EQ(1000, metres.count());
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToKiloMetres_WillYieldKilometres)
    {
        auto kilometres = units::si::distance_cast<units::si::kilometres>(1000_m);
        EXPECT_EQ(1, kilometres.count());
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromKilometresToMetres_WillYieldMetres)
    {
        auto metres = units::si::distance_cast<units::si::metres>(1_km);
        EXPECT_EQ(1000, metres.count());
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromHalfUnitToWhole_WillRoundDown)
    {
        auto kilometres = units::si::distance_cast<units::si::kilometres>(999_m);
        EXPECT_EQ(0, kilometres.count());
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToYards_WillYieldYards)
    {
        auto yards = units::si::distance_cast<units::yards>(1_m);
        EXPECT_NEAR(1.09361, yards.count(), 0.00001);
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromKiloMetresToYards_WillYieldYards)
    {
        auto yards = units::si::distance_cast<units::yards>(1_km);
        EXPECT_NEAR(1093.6133, yards.count(), 0.0001);
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromKilometresToNanometres_WillYieldNanometres)
    {
        auto nanometres = units::si::distance_cast<units::si::nanometres>(1_km);
        EXPECT_EQ(1000000000000, nanometres.count());
    }

    class LiteralsTest : public Test
    {
    };

    TEST_F(LiteralsTest, MetresLiteral_WhenUsed_WillReturnMetresType)
    {
        constexpr auto is_same = std::is_same<units::si::metres, decltype(1_m)>::value;
        EXPECT_TRUE(is_same) << "Incompatible types";
    }

    TEST_F(LiteralsTest, KilometresLiteral_WhenUsed_WillReturnKilometresType)
    {
        constexpr auto is_same = std::is_same<units::si::kilometres, decltype(1_km)>::value;
        EXPECT_TRUE(is_same) << "Incompatible types";
    }
}
