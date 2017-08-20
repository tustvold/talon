#pragma once
#include "talon.hpp"
#include "ecs.hpp"
#include "ComponentStorage.hpp"
#include "IdentifierPool.hpp"

TALON_NS_BEGIN

namespace detail {
template<typename Iter1, typename Iter2>
struct ForEachUtilIter {
public:
    ForEachUtilIter(Iter1 begin1, Iter1 end1, Iter2 begin2, Iter2 end2)
        : current1(begin1), end1(end1), current2(begin2), end2(end2) {
        if (!isValid())
            increment();
    }

    auto operator*() {
        auto val1 = *current1;
        auto val2 = *current2;

        auto components = boost::hana::concat(val1[1_c], val2[1_c]);
        auto ret = boost::hana::make_tuple(val1[0_c], components);
        return ret;
    }

    bool operator==(const ForEachUtilIter &rhs) { return current1 == rhs.current1 && current2 == rhs.current2; }
    bool operator!=(const ForEachUtilIter &rhs) { return current1 != rhs.current1 || current2 != rhs.current2; }

    ForEachUtilIter operator++() {
        ForEachUtilIter i = *this;
        increment();
        return i;
    }

    ForEachUtilIter operator++(int bar) {
        increment();
        return *this;
    }
private:
    Iter1 current1;
    Iter1 end1;
    Iter2 current2;
    Iter2 end2;

    inline bool isValid() {
        return current1.isValid() && current2.isValid() && (*current1)[0_c] == (*current2)[0_c];
    }

    void increment() {
        while (current1.isValid() && current2.isValid() && current1 != end1 && current2 != end2) {
            if ((*current1)[0_c] < (*current2)[0_c]) {
                current1++;
            } else {
                current2++;
            }
            if (isValid())
                return;
        }
        while (current1 != end1) {
            current1++;
            if (isValid())
                return;
        }
        while (current2 != end2) {
            current2++;
            if (isValid())
                return;
        }
    }
};

template<typename... Components>
struct ForEachUtil;

template<class TComponent>
struct ForEachUtil<TComponent> {
    template<class StorageTuple>
    static auto begin(StorageTuple &st) {
        auto &componentStorage = st.template getComponentStorage<TComponent>();
        return componentStorage.begin();
    }

    template<class StorageTuple>
    static auto end(StorageTuple &st) {
        auto &componentStorage = st.template getComponentStorage<TComponent>();
        return componentStorage.end();
    }
};

template<typename Component, typename... Others>
struct ForEachUtil<Component, Others...> {
    template<class StorageTuple>
    static auto begin(StorageTuple &st) {
        auto myBegin = ForEachUtil<Component>::begin(st);
        auto myEnd = ForEachUtil<Component>::end(st);

        auto otherBegin = ForEachUtil<Others...>::begin(st);
        auto otherEnd = ForEachUtil<Others...>::end(st);
        return ForEachUtilIter(myBegin, myEnd, otherBegin, otherEnd);
    }

    template<class StorageTuple>
    static auto end(StorageTuple &st) {
        auto myEnd = ForEachUtil<Component>::end(st);
        auto otherEnd = ForEachUtil<Others...>::end(st);
        return ForEachUtilIter(myEnd, myEnd, otherEnd, otherEnd);
    }
};
}

template<typename... SystemComponents>
class World {
public:
    World() = default;
    World(const World &) = delete;
    World(World &&) = delete;
    World &operator=(const World &) = delete;
    World &operator=(World &&) = delete;

    template<typename... EntityComponents>
    EntityID createEntity() {
        auto id = pool.get();

        auto types = boost::hana::tuple_t<EntityComponents...>;
        boost::hana::for_each(types, [this, &id](auto t) {
            using unwrapped = typename decltype(t)::type;
            getComponentStorage<unwrapped>().add(id);
        });

        return id;
    }

    template<typename... Components, class UnaryFunction>
    void for_each(UnaryFunction f) {
        auto begin = detail::ForEachUtil<Components...>::begin(storage);
        auto end = detail::ForEachUtil<Components...>::end(storage);

        for (auto it = begin; it != end; ++it) {
            f(*it);
        }
    }

    template<typename Component>
    auto &getComponentStorage() {
        return storage.getComponentStorage<Component>();
    }

    template<typename Component>
    auto &getComponentStorage() const {
        return storage.getComponentStorage<Component>();
    }

private:
    IdentifierPool pool;
    ComponentStorageTuple<SystemComponents...> storage;
};

TALON_NS_END