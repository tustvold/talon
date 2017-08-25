#pragma once
#include <TalonTypes.hpp>

TALON_NS_BEGIN

class CategoryComponentData {
public:
    template<typename, template<typename> class>
    friend class ComponentStorageCategory;

    CategoryComponentData(CategoryID category) : next_sibling(EntityIDInvalid), category(category) {

    }

private:
    // ComponentStorageTree guarantees to increment the storage generation whenever these values are touched
    mutable EntityID next_sibling;
    CategoryID category;
};

TALON_NS_END