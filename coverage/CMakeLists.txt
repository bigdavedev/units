
include_directories ( ${CMAKE_SOURCE_DIR} )
set ( SOURCES distance.h )

coveralls_setup ( "${SOURCES}"
                  ON
                  "${CMAKE_SOURCE_DIR}/external/JoakimSoderberg/coveralls-cmake/cmake" )
