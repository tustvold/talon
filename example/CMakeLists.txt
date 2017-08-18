set(SOURCE_FILES main.cpp ../external/signals/Signal.hpp ../external/signals/dummy.cpp)
add_executable(imgui_glfw_test ${SOURCE_FILES})

target_link_libraries(imgui_glfw_test talon)