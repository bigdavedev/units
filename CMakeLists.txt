cmake_minimum_required ( VERSION 2.8 )

set ( PROJECT_NAME Units )

set ( CMAKE_MODULE_PATH
      ${CMAKE_MODULE_PATH}
      ${PROJECT_SOURCE_DIR}/cmake
      ${PROJECT_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake )

add_definitions ( "-std=c++1z" )

execute_process ( COMMAND git describe --always --tags
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

# Here be test!
include ( gtest )
include ( Coveralls )
add_gtest ()

coveralls_turn_on_coverage ()


enable_testing ()

add_subdirectory ( test )
add_subdirectory ( coverage )
