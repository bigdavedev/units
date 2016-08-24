#include <gtest/gtest.h>

#include "distance.h"
using namespace testing;

namespace TestDistanceUnits
{
    class DistanceTest : public Test
    {
    protected:
        std::units::si::distance<int> distance{0};
    };

    TEST_F(DistanceTest, Constructor_WhenInvoked_WillInitialise)
    {
        EXPECT_EQ(0, distance.count());
    }
}
