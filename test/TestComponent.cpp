#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Component.hpp"

#include <boost/timer/timer.hpp>

USING_TALON_NS;

struct TestComponentTransform {
    TestComponentTransform() = default;
    TestComponentTransform(const TestComponentTransform &) = delete;
    TestComponentTransform(TestComponentTransform &&) = delete;
    TestComponentTransform &operator=(const TestComponentTransform &) = delete;
    TestComponentTransform &operator=(TestComponentTransform &&) = delete;

    static constexpr const char *string() {
        return "TestComponentTransform";
    }

    MOCK_METHOD0(testTransform, void());
};

struct TestComponentMeshFilter {
    TestComponentMeshFilter() = default;
    TestComponentMeshFilter(const TestComponentMeshFilter &) = delete;
    TestComponentMeshFilter(TestComponentMeshFilter &&) = delete;
    TestComponentMeshFilter &operator=(const TestComponentMeshFilter &) = delete;
    TestComponentMeshFilter &operator=(TestComponentMeshFilter &&) = delete;

    static constexpr const char *string() {
        return "TestComponentMeshFilter";
    }

    MOCK_METHOD0(testMeshFilter, void());
};

struct TestComponentMeshFilter2 {
    TestComponentMeshFilter2() = default;
    TestComponentMeshFilter2(const TestComponentMeshFilter2 &) = delete;
    TestComponentMeshFilter2(TestComponentMeshFilter2 &&) = delete;
    TestComponentMeshFilter2 &operator=(const TestComponentMeshFilter2 &) = delete;
    TestComponentMeshFilter2 &operator=(TestComponentMeshFilter2 &&) = delete;

    static constexpr const char *string() {
        return "TestComponentMeshFilter2";
    }
};

TALON_NS_BEGIN

template<>
struct ComponentStorage<TestComponentTransform> : ComponentArrayStorage<TestComponentTransform> {};
template<>
struct ComponentStorage<TestComponentMeshFilter> : ComponentDictStorage<TestComponentMeshFilter> {};

TALON_NS_END

TEST(TestComponent, TestTransform) {
    World<TestComponentTransform, TestComponentMeshFilter> world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 50; i++) {
        ids.push_back(world.createEntity<TestComponentTransform>());
    }

    for (auto id : ids) {
        TestComponentTransform& component = world.getComponentStorage<TestComponentTransform>().get(id);
        EXPECT_CALL(component, testTransform()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID, boost::hana::tuple<TestComponentTransform*> components) {
        components[0_c]->testTransform();
    });

    for (int i = 0; i < 100; i++)
        world.for_each<TestComponentTransform>(increment);
}

TEST(TestComponent, TestMeshFilter) {
    World<TestComponentTransform, TestComponentMeshFilter> world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 50; i++) {
        ids.push_back(world.createEntity<TestComponentMeshFilter>());
    }

    for (auto id : ids) {
        TestComponentMeshFilter& component = world.getComponentStorage<TestComponentMeshFilter>().get(id);
        EXPECT_CALL(component, testMeshFilter()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID, boost::hana::tuple<TestComponentMeshFilter*> components) {
        components[0_c]->testMeshFilter();
    });

    for (int i = 0; i < 100; i++)
        world.for_each<TestComponentMeshFilter>(increment);
}

TEST(TestComponent, TestMultiple) {
    World<TestComponentTransform, TestComponentMeshFilter> world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 50; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (auto id : ids) {
        TestComponentMeshFilter& componentMeshFilter = world.getComponentStorage<TestComponentMeshFilter>().get(id);
        EXPECT_CALL(componentMeshFilter, testMeshFilter()).Times(100);

        TestComponentTransform& componentTransform = world.getComponentStorage<TestComponentTransform>().get(id);
        EXPECT_CALL(componentTransform, testTransform()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID, boost::hana::tuple<TestComponentMeshFilter*, TestComponentTransform*> components) {
        components[0_c]->testMeshFilter();
        components[1_c]->testTransform();
    });

    for (int i = 0; i < 100; i++)
        world.for_each<TestComponentMeshFilter, TestComponentTransform>(increment);
}