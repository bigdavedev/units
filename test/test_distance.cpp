#include <gtest/gtest.h>

#include "distance.h"
using namespace testing;
using namespace std::literals::distance_literals;

namespace TestDistanceUnits
{
    class DistanceTest : public Test
    {
    protected:
        std::units::si::distance<long int> distance{0};
    };

    TEST_F(DistanceTest, Constructor_WhenInvoked_WillInitialise)
    {
        EXPECT_EQ(0, distance.count());
    }

    class LiteralsTest : public Test
    {
    };

    TEST_F(LiteralsTest, MetresLiteral_WhenUsed_WillReturnMetresType)
    {
        static_assert(std::is_same<std::units::si::metres, decltype(1_m)>::value,
                      "Incompatible types");
        EXPECT_TRUE(true);
    }
}
