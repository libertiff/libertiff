
find_package(GTest 1.10.0)
if(GTest_FOUND)
  option(USE_EXTERNAL_GTEST "Compile against external GTest" ON)
else()
  option(USE_EXTERNAL_GTEST "Compile against external GTest" OFF)
endif()

if(USE_EXTERNAL_GTEST)

    if(NOT GTest_FOUND)
        message(FATAL_ERROR "External GTest >= 1.10.0 not found")
    endif()
    message(STATUS "Using external GTest")

    # CMake < 3.20.0 uses GTest::GTest
    # CMake >= 3.20 uses GTest::gtest
    add_library(gtest_for_libertiff INTERFACE IMPORTED)
    if(NOT TARGET GTest::gtest)
        set_target_properties(gtest_for_libertiff PROPERTIES
                              INTERFACE_LINK_LIBRARIES "GTest::GTest")
    else()
        set_target_properties(gtest_for_libertiff PROPERTIES
                              INTERFACE_LINK_LIBRARIES "GTest::gtest")
    endif()

else()

    message(STATUS "Using internal GTest")

    #
    # Build Google Test
    #
    # Source https://github.com/google/googletest/blob/master/googletest/README.md
    # Download and unpack googletest at configure time
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt.in
      ${CMAKE_CURRENT_BINARY_DIR}/googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()
    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    #option(INSTALL_GTEST "Enable installation of googletest" OFF)
    add_subdirectory(
      ${CMAKE_CURRENT_BINARY_DIR}/googletest-src
      ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
      EXCLUDE_FROM_ALL)

    add_library(gtest_for_libertiff ALIAS gtest)

endif()  # USE_EXTERNAL_GTEST

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE gtest_for_libertiff)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
if(NOT MSVC AND CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(tests PRIVATE ${CMAKE_THREAD_LIBS_INIT})
endif()
add_test(NAME tests COMMAND tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
