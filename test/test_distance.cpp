#include <gtest/gtest.h>

#include "distance.h"
using namespace testing;
using namespace std::literals::distance_literals;

namespace TestDistanceUnits
{
    class DistanceTest : public Test
    {
    protected:
        std::units::si::distance<long int> distance{1};
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

    class DistanceCastTest : public Test
    {
    };

    TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToMetres_WillYieldMetres)
    {
        auto metres = std::units::si::distance_cast<std::units::si::metres>(std::units::si::metres{1000});
        EXPECT_EQ(1000, metres.count());
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToKiloMetres_WillYieldKilometres)
    {
        auto kilometres = std::units::si::distance_cast<std::units::si::kilometres>(std::units::si::metres{1000});
        EXPECT_EQ(1, kilometres.count());
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromKilometresToMetres_WillYieldMetres)
    {
        auto metres = std::units::si::distance_cast<std::units::si::metres>(std::units::si::kilometres{1});
        EXPECT_EQ(1000, metres.count());
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromHalfUnitToWhole_WillRoundDown)
    {
        auto kilometres = std::units::si::distance_cast<std::units::si::kilometres>(999_m);
        EXPECT_EQ(0, kilometres.count());
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromMetresToYards_WillYieldYards)
    {
        auto yards = std::units::si::distance_cast<std::units::si::yards>(1_m);
        EXPECT_NEAR(1.09361, yards.count(), 0.00001);
    }

    TEST_F(DistanceCastTest, Cast_WhenCastingFromKiloMetresToYards_WillYieldYards)
    {
        auto yards = std::units::si::distance_cast<std::units::si::yards>(std::units::si::kilometres{1});
        EXPECT_NEAR(1093.6133, yards.count(), 0.0001);
    }

    class LiteralsTest : public Test
    {
    };

    TEST_F(LiteralsTest, MetresLiteral_WhenUsed_WillReturnMetresType)
    {
        constexpr auto is_same = std::is_same<std::units::si::metres, decltype(1_m)>::value;
        EXPECT_TRUE(is_same) << "Incompatible types";
    }
}
