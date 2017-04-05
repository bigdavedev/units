project (distance_ut)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions ("-std=c++1z")
endif()

set (TARGET_NAME ${PROJECT_NAME})

set (TEST_SOURCES
	test_distance.cpp
)

if (WIN32)
	# Prevent overriding the parent project's compiler/linker
	# settings on Windows
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

add_executable (${TARGET_NAME} ${TEST_SOURCES})
add_dependencies ( ${TARGET_NAME} gtest_main )

target_include_directories (${TARGET_NAME}
	PRIVATE
	${CMAKE_SOURCE_DIR}

	SYSTEM
	gtest
)

target_link_libraries (${TARGET_NAME}
	PRIVATE
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
	coveralls_setup (units.h
		${UNITS_ENABLE_COVERAGE_UPLOAD}
		"${CMAKE_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake"
	)
endif()
