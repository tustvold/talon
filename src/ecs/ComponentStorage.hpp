#pragma once
#include "TalonConfig.hpp"
#include <boost/hana.hpp>
using namespace boost::hana::literals;

TALON_NS_BEGIN

#define COMPONENT_STORAGE_DEF(Component, StorageType) \
    template <> struct ComponentStorage<Component> : StorageType<Component> {}

#define COMPONENT_STORAGE_DEF_TREE(Component, StorageType) \
    template <> struct ComponentStorage<Component> : ComponentStorageTree<Component, StorageType> {}


struct ComponentStorageBase {
public:
    // We initialize generation to 1, so that 0 corresponds to uninitialized for Views
    ComponentStorageBase() : generation(1) {}

    ComponentGenerationID getGeneration() const {
        return generation;
    }

protected:

    void incrementGeneration() {
        generation++;
    }

private:

    ComponentGenerationID generation;
};

template<typename Component>
struct ComponentStorage;

template<typename... SystemComponents>
struct ComponentStorageTuple {

    template<typename Component>
    auto &getComponentStorage() {
        constexpr auto
        index = boost::hana::index_if(boost::hana::tuple_t<ComponentStorage<SystemComponents>...>, [](auto t) {
            return t == boost::hana::type_c<ComponentStorage<Component>>;
        });
        static_assert(index != boost::hana::nothing, "Could not find requested component in StorageTuple");

        return boost::hana::at(data, *index);
    }

    template<typename Component>
    auto &getComponentStorage() const {
        constexpr auto
        index = boost::hana::index_if(boost::hana::tuple_t<ComponentStorage<SystemComponents>...>, [](auto t) {
            return t == boost::hana::type_c<ComponentStorage<Component>>;
        });
        static_assert(index != boost::hana::nothing, "Could not find requested component in StorageTuple");

        return boost::hana::at(data, *index);
    }

    boost::hana::tuple<ComponentStorage<SystemComponents>...> data;
};

TALON_NS_END