

cmake_minimum_required(VERSION 3.8)
project(talon)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

MESSAGE(STATUS "${CMAKE_CXX_COMPILER_ID}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # Update if necessary
    MESSAGE(STATUS "Enabling Warnings")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wno-long-long -pedantic -Wno-unused-local-typedefs")
else()
    MESSAGE(WARNING "Wasn't able to enable pedantic warnings")
endif()



find_package(GLFW3 REQUIRED)
find_package(TBB REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(external/signals)

add_subdirectory(src)
add_subdirectory(example)


