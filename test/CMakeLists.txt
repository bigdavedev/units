project ( distance_ut )
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions ( "-std=c++1z" )
endif()

set ( TARGET_NAME ${PROJECT_NAME} )

set ( TEST_SOURCES test_distance.cpp ../units.h )

add_executable ( ${TARGET_NAME} ${SOURCES} ${TEST_SOURCES} )
add_dependencies ( ${TARGET_NAME} googletest )

include_directories ( ${CMAKE_SOURCE_DIR}
					  ${CMAKE_BINARY_DIR}/googletest-src/googletest/include )
target_link_libraries ( ${TARGET_NAME} -pthread gtest_main )
add_custom_target ( runtests COMMAND ./${TARGET_NAME} )
add_test ( ${TARGET_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME} )
