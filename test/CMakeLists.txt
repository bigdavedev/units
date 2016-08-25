project ( distance_ut )
add_definitions ( "-std=c++14" )

set ( TARGET_NAME ${PROJECT_NAME} )

set ( TEST_SOURCES test_distance.cpp )
add_executable ( ${TARGET_NAME} ${SOURCES} ${TEST_SOURCES} )
add_dependencies ( ${TARGET_NAME} googletest )

include_directories ( ${CMAKE_SOURCE_DIR} )
target_link_libraries ( ${TARGET_NAME} -pthread ${GTEST_LIBRARIES} )
add_custom_target ( runtests COMMAND ./${TARGET_NAME} )
add_test ( ${TARGET_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME} )
