#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <TalonTypes.hpp>
#include <ecs/ComponentStorageArray.hpp>
#include <ecs/ComponentStorageMap.hpp>
#include <ecs/ComponentStorageTree.hpp>
#include <ecs/View.hpp>
#include "ecs/TWorld.hpp"

USING_TALON_NS;

struct TestComponentTransform {
    TestComponentTransform() = default;
    TestComponentTransform(const TestComponentTransform &other) {
        idx = other.idx;
        other.testTransform();
    }
    TestComponentTransform(TestComponentTransform &&) = delete;
    TestComponentTransform &operator=(const TestComponentTransform &) = delete;
    TestComponentTransform &operator=(TestComponentTransform &&) = delete;

    static constexpr const char *string() {
        return "TestComponentTransform";
    }

    int idx = 0;

    MOCK_CONST_METHOD0(testTransform, void());
};

struct TestComponentMeshFilter {
    TestComponentMeshFilter() = default;
    TestComponentMeshFilter(const TestComponentMeshFilter &) = delete;
    TestComponentMeshFilter(TestComponentMeshFilter &&other) {
        idx = other.idx;
        other.testMeshFilter();
    }
    TestComponentMeshFilter &operator=(const TestComponentMeshFilter &) = delete;
    TestComponentMeshFilter &operator=(TestComponentMeshFilter &&) = delete;

    int idx = 0;

    static constexpr const char *string() {
        return "TestComponentMeshFilter";
    }
    MOCK_CONST_METHOD0(testMeshFilter, void());
};

struct TestComponentMeshFilter2 {
    TestComponentMeshFilter2() = default;
    TestComponentMeshFilter2(const TestComponentMeshFilter2 &other) {
        idx = other.idx;
        other.testMeshFilter2();
    }
    TestComponentMeshFilter2(TestComponentMeshFilter2 &&) = delete;
    TestComponentMeshFilter2 &operator=(const TestComponentMeshFilter2 &) = delete;
    TestComponentMeshFilter2 &operator=(TestComponentMeshFilter2 &&) = delete;

    int idx = 0;

    static constexpr const char *string() {
        return "TestComponentMeshFilter2";
    }

    MOCK_CONST_METHOD0(testMeshFilter2, void());
};

TALON_NS_BEGIN

COMPONENT_STORAGE_DEF(TestComponentTransform, ComponentStorageArray);

COMPONENT_STORAGE_DEF(TestComponentMeshFilter, ComponentStorageMap);

COMPONENT_STORAGE_DEF_TREE(TestComponentMeshFilter2, ComponentStorageArray);

TALON_NS_END

using TestWorldType = TWorld<TestComponentTransform, TestComponentMeshFilter, TestComponentMeshFilter2>;

TEST(TestComponent, TestTransform) {
    TestWorldType world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 50; i++) {
        ids.push_back(world.createEntity<TestComponentTransform>());
    }

    for (auto id : ids) {
        TestComponentTransform *component = world.getComponentStorage<TestComponentTransform>().get(id);
        EXPECT_CALL(*component, testTransform()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID, boost::hana::tuple<TestComponentTransform *> components) {
        components[0_c]->testTransform();
    });

    for (int i = 0; i < 100; i++)
        world.for_each<TestComponentTransform>(increment);
}

TEST(TestComponent, TestMeshFilter) {
    TestWorldType world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 50; i++) {
        ids.push_back(world.createEntity<TestComponentMeshFilter>());
    }

    for (auto id : ids) {
        TestComponentMeshFilter *component = world.getComponentStorage<TestComponentMeshFilter>().get(id);
        EXPECT_CALL(*component, testMeshFilter()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID, boost::hana::tuple<TestComponentMeshFilter *> components) {
        components[0_c]->testMeshFilter();
    });

    for (int i = 0; i < 100; i++)
        world.for_each<TestComponentMeshFilter>(increment);
}

TEST(TestComponent, TestMultiple) {
    TestWorldType world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 25; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentTransform>();
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentMeshFilter>();
    }

    for (int i = 0; i < 25; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (auto id : ids) {
        TestComponentMeshFilter *componentMeshFilter = world.getComponentStorage<TestComponentMeshFilter>().get(id);
        EXPECT_CALL(*componentMeshFilter, testMeshFilter()).Times(100);

        TestComponentTransform *componentTransform = world.getComponentStorage<TestComponentTransform>().get(id);
        EXPECT_CALL(*componentTransform, testTransform()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID,
                                          boost::hana::tuple<TestComponentMeshFilter *,
                                                             TestComponentTransform *> components) {
        components[0_c]->testMeshFilter();
        components[1_c]->testTransform();
    });

    for (int i = 0; i < 100; i++)
        world.for_each<TestComponentMeshFilter, TestComponentTransform>(increment);
}

