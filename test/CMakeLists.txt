enable_testing()

SET(SOURCE_FILES main.cpp TestComponent.cpp BenchComponent.cpp TestTreeComponent.cpp BenchTreeComponent.cpp Components.hpp)

add_executable(talon.tests ${SOURCE_FILES})

target_include_directories(talon.tests PRIVATE ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
target_link_libraries(talon.tests talon ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES} boost_timer)