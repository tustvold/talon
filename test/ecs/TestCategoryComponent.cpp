#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Components.hpp"

USING_TALON_NS;

TEST(TestCategoryComponent, TestCategoryStorage) {
    ComponentStorage<TestComponentFlatMapCategory> storage;

    const int num_tests = 1024;
    std::vector<EntityID> addedIDs;


    std::vector<EntityID> toAdd;
    for (int i = 0; i < num_tests; i++) {
        toAdd.push_back(i);
    }

    auto random = std::default_random_engine(1234);

    std::shuffle(toAdd.begin(), toAdd.end(), random);

    const CategoryID maxCategory = 5;

    std::uniform_int_distribution<CategoryID> uniform_dist(1, maxCategory);

    for (auto id : toAdd) {
        CategoryID category = uniform_dist(random);
        storage.add(id, category);
    }


    CategoryID currentID = 0;
    EntityID lastEntityID = 0;


    storage.category_for_each([&currentID, &lastEntityID](CategoryID id){
        EXPECT_LT(currentID, id);
        EXPECT_LT(id, 1 + maxCategory);
        currentID = id;
        lastEntityID = 0;
    },

    [&currentID, &lastEntityID](CategoryID categoryID, EntityID entityID, TestComponentFlatMapCategory* component){
        EXPECT_FALSE(component->touched);
        EXPECT_LE(lastEntityID, entityID);
        EXPECT_EQ(categoryID, currentID);
        component->touched = true;
        lastEntityID = entityID;
    });

}