add_library(signals Dummy.cpp ../vulkan_mem_alloc/vulkan_mem_alloc.hpp)
target_include_directories(signals PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})