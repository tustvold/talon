enable_testing()

SET(SOURCE_FILES main.cpp ecs/TestComponent.cpp ecs/BenchComponent.cpp ecs/TestTreeComponent.cpp ecs/BenchTreeComponent.cpp ecs/Components.hpp ecs/TestCategoryComponent.cpp ecs/TestDependency.cpp TestDescriptor.cpp mock/MockApplication.cpp mock/MockApplication.hpp mock/MockCommandPool.hpp mock/MockDeviceManager.hpp mock/MockInstanceManager.hpp mock/MockMemoryAllocator.hpp mock/MockSurfaceManager.hpp mock/MockWindowManager.hpp)

add_executable(talon.tests ${SOURCE_FILES})

target_include_directories(talon.tests PRIVATE ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
target_link_libraries(talon.tests talon ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES} boost_timer)