TEST(TestComponent, TestMultiple2) {
    TestWorldType world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 25; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentTransform>();
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentMeshFilter>();
    }

    for (int i = 0; i < 25; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentTransform>();
    }

    for (auto id : ids) {
        TestComponentMeshFilter *componentMeshFilter = world.getComponentStorage<TestComponentMeshFilter>().get(id);
        EXPECT_CALL(*componentMeshFilter, testMeshFilter()).Times(100);

        TestComponentTransform *componentTransform = world.getComponentStorage<TestComponentTransform>().get(id);
        EXPECT_CALL(*componentTransform, testTransform()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID,
                                          boost::hana::tuple<TestComponentMeshFilter *,
                                                             TestComponentTransform *> components) {
        components[0_c]->testMeshFilter();
        components[1_c]->testTransform();
    });

    for (int i = 0; i < 100; i++)
        world.for_each<TestComponentMeshFilter, TestComponentTransform>(increment);
}


TEST(TestComponent, TestMultiple3) {
    TestWorldType world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 25; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentTransform>();
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentMeshFilter>();
    }

    for (int i = 0; i < 25; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentMeshFilter>();
    }

    for (auto id : ids) {
        TestComponentMeshFilter *componentMeshFilter = world.getComponentStorage<TestComponentMeshFilter>().get(id);
        EXPECT_CALL(*componentMeshFilter, testMeshFilter()).Times(100);

        TestComponentTransform *componentTransform = world.getComponentStorage<TestComponentTransform>().get(id);
        EXPECT_CALL(*componentTransform, testTransform()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID,
                                          boost::hana::tuple<TestComponentMeshFilter *,
                                                             TestComponentTransform *> components) {
        components[0_c]->testMeshFilter();
        components[1_c]->testTransform();
    });

    for (int i = 0; i < 100; i++)
        world.for_each<TestComponentMeshFilter, TestComponentTransform>(increment);
}

TEST(TestComponent, TestView) {
    TestWorldType world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 50; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (auto id : ids) {
        TestComponentTransform *component = world.getComponentStorage<TestComponentTransform>().get(id);
        EXPECT_CALL(*component, testTransform()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID, boost::hana::tuple<TestComponentTransform *> components) {
        components[0_c]->testTransform();
    });

    auto view1 = makeView<TestComponentTransform>(world);

    for (int i = 0; i < 100; i++)
        view1.for_each(world, increment);
}

TEST(TestComponent, TestViewMutate) {
    TestWorldType world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 50; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (auto id : ids) {
        TestComponentTransform *component = world.getComponentStorage<TestComponentTransform>().get(id);
        EXPECT_CALL(*component, testTransform()).Times(2);
    }

    auto increment = boost::hana::fuse([](auto entityID, boost::hana::tuple<TestComponentTransform *> components) {
        components[0_c]->testTransform();
    });

    auto view1 = makeView<TestComponentTransform>(world);
    view1.for_each(world, increment);

    auto new_id = world.createEntity<TestComponentTransform, TestComponentMeshFilter>();
    TestComponentTransform *component = world.getComponentStorage<TestComponentTransform>().get(new_id);
    EXPECT_CALL(*component, testTransform()).Times(1);

    view1.for_each(world, increment);
}

TEST(TestComponent, TestViewMultiple) {
    TestWorldType world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 25; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentTransform>();
    }

    for (int i = 0; i < 100; i++) {
        world.createEntity<TestComponentMeshFilter>();
    }

    for (int i = 0; i < 25; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    for (auto id : ids) {
        TestComponentMeshFilter *componentMeshFilter = world.getComponentStorage<TestComponentMeshFilter>().get(id);
        EXPECT_CALL(*componentMeshFilter, testMeshFilter()).Times(100);

        TestComponentTransform *componentTransform = world.getComponentStorage<TestComponentTransform>().get(id);
        EXPECT_CALL(*componentTransform, testTransform()).Times(100);
    }

    auto increment = boost::hana::fuse([](auto entityID,
                                          boost::hana::tuple<TestComponentMeshFilter *,
                                                             TestComponentTransform *> components) {
        components[0_c]->testMeshFilter();
        components[1_c]->testTransform();
    });

    auto view1 = makeView<TestComponentMeshFilter, TestComponentTransform>(world);

    for (int i = 0; i < 100; i++)
        view1.for_each(world, increment);
}

