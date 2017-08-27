#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/timer/timer.hpp>
#include <TalonConfig.hpp>
#include <ecs/TWorld.hpp>
#include "Components.hpp"

template<typename Component>
void treeForEachTest() {
    TWorld<Component> world;

    static const int num_tests = 800;
    generateTreeHierarchy(world.template getComponentStorage<Component>(), num_tests);

    std::cout << std::endl << std::endl;
    std::cout << "Tree For Each Test - " << Component::name() << std::endl;

    boost::timer::auto_cpu_timer timer;

    for (int i = 0; i < 1000; i++) {
        world.template tree_for_each<Component>(boost::hana::fuse([](EntityID id,
                                                                     Component *component,
                                                                     EntityID parent_id,
                                                                     Component *parent) {
            EXPECT_EQ(parent == nullptr, parent_id == EntityIDInvalid);
            if (parent != nullptr) {
                EXPECT_TRUE(parent->touched);
            }
            component->touched = true;
        }));
    }

}

TEST(BenchTreeComponent, RunTreeBenches) {
    treeForEachTest<TestComponentArrayTree>();
    treeForEachTest<TestComponentMapTree>();
    treeForEachTest<TestComponentFlatMapTree>();
}