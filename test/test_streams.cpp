#include <gtest/gtest.h>

#include "units.h"

using namespace distance_literals;

using testing::Test;

#include <sstream>

namespace TestStreamOutput
{
	class StreamOutputTest : public Test
	{
	protected:
		std::stringstream buffer;
	};

	TEST_F(StreamOutputTest, StreamInsertionOperator_WillFormatStringCorrectly)
	{
		buffer << 1_m;
		EXPECT_STREQ("1", buffer.str().c_str());
	}
}
