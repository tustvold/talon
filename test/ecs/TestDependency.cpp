#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ecs/annotations/AnnotationDependency.hpp>
#include <ecs/TWorld.hpp>
#include "Components.hpp"

USING_TALON_NS;

struct TestComponentDependencies {

    static constexpr auto getDependencies() {
        return boost::hana::tuple_t<TestComponentFlatMapCategory>;
    }

    int fooMember;
};

struct TestComponentDependencies2 {

    static constexpr auto getDependencies() {
        return boost::hana::tuple_t<TestComponentFlatMap>;
    }

    int fooMember;
};

TALON_NS_BEGIN

COMPONENT_STORAGE_DEF(TestComponentDependencies2, ComponentStorageFlatMap);

TALON_NS_END

ASSERT_HAS_MEMBER(fooMember, TestComponentDependencies);

using Checker1 = DependencyChecker<TestComponentFlatMapCategory, TestComponentDependencies>;

using Checker2 = DependencyChecker<TestComponentDependencies>;

using Checker3 = DependencyChecker<TestComponentDependencies2, TestComponentDependencies, TestComponentFlatMap, TestComponentFlatMapCategory>;

using Checker4 = DependencyChecker<TestComponentDependencies2, TestComponentDependencies, TestComponentFlatMapCategory>;

using Checker5 = DependencyChecker<TestComponentDependencies2, TestComponentDependencies, TestComponentFlatMap>;

static_assert(Checker1::hasType<TestComponentFlatMapCategory>(), "Dependency Checker not working");
static_assert(!DependencyChecker<TestComponentArrayCategory>::hasType<TestComponentFlatMapCategory>(), "Dependency Checker not working");

BOOST_HANA_CONSTANT_CHECK(Checker1::getComponentDependencies<TestComponentDependencies>() == boost::hana::tuple_t<TestComponentFlatMapCategory>);
BOOST_HANA_CONSTANT_CHECK(Checker1::getComponentDependencies<TestComponentFlatMapCategory>() == boost::hana::make_tuple());

static_assert(Checker1::componentDependenciesSatisfiedIndividual<TestComponentDependencies>());
static_assert(!Checker2::componentDependenciesSatisfiedIndividual<TestComponentDependencies>());

static_assert(Checker1::componentDependenciesSatisfied());
static_assert(!Checker2::componentDependenciesSatisfied());

static_assert(Checker3::componentDependenciesSatisfied());
static_assert(!Checker4::componentDependenciesSatisfied());
static_assert(!Checker5::componentDependenciesSatisfied());
