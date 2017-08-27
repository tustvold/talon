#pragma once
#include "AnnotationCategory.hpp"
#include "AnnotationTree.hpp"
#include "AnnotationDependency.hpp"

// When combined with ComponentStorageCategory allows iteration through components by category
#define CATEGORY_ANNOTATION categoryComponentDatas
#define ADD_CATEGORY_ANNOTATION() AnnotationCategory CATEGORY_ANNOTATION

// When combined with ComponentStorageTree allows iteration through components by tree hierarchy
#define TREE_ANNOTATION treeComponentDatas
#define ADD_TREE_ANNOTATION() AnnotationTree TREE_ANNOTATION

#define ADD_DEPENDENCY_ANNOTATION(...) static constexpr auto getDependencies() { \
    return boost::hana::tuple_t< __VA_ARGS__ >; \
}