TEST(TestComponent, TestViewPredicate) {
    TestWorldType world;

    std::vector<EntityID> ids;

    for (int i = 0; i < 50; i++) {
        ids.push_back(world.createEntity<TestComponentTransform, TestComponentMeshFilter>());
    }

    auto target_id = ids[23];

    for (auto id : ids) {
        TestComponentTransform *component = world.getComponentStorage<TestComponentTransform>().get(id);
        if (id == target_id)
            EXPECT_CALL(*component, testTransform()).Times(100);
        else
            EXPECT_CALL(*component, testTransform()).Times(0);
    }

    auto increment =
        boost::hana::fuse([target_id](auto entityID, boost::hana::tuple<TestComponentTransform *> components) {
            components[0_c]->testTransform();
        });

    auto view1 = makeView<TestComponentTransform>(world,
                                                  boost::hana::fuse([target_id](auto id,
                                                                                boost::hana::tuple<
                                                                                    TestComponentTransform *> components) {
                                                      return id == target_id;
                                                  }));

    for (int i = 0; i < 100; i++)
        view1.for_each(world, increment);
}

TEST(TestComponent, TestComponentStorageTreeMap) {
    ComponentStorageTree<TestComponentTransform, ComponentStorageMap> storage;

    storage.add(1);
    storage.add(5);
    storage.addWithParent(2, 5);
    storage.add(3);
    storage.addWithParent(4, 5);

    std::vector<EntityID> normalIterate;
    std::vector<EntityID> normalIterate_expected = {1, 2, 3, 4, 5};

    std::vector<EntityID> treeIterate;
    std::vector<EntityID> treeIterate_expected = {1, 3, 5, 2, 4};

    std::vector<EntityID> parents;
    std::vector<EntityID> parents_expected = {EntityIDInvalid, EntityIDInvalid, EntityIDInvalid, 5, 5};

    storage.for_each([&normalIterate](auto component) {
        normalIterate.push_back(component[0_c]);
    });

    storage.tree_for_each([&treeIterate, &parents](auto component) {
        treeIterate.push_back(component[0_c]);
        parents.push_back(component[2_c]);
    });

    EXPECT_EQ(normalIterate, normalIterate_expected);
    EXPECT_EQ(treeIterate, treeIterate_expected);
    EXPECT_EQ(parents, parents_expected);
}

TEST(TestComponent, TestComponentStorageTreeArray) {
    ComponentStorageTree<TestComponentTransform, ComponentStorageArray> storage;

    storage.add(1);
    storage.add(5);
    storage.addWithParent(2, 5);
    storage.add(3);
    storage.addWithParent(4, 5);

    std::vector<EntityID> normalIterate;
    std::vector<EntityID> normalIterate_expected = {1, 2, 3, 4, 5};

    std::vector<EntityID> treeIterate;
    std::vector<EntityID> treeIterate_expected = {1, 3, 5, 2, 4};

    std::vector<EntityID> parents;
    std::vector<EntityID> parents_expected = {EntityIDInvalid, EntityIDInvalid, EntityIDInvalid, 5, 5};

    storage.for_each([&normalIterate](auto component) {
        normalIterate.push_back(component[0_c]);
    });

    storage.tree_for_each([&treeIterate, &parents](auto component) {
        treeIterate.push_back(component[0_c]);
        parents.push_back(component[2_c]);
    });

    EXPECT_EQ(normalIterate, normalIterate_expected);
    EXPECT_EQ(treeIterate, treeIterate_expected);
    EXPECT_EQ(parents, parents_expected);
}

TEST(TestComponent, TestComponentConstruct) {
    TestWorldType world;

    TestComponentTransform transform;
    transform.idx = 4;
    TestComponentMeshFilter meshFilter;
    meshFilter.idx = 7;

    EXPECT_CALL(transform, testTransform()).Times(1);
    EXPECT_CALL(meshFilter, testMeshFilter()).Times(1);

    auto id = world.createEntity(transform, std::move(meshFilter));

    TestComponentTransform *transform2 = world.getComponentStorage<TestComponentTransform>().get(id);
    TestComponentMeshFilter *meshFilter2 = world.getComponentStorage<TestComponentMeshFilter>().get(id);

    EXPECT_EQ(transform2->idx, transform.idx);
    EXPECT_EQ(meshFilter2->idx, meshFilter.idx);
}

