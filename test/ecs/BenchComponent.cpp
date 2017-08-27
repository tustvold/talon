#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/timer/timer.hpp>
#include <TalonConfig.hpp>
#include <ecs/TWorld.hpp>
#include <ecs/View.hpp>
#include "Components.hpp"

USING_TALON_NS;

#ifndef NDEBUG
static const int num_iterations = 1;
#else
static const int num_iterations = 100;
#endif

// This is indicative of a rapidly changing hierarchy
template <typename Component>
void DynamicTest() {
    std::cout << std::endl << std::endl;
    std::cout << "Dynamic Test - " << Component::name() << std::endl;

    boost::timer::auto_cpu_timer timer;
    for (int j = 0; j < num_iterations; j++) {
        TWorld<Component> world;
        for (EntityID i = 0; i < MaxEntityID; i++) {
            world.template createEntity<Component>();

            world.template for_each<Component>(boost::hana::fuse([](auto id, auto components){
                components[0_c]->idx++;
            }));
        }
    }
}

// This is indicative of a rapidly changing hierarchy
template <typename Component>
void DynamicTestView() {
    std::cout << std::endl << std::endl;
    std::cout << "Dynamic Test View - " << Component::name() << std::endl;

    boost::timer::auto_cpu_timer timer;
    for (int j = 0; j < num_iterations; j++) {
        TWorld<Component> world;
        auto view = makeView<Component>(world, [](auto a) { return true; });

        for (EntityID i = 0; i < MaxEntityID; i++) {
            world.template createEntity<Component>();

            view.for_each(world, boost::hana::fuse([](auto id, auto components){
                components[0_c]->idx++;
            }));
        }
    }
}

// This is indicative of a largely static hierarchy
template <typename Component>
void StaticTest() {
    std::cout << std::endl << std::endl;
    std::cout << "Static Test - " << Component::name() << std::endl;

    boost::timer::auto_cpu_timer timer;
    for (int j = 0; j < num_iterations; j++) {
        timer.stop();
        TWorld<Component> world;
        for (EntityID i = 0; i < MaxEntityID; i++) {
            world.template createEntity<Component>();
        }
        timer.resume();

        for (EntityID i = 0; i < MaxEntityID; i++) {
            world.template for_each<Component>(boost::hana::fuse([](auto id, auto components){
                components[0_c]->idx++;
            }));
        }
    }
}

// This is indicative of a largely static hierarchy
template <typename Component>
void StaticTestView() {
    std::cout << std::endl << std::endl;
    std::cout << "Static Test View - " << Component::name() << std::endl;

    boost::timer::auto_cpu_timer timer;
    for (int j = 0; j < num_iterations; j++) {
        timer.stop();
        TWorld<Component> world;

        for (EntityID i = 0; i < MaxEntityID; i++) {
            world.template createEntity<Component>();
        }
        timer.resume();
        auto view = makeView<Component>(world, [](auto a) { return true; });


        for (EntityID i = 0; i < MaxEntityID; i++) {
            view.for_each(world, boost::hana::fuse([](auto id, auto components){
                components[0_c]->idx++;
            }));
        }
    }
}

TEST(BenchComponent, DynamicBenchmarks) {
    DynamicTest<TestComponentArray>();
    DynamicTest<TestComponentMap>();
    DynamicTest<TestComponentFlatMap>();
    DynamicTest<TestComponentArrayTree>();
    DynamicTest<TestComponentMapTree>();
    DynamicTest<TestComponentFlatMapTree>();

    DynamicTestView<TestComponentArray>();
    DynamicTestView<TestComponentMap>();
    DynamicTestView<TestComponentFlatMap>();
    DynamicTestView<TestComponentArrayTree>();
    DynamicTestView<TestComponentMapTree>();
    DynamicTestView<TestComponentFlatMapTree>();
}

TEST(BenchComponent, StaticBenchmarks) {
    StaticTest<TestComponentArray>();
    StaticTest<TestComponentMap>();
    StaticTest<TestComponentFlatMap>();
    StaticTest<TestComponentArrayTree>();
    StaticTest<TestComponentMapTree>();
    StaticTest<TestComponentFlatMapTree>();

    StaticTestView<TestComponentArray>();
    StaticTestView<TestComponentMap>();
    StaticTestView<TestComponentFlatMap>();
    StaticTestView<TestComponentArrayTree>();
    StaticTestView<TestComponentMapTree>();
    StaticTestView<TestComponentFlatMapTree>();
}