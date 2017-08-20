#pragma once
#include "TWorld.hpp"
#include <array>
#include <vector>

TALON_NS_BEGIN

template<typename TWorld, typename... Components>
class View {
public:
    explicit View(TWorld& world) : generations({}) {
    }

    template<class UnaryFunction>
    void for_each(TWorld& world, UnaryFunction f) {
        int i = 0;
        bool dirty = false;
        boost::hana::for_each(boost::hana::tuple_t<Components...>, [&](auto t) {
            using unwrapped = typename decltype(t)::type;
            auto other = world.template getComponentStorage<unwrapped>().getGeneration();
            dirty |= other != generations[i];
            generations[i] = other;
            i++;
        });

        if (dirty) {
            view.clear();

            auto beginLocal = world.template begin<Components...>();
            auto endLocal = world.template end<Components...>();
            for (auto it = beginLocal; it != endLocal; ++it) {
                f(*it);
                view.push_back(*it);
            }
            return;
        }

        auto beginLocal = world.template begin<Components...>();
        auto endLocal = world.template end<Components...>();
        for (auto it = beginLocal; it != endLocal; ++it) {
            f(*it);
        }
    }

private:
    std::array<GenerationID, sizeof...(Components)> generations;
    std::vector<boost::hana::tuple<EntityID, boost::hana::tuple<typename std::add_pointer<Components>::type...>>> view;
};

template <typename... Components, typename TWorld>
auto makeView(TWorld& world) {
    return View<TWorld, Components...>(world);
}

TALON_NS_END