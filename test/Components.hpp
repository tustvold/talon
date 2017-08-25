#pragma once
#include <TalonConfig.hpp>
#include <ecs/ComponentStorage.hpp>
#include <ecs/ComponentStorageVector.hpp>
#include <ecs/ComponentStorageMap.hpp>
#include <ecs/ComponentStorageTree.hpp>
#include <ecs/ComponentStorageFlatMap.hpp>
#include <ecs/component/TreeComponentData.hpp>

USING_TALON_NS;

struct TestComponentArray {
    TreeComponentData treeComponentData;
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentArray";
    }
    mutable bool touched = false;
};

struct TestComponentMap {
    TreeComponentData treeComponentData;
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentMap";
    }
    mutable bool touched = false;
};

struct TestComponentFlatMap {
    TreeComponentData treeComponentData;
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentFlatMap";
    }
    mutable bool touched = false;
};

struct TestComponentArrayTree {
    TreeComponentData treeComponentData;
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentArrayTree";
    }
    mutable bool touched = false;
};

struct TestComponentMapTree {
    TreeComponentData treeComponentData;
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentMapTree";
    }
    mutable bool touched = false;
};

struct TestComponentFlatMapTree {
    TreeComponentData treeComponentData;
    int idx = 0;
    static constexpr const char* name() {
        return "TestComponentFlatMapTree";
    }
    mutable bool touched = false;
};

TALON_NS_BEGIN
COMPONENT_STORAGE_DEF(TestComponentArray, ComponentStorageVector);
COMPONENT_STORAGE_DEF(TestComponentMap, ComponentStorageMap);
COMPONENT_STORAGE_DEF(TestComponentFlatMap, ComponentStorageFlatMap);
COMPONENT_STORAGE_DEF_TREE(TestComponentArrayTree, ComponentStorageVector);
COMPONENT_STORAGE_DEF_TREE(TestComponentMapTree, ComponentStorageMap);
COMPONENT_STORAGE_DEF_TREE(TestComponentFlatMapTree, ComponentStorageFlatMap);
TALON_NS_END

template <typename Storage>
void generateTreeHierarchy(Storage& storage, int num_tests) {
    std::vector<EntityID> addedIDs;


    std::vector<EntityID> toAdd;
    for (int i = 0; i < num_tests; i++) {
        toAdd.push_back(i);
    }

    std::random_shuffle(toAdd.begin(), toAdd.end());

    storage.add(toAdd.back());
    addedIDs.push_back(toAdd.back());
    toAdd.pop_back();

    for (auto id : toAdd) {
        auto parent_idx = rand() % addedIDs.size();
        bool foo = static_cast<bool>(rand() % 8);
        if (!foo) {
            storage.addWithParent(id, addedIDs[parent_idx]);
        } else {
            storage.add(id);
        }
        addedIDs.push_back(id);
    }
}