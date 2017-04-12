project (test_units)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions ("-std=c++1z")
	add_definitions ("-fno-elide-constructors")
	add_definitions ("-fno-inline")
	add_definitions ("-Wall")
	add_definitions ("-Wextra")
	add_definitions ("-Wpedantic")

	set (EXTRA_LIBRARIES -pthread)
else ()
	add_definitions ("/W4")
	add_definitions ("/WX")
	add_definitions ("/Zl")
	add_definitions ("/Za")

	set (EXTRA_LIBRARIES)
endif()

include_directories (SYSTEM ${CMAKE_SOURCE_DIR}/external/googletest/googletest/include)
include_directories (${CMAKE_SOURCE_DIR})

if (UNITS_ENABLE_COVERAGE AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set (CMAKE_MODULE_PATH
		${CMAKE_MODULE_PATH}
		${CMAKE_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake
	)
	include (Coveralls)
	coveralls_turn_on_coverage ()
endif()

macro (units_add_test TEST_NAME SOURCE_FILE)
	set (TARGET_NAME ${TEST_NAME})
	add_executable (${TARGET_NAME} ${SOURCE_FILE})
	add_dependencies (${TARGET_NAME} gtest_main)

	target_link_libraries (${TARGET_NAME}
		${EXTRA_LIBRARIES}
		gtest_main
	)

	add_test (${TARGET_NAME}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}
	)
endmacro ()

units_add_test (test_units test_units.cpp)
units_add_test (test_distance test_distance.cpp)
units_add_test (test_streams test_streams.cpp)
units_add_test (test_mass test_mass.cpp)

