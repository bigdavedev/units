cmake_minimum_required (VERSION 2.8)

project (units)

option (UNITS_BUILD_DOCUMENTATION "Build documentation with doxygen" ON)
option (UNITS_BUILD_TESTS "Build unit test suite" ON)
option (UNITS_ENABLE_COVERAGE "Enable coverage with Coveralls" OFF)
option (UNITS_ENABLE_COVERAGE_UPLOAD "Enable uploading of coverage data to coveralls.io" OFF)

## Generate Doxygen
if (UNITS_BUILD_DOCUMENTATION)
	find_package (Doxygen)
	if (DOXYGEN_FOUND)
		execute_process (COMMAND git describe --always --tags
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE GIT_REPO_VERSION
		)

		configure_file (doxygen/Doxyfile.in
			${CMAKE_BINARY_DIR}/Doxyfile
			@ONLY
		)

		add_custom_target (doc ALL
			${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
			COMMENT "Generating Doxygen files..."
			VERBATIM
		)
	endif ()
endif ()

if (UNITS_BUILD_TESTS)
	set (BUILD_GTEST ON CACHE BOOL "" FORCE)
	set (BUILD_GMOCK OFF CACHE BOOL "" FORCE)
	if (WIN32)
		# Prevent overriding the parent project's compiler/linker
		# settings on Windows
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	endif ()

	add_subdirectory (${PROJECT_SOURCE_DIR}/external/googletest)

	enable_testing ()
	add_subdirectory (test)

	if (UNITS_ENABLE_COVERAGE)
		set (CMAKE_MODULE_PATH
			${CMAKE_MODULE_PATH}
			${CMAKE_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake
		)
		include (Coveralls)
		coveralls_turn_on_coverage ()

		set (FILES units.h)
		message (STATUS "Measuring coverage for ${FILES}")

		coveralls_setup ("${FILES}"
		                 ${UNITS_ENABLE_COVERAGE_UPLOAD}
		                 ${CMAKE_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake
		)
	endif()
endif ()

