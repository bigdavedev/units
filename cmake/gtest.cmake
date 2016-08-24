
macro ( add_gtest )
    # Setup testing environment
    include( ExternalProject )
    set_directory_properties( PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/external )

    # Add gtest
    ExternalProject_Add(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        TIMEOUT 10
        # Force separate output paths for debug and release builds to allow easy
        # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
                   -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
                   -Dgtest_force_shared_crt=ON
                   -DBUILD_GTEST=ON
                   -DBUILD_GMOCK=OFF
        # Disable install step
        INSTALL_COMMAND ""
        # Wrap download, configure and build steps in a script to log output
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON )
    ExternalProject_Get_Property( googletest source_dir )
    include_directories( ${source_dir}/googletest/include )
    ExternalProject_Get_Property( googletest binary_dir )

    set ( GTEST_LIBRARIES "" )
    message ( STATUS "Build type: ${CMAKE_BUILD_TYPE}" )
    if ( CMAKE_BUILD_TYPE MATCHES Debug )
        set ( GTEST_LIBRARIES ${binary_dir}/googletest/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a
                              ${binary_dir}/googletest/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a )
    else ( )
        set ( GTEST_LIBRARIES ${binary_dir}/googletest/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a
                              ${binary_dir}/googletest/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a )
    endif()
endmacro()
