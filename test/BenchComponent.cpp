#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/timer/timer.hpp>
#include <TalonConfig.hpp>
#include <ecs/ComponentStorage.hpp>
#include <ecs/ComponentStorageArray.hpp>
#include <ecs/ComponentStorageMap.hpp>
#include <ecs/ComponentStorageTree.hpp>
#include <ecs/TWorld.hpp>
#include <ecs/View.hpp>

USING_TALON_NS;

struct TestComponentArray : public TreeComponent {
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentArray";
    }
};

struct TestComponentMap : public TreeComponent {
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentMap";
    }
};

struct TestComponentArrayTree : public TreeComponent {
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentArrayTree";
    }
};

struct TestComponentMapTree : public TreeComponent {
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentMapTree";
    }
};

TALON_NS_BEGIN
COMPONENT_STORAGE_DEF(TestComponentArray, ComponentStorageArray);
COMPONENT_STORAGE_DEF(TestComponentMap, ComponentStorageMap);
COMPONENT_STORAGE_DEF_TREE(TestComponentArrayTree, ComponentStorageArray);
COMPONENT_STORAGE_DEF_TREE(TestComponentMapTree, ComponentStorageMap);
TALON_NS_END

// This is indicative of a rapidly changing hierarchy
template <typename Component>
void DynamicTest() {
    std::cout << std::endl << std::endl;
    std::cout << "Dynamic Test - " << Component::name() << std::endl;

    boost::timer::auto_cpu_timer timer;
    for (int j = 0; j < 1; j++) {
        TWorld<Component> world;
        for (EntityID i = 0; i < MaxEntityID; i++) {
            world.template createEntity<Component>();

            world.template for_each<Component>(boost::hana::fuse([](auto id, auto components){
                components[0_c]->idx++;
            }));
        }

        if (j == 0) {
            for (EntityID i = 0; i < MaxEntityID; i++) {
                EXPECT_EQ(world.template getComponentStorage<Component>().get(i)->idx, MaxEntityID - i);
            }
        }
    }
}

// This is indicative of a rapidly changing hierarchy
template <typename Component>
void DynamicTestView() {
    std::cout << std::endl << std::endl;
    std::cout << "Dynamic Test View - " << Component::name() << std::endl;

    boost::timer::auto_cpu_timer timer;
    for (int j = 0; j < 1; j++) {
        TWorld<Component> world;
        auto view = makeView<Component>(world, [](auto a) { return true; });

        for (EntityID i = 0; i < MaxEntityID; i++) {
            world.template createEntity<Component>();

            view.for_each(world, boost::hana::fuse([](auto id, auto components){
                components[0_c]->idx++;
            }));
        }

        if (j == 0) {
            for (EntityID i = 0; i < MaxEntityID; i++) {
                EXPECT_EQ(world.template getComponentStorage<Component>().get(i)->idx, MaxEntityID - i);
            }
        }
    }
}

// This is indicative of a largely static hierarchy
template <typename Component>
void StaticTest() {
    std::cout << std::endl << std::endl;
    std::cout << "Static Test - " << Component::name() << std::endl;

    boost::timer::auto_cpu_timer timer;
    for (int j = 0; j < 1; j++) {
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

        if (j == 0) {
            for (EntityID i = 0; i < MaxEntityID; i++) {
                EXPECT_EQ(world.template getComponentStorage<Component>().get(i)->idx, MaxEntityID);
            }
        }
    }
}

// This is indicative of a largely static hierarchy
template <typename Component>
void StaticTestView() {
    std::cout << std::endl << std::endl;
    std::cout << "Static Test View - " << Component::name() << std::endl;

    boost::timer::auto_cpu_timer timer;
    for (int j = 0; j < 1; j++) {
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

        if (j == 0) {
            for (EntityID i = 0; i < MaxEntityID; i++) {
                EXPECT_EQ(world.template getComponentStorage<Component>().get(i)->idx, MaxEntityID);
            }
        }
    }
}

TEST(BenchComponent, DynamicBenchmarks) {
    DynamicTest<TestComponentArray>();
    DynamicTest<TestComponentMap>();
    DynamicTest<TestComponentArrayTree>();
    DynamicTest<TestComponentMapTree>();

    DynamicTestView<TestComponentArray>();
    DynamicTestView<TestComponentMap>();
    DynamicTestView<TestComponentArrayTree>();
    DynamicTestView<TestComponentMapTree>();
}

TEST(BenchComponent, StaticBenchmarks) {
    StaticTest<TestComponentArray>();
    StaticTest<TestComponentMap>();
    StaticTest<TestComponentArrayTree>();
    StaticTest<TestComponentMapTree>();

    StaticTestView<TestComponentArray>();
    StaticTestView<TestComponentMap>();
    StaticTestView<TestComponentArrayTree>();
    StaticTestView<TestComponentMapTree>();
}