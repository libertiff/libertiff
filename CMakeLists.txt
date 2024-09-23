cmake_minimum_required(VERSION 3.16)
project(libertiff_tests LANGUAGES CXX)
include(CTest)
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
   CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 9)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Winit-self -Wunused-parameter -Wmissing-declarations -Wlogical-op -Wshadow -Wmissing-include-dirs -Wformat -Werror=format-security -Wno-format-nonliteral -Werror=vla -Wno-clobbered -Wdate-time -Wnull-dereference -Wextra-semi -Wfloat-conversion -Wnon-virtual-dtor -Woverloaded-virtual -Wsuggest-override -fno-operator-names -Wzero-as-null-pointer-constant -Wold-style-cast")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
        CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Winit-self -Wunused-parameter -Wmissing-declarations -Wshadow -Wmissing-include-dirs -Wformat -Werror=format-security -Wno-format-nonliteral -Werror=vla -Wdate-time -Wnull-dereference -Wextra-semi -Wfloat-conversion -Wnon-virtual-dtor -Woverloaded-virtual -fno-operator-names -Wzero-as-null-pointer-constant -Wold-style-cast")
endif()

add_executable(demo demo.cpp)

add_subdirectory(tests)
