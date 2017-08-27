#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <TalonTypes.hpp>
#include <ecs/ComponentStorageVector.hpp>
#include <ecs/ComponentStorageMap.hpp>
#include <ecs/ComponentStorageTree.hpp>
#include <ecs/View.hpp>
#include "ecs/TWorld.hpp"
#include "Components.hpp"

USING_TALON_NS;

TEST(TestTreeComponent, TestComponentStorageTreeArray) {
    ComponentStorageTree<TestComponentArrayTree, ComponentStorageVector> storage;

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

TEST(TestTreeComponent, TestComponentStorageTreeArrayRandom) {
    ComponentStorageTree<const TestComponentArrayTree, ComponentStorageVector> storage;

    const int num_tests = 1024;

    generateTreeHierarchy(storage, num_tests);

    storage.tree_for_each(boost::hana::fuse([](EntityID id,
                                               const TestComponentArrayTree *component,
                                               EntityID parent_id,
                                               const TestComponentArrayTree *parent) {

        EXPECT_EQ(parent == nullptr, parent_id == EntityIDInvalid);
        if (parent != nullptr) {
            EXPECT_TRUE(parent->touched);
        }
        EXPECT_FALSE(component->touched);
        component->touched = true;
    }));

    for (int i = 0; i < num_tests; i++) {
        auto component = storage.get(i);
        EXPECT_TRUE(component->touched);
    }
}

TEST(TestTreeComponent, TestComponentStorageTreeArrayRandomWorld) {
    TWorld<TestComponentArrayTree> world;

    const int num_tests = 1024;

    generateTreeHierarchy(world.getComponentStorage<TestComponentArrayTree>(), num_tests);

    world.tree_for_each<TestComponentArrayTree>(boost::hana::fuse([](EntityID id,
                                               const TestComponentArrayTree *component,
                                               EntityID parent_id,
                                               const TestComponentArrayTree *parent) {

        EXPECT_EQ(parent == nullptr, parent_id == EntityIDInvalid);
        if (parent != nullptr) {
            EXPECT_TRUE(parent->touched);
        }
        EXPECT_FALSE(component->touched);
        component->touched = true;
    }));


    auto& storage = world.getComponentStorage<TestComponentArrayTree>();
    for (int i = 0; i < num_tests; i++) {
        auto component = storage.get(i);
        EXPECT_TRUE(component->touched);
    }
}