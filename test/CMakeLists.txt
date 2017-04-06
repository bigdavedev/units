project (distance_ut)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions ("-std=c++1z")
endif()

set (TARGET_NAME ${PROJECT_NAME})

set (TEST_SOURCES
	test_distance.cpp
)

add_executable (${TARGET_NAME} ${TEST_SOURCES})
add_dependencies ( ${TARGET_NAME} gtest_main )

include_directories (SYSTEM ${CMAKE_SOURCE_DIR}/external/googletest/googletest/include)
include_directories (${CMAKE_SOURCE_DIR})

target_link_libraries (${TARGET_NAME}
	-pthread
	gtest_main
)

add_test (${TARGET_NAME}
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}
)

if (UNITS_ENABLE_COVERAGE)
	set (CMAKE_MODULE_PATH
		${CMAKE_MODULE_PATH}
		${CMAKE_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake
	)
	include (Coveralls)
	coveralls_turn_on_coverage ()
	
	get_filename_component (UNITS ../units.h ABSOLUTE)
	get_filename_component (TEST_DISTANCE test_distance.cpp ABSOLUTE)
	
	set (FILES ${UNITS} ${TEST_DISTANCE})
	
	coveralls_setup (${FILES}
		${UNITS_ENABLE_COVERAGE_UPLOAD}
		"${CMAKE_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake"
	)
endif()
