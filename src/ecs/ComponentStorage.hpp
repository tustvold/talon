#pragma once
#include "TalonConfig.hpp"
#include "Entity.hpp"

TALON_NS_BEGIN

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
        static_assert(index != boost::hana::nothing, "Failed to find component");

        return boost::hana::at(data, *index);
    }

    template<typename Component>
    auto &getComponentStorage() const {
        constexpr auto
        index = boost::hana::index_if(boost::hana::tuple_t<ComponentStorage<SystemComponents>...>, [](auto t) {
            return t == boost::hana::type_c<ComponentStorage<Component>>;
        });
        static_assert(index != boost::hana::nothing, "Failed to find component");

        return boost::hana::at(data, *index);
    }

    boost::hana::tuple<ComponentStorage<SystemComponents>...> data;
};

TALON_NS_END