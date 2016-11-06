cmake_minimum_required ( VERSION 2.8 )

project ( units )

set ( CMAKE_MODULE_PATH
      ${CMAKE_MODULE_PATH}
      ${PROJECT_SOURCE_DIR}/cmake
      ${PROJECT_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake )

execute_process ( COMMAND git describe --always --tags
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  OUTPUT_VARIABLE GIT_REPO_VERSION )

message ( STATUS "version: " ${GIT_REPO_VERSION} )

## Generate Doxygen
find_package ( Doxygen )
if ( DOXYGEN_FOUND )
	configure_file ( doxygen/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY )
	add_custom_target ( doc
		                ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
		                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
		                COMMENT "Generating Doxygen files..." VERBATIM )
endif ()

file(REMOVE_RECURSE ${PROJECT_BINARY_DIR}/*.gcda)

# Download and unpack googletest at configure time
configure_file(cmake/gtest.cmake.in external/googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	include ( Coveralls )
	coveralls_turn_on_coverage ()
	add_subdirectory ( coverage )
endif()

enable_testing ()

add_subdirectory ( test )