TEST(TestComponent, TestComponentConstructTree) {
    TestWorldType world;

    TestComponentTransform transform;
    transform.idx = 4;
    TestComponentMeshFilter2 meshFilter;
    meshFilter.idx = 7;

    EXPECT_CALL(transform, testTransform()).Times(1);
    EXPECT_CALL(meshFilter, testMeshFilter2()).Times(1);

    auto id = world.createEntity(transform, meshFilter);

    TestComponentTransform *transform2 = world.getComponentStorage<TestComponentTransform>().get(id);
    TestComponentMeshFilter2 *meshFilter2 = world.getComponentStorage<TestComponentMeshFilter2>().get(id);

    EXPECT_EQ(transform2->idx, transform.idx);
    EXPECT_EQ(meshFilter2->idx, meshFilter.idx);
}

TEST(TestComponent, TestComponentConstComponent) {
    TWorld<TestComponentTransform, const TestComponentTransform> world;

    auto id_non_const = world.createEntity<TestComponentTransform>();
    auto id_const = world.createEntity<const TestComponentTransform>();

    auto transform_non_const = world.getComponentStorage<TestComponentTransform>().get(id_non_const);
    auto transform_const = world.getComponentStorage<const TestComponentTransform>().get(id_const);

    auto can_alter = boost::hana::is_valid([](auto&& x) -> decltype((void)x->idx++){});

    static_assert(can_alter(transform_non_const));
    static_assert(!can_alter(transform_const));

    EXPECT_TRUE(transform_non_const != nullptr);
    EXPECT_TRUE(transform_const != nullptr);

}

TEST(TestComponent, TestComponentConstWorld) {
    TWorld<TestComponentTransform, const TestComponentTransform> world;
    auto const_world = (const decltype(world)*)&world;


    auto id_non_const = world.createEntity<TestComponentTransform>();
    auto id_const = world.createEntity<const TestComponentTransform>();

    auto transform_non_const = const_world->getComponentStorage<TestComponentTransform>().get(id_non_const);
    auto transform_const = const_world->getComponentStorage<const TestComponentTransform>().get(id_const);

    auto can_alter = boost::hana::is_valid([](auto&& x) -> decltype((void)x->idx++){});

    static_assert(!can_alter(transform_non_const));
    static_assert(!can_alter(transform_const));

    EXPECT_TRUE(transform_non_const != nullptr);
    EXPECT_TRUE(transform_const != nullptr);
}

TEST(TestComponent, TestComponentConstWorld2) {
    TWorld<TestComponentMeshFilter2, const TestComponentMeshFilter2> world;
    auto const_world = (const decltype(world)*)&world;


    auto id_non_const = world.createEntity<TestComponentMeshFilter2>();
    auto id_const = world.createEntity<const TestComponentMeshFilter2>();

    auto transform_non_const = const_world->getComponentStorage<TestComponentMeshFilter2>().get(id_non_const);
    auto transform_const = const_world->getComponentStorage<const TestComponentMeshFilter2>().get(id_const);

    auto can_alter = boost::hana::is_valid([](auto&& x) -> decltype((void)x->idx++){});

    static_assert(!can_alter(transform_non_const));
    static_assert(!can_alter(transform_const));

    EXPECT_TRUE(transform_non_const != nullptr);
    EXPECT_TRUE(transform_const != nullptr);
}

TEST(TestComponent, TestComponentConstWorld3) {
    TWorld<TestComponentMeshFilter2, const TestComponentMeshFilter2> world;
    auto const_world = (const decltype(world)*)&world;


    auto id_non_const = world.createEntity<TestComponentMeshFilter2>();
    auto id_const = world.createEntity<const TestComponentMeshFilter2>();

    auto transform_non_const = const_world->getComponentStorage<TestComponentMeshFilter2>().get(id_non_const);
    auto transform_const = const_world->getComponentStorage<const TestComponentMeshFilter2>().get(id_const);

    const_world->for_each<TestComponentMeshFilter2>([](auto a) {
        auto can_alter = boost::hana::is_valid([](auto&& x) -> decltype((void)x->idx++){});

        auto component = a[1_c];
        static_assert(!can_alter(component));
    });

    const_world->for_each<const TestComponentMeshFilter2>([](auto a) {
        auto can_alter = boost::hana::is_valid([](auto&& x) -> decltype((void)x->idx++){});

        auto component = a[1_c];
        static_assert(!can_alter(component));
    });

    EXPECT_TRUE(transform_non_const != nullptr);
    EXPECT_TRUE(transform_const != nullptr);
}