#include <gtest/gtest.h>

#include "units.h"

#include <sstream>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

using namespace distance_literals;

using testing::Test;
using testing::Types;

namespace TestStreamOutput
{
	namespace
	{
		auto literal_to_string_lookup = std::unordered_map<std::type_index, std::string>
		    {{std::type_index{typeid(units::nanometres)}, std::string{"nm"}},
		     {std::type_index{typeid(units::micrometres)}, std::string{"um"}},
		     {std::type_index{typeid(units::millimetres)}, std::string{"mm"}},
		     {std::type_index{typeid(units::centimetres)}, std::string{"cm"}},
		     {std::type_index{typeid(units::decimeters)}, std::string{"dm"}},
		     {std::type_index{typeid(units::metres)}, std::string{"m"}},
		     {std::type_index{typeid(units::kilometres)}, std::string{"km"}},
		     {std::type_index{typeid(units::thous)}, std::string{"th"}},
		     {std::type_index{typeid(units::inches)}, std::string{"in"}},
		     {std::type_index{typeid(units::links)}, std::string{"li"}},
		     {std::type_index{typeid(units::feet)}, std::string{"ft"}},
		     {std::type_index{typeid(units::yards)}, std::string{"yd"}},
		     {std::type_index{typeid(units::rods)}, std::string{"rd"}},
		     {std::type_index{typeid(units::chains)}, std::string{"ch"}},
		     {std::type_index{typeid(units::furlongs)}, std::string{"fur"}},
		     {std::type_index{typeid(units::miles)}, std::string{"mi"}},
		     {std::type_index{typeid(units::leagues)}, std::string{"lea"}},
		     {std::type_index{typeid(units::fathoms)}, std::string{"ftm"}},
		     {std::type_index{typeid(units::cables)}, std::string{"cb"}},
		     {std::type_index{typeid(units::nautical_miles)}, std::string{"nmi"}},
		     {std::type_index{typeid(units::earth_radii)}, std::string{"R"}},
		     {std::type_index{typeid(units::lunar_distances)}, std::string{"LD"}},
		     {std::type_index{typeid(units::astronimical_units)}, std::string{"AU"}},
		     {std::type_index{typeid(units::light_years)}, std::string{"ly"}},
		     {std::type_index{typeid(units::parsecs)}, std::string{"pc"}}};
	}

	template <typename T>
	class StreamOutputTest : public Test
	{
	protected:
		std::stringstream buffer;
	};

	TYPED_TEST_CASE_P(StreamOutputTest);

	TYPED_TEST_P(StreamOutputTest, StreamInsertionOperator_WillFormatStringCorrectly)
	{
		buffer << TypeParam{1};

		std::stringstream buffer_expected{""};
		buffer_expected << "1" << literal_to_string_lookup[std::type_index{typeid(TypeParam)}];
		EXPECT_EQ(buffer_expected.str(), buffer.str());
	}

	using DistanceTuple = Types<units::nanometres,
	                            units::micrometres,
	                            units::millimetres,
	                            units::centimetres,
	                            units::decimetres,
	                            units::metres,
	                            units::kilometres,
	                            units::thous,
	                            units::inches,
	                            units::links,
	                            units::feet,
	                            units::yards,
	                            units::rods,
	                            units::chains,
	                            units::furlongs,
	                            units::miles,
	                            units::leagues,
	                            units::fathoms,
	                            units::cables,
	                            units::nautical_miles,
	                            units::earth_radii,
	                            units::lunar_distances,
	                            units::astronimical_units,
	                            units::light_years,
	                            units::parsecs>;

	REGISTER_TYPED_TEST_CASE_P(StreamOutputTest, StreamInsertionOperator_WillFormatStringCorrectly);
	INSTANTIATE_TYPED_TEST_CASE_P(Distance, StreamOutputTest, DistanceTuple);
}
