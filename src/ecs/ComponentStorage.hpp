#pragma once
#include <TalonConfig.hpp>
#include <TalonTypes.hpp>
#include <boost/hana.hpp>
using namespace boost::hana::literals;

TALON_NS_BEGIN

#define COMPONENT_STORAGE_DEF(Component, StorageType) \
    template <> struct ComponentStorage<Component> : StorageType<Component> {}; \
    template <> struct ComponentStorage<const Component> : StorageType<const Component> {}

#define COMPONENT_STORAGE_DEF_TREE(Component, StorageType) \
    template <> struct ComponentStorage<Component> : ComponentStorageTree<Component, StorageType> {}; \
    template <> struct ComponentStorage<const Component> : ComponentStorageTree<const Component, StorageType> {}

#define COMPONENT_STORAGE_DEF_CATEGORY(Component, StorageType) \
    template <> struct ComponentStorage<Component> : ComponentStorageCategory<Component, StorageType> {}; \
    template <> struct ComponentStorage<const Component> : ComponentStorageCategory<const Component, StorageType> {}

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
struct ComponentStorage {
    static_assert(sizeof(Component) == 0, "Must provide specialization for component");
};

template<typename... SystemComponents>
struct ComponentStorageTuple {

    template<typename Component>
    ComponentStorage<Component> &getComponentStorage() {
        constexpr auto
        index = boost::hana::index_if(boost::hana::tuple_t<ComponentStorage<SystemComponents>...>, [](auto t) {
            return t == boost::hana::type_c<ComponentStorage<Component>>;
        });
        static_assert(index != boost::hana::nothing, "Could not find requested component in StorageTuple");

        return boost::hana::at(data, *index);
    }

    template<typename Component>
    const ComponentStorage<Component> &getComponentStorage() const {
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