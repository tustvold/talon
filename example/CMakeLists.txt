set(SOURCE_FILES main.cpp ../external/signals/Signal.hpp ../external/signals/Dummy.cpp TestScene.cpp TestScene.hpp ../config/default/TalonTypes.hpp ../config/default/DescriptorConfig.hpp TestMaterial.cpp TestMaterial.hpp)
add_executable(imgui_glfw_test ${SOURCE_FILES})

target_link_libraries(imgui_glfw_test talon)