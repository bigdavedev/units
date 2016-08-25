cmake_minimum_required ( VERSION 2.8 )

set ( CMAKE_MODULE_PATH
      ${CMAKE_MODULE_PATH}
      ${PROJECT_SOURCE_DIR}/cmake
      ${PROJECT_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake )

add_definitions ( "-std=c++14" )

# Here be test!
include ( gtest )
include ( Coveralls )
add_gtest ()

coveralls_turn_on_coverage ()


enable_testing ()

add_subdirectory ( test )
add_subdirectory ( coverage )
