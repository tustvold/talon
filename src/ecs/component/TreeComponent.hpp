#pragma once
#include <TalonTypes.hpp>

TALON_NS_BEGIN

class TreeComponent {
public:
    template<typename, template<typename> class>
    friend class ComponentStorageTree;

    TreeComponent() : first_child(EntityIDInvalid), next_sibling(EntityIDInvalid) {

    }

private:
    // ComponentStorageTree guarantees to increment the storage generation whenever these values are touched
    mutable EntityID first_child;
    mutable EntityID next_sibling;
};

TALON_NS_END