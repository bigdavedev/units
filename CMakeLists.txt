cmake_minimum_required (VERSION 2.8)

project (units)

option (UNITS_BUILD_DOCUMENTATION "Build documentation with doxygen" ON)
option (UNITS_BUILD_TESTS "Build unit test suite" ON)
option (UNITS_ENABLE_COVERAGE "Enable coverage with Coveralls" OFF)

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
	add_subdirectory (${PROJECT_SOURCE_DIR}/external/googletest)

	enable_testing ()
	add_subdirectory (test)
endif ()

