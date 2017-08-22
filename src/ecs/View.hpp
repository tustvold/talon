#pragma once
#include "TWorld.hpp"
#include <array>
#include <vector>

TALON_NS_BEGIN

template<typename TWorld, typename Predicate, typename... Components>
class View {
public:
    explicit View(TWorld &world, Predicate predicate) : predicate(predicate), generations({}) {
    }

    template<class UnaryFunction>
    void for_each(TWorld &world, UnaryFunction f) {
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
                if (!predicate(*it))
                    continue;
                f(*it);
                view.push_back(*it);
            }
            return;
        }

        auto beginLocal = view.begin();
        auto endLocal = view.end();
        for (auto it = beginLocal; it != endLocal; ++it) {
            f(*it);
        }
    }

private:
    Predicate predicate;
    std::array<ComponentGenerationID, sizeof...(Components)> generations;
    std::vector<boost::hana::tuple<EntityID, boost::hana::tuple<typename std::add_pointer<Components>::type...>>> view;
};

template<typename... Components, typename TWorld>
auto makeView(TWorld &world) {
    auto always_true = [](auto a) { return true; };
    return View<TWorld, decltype(always_true), Components...>(world, always_true);
}

template<typename... Components, typename TWorld, typename Predicate>
auto makeView(TWorld &world, Predicate predicate) {
    return View<TWorld, Predicate, Components...>(world, predicate);
}

TALON_NS_END