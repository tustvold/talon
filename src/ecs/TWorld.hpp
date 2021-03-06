#pragma once
#include "TalonConfig.hpp"
#include "ComponentStorage.hpp"
#include "IdentifierPool.hpp"
#include <functional>
#include <ecs/annotations/AnnotationDependency.hpp>

TALON_NS_BEGIN

template<typename... SystemComponents>
class TWorld;

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

    bool operator==(const ForEachUtilIter &rhs) { return current1 == rhs.current1 || current2 == rhs.current2; }
    bool operator!=(const ForEachUtilIter &rhs) { return current1 != rhs.current1 && current2 != rhs.current2; }

    ForEachUtilIter operator++() {
        ForEachUtilIter i = *this;
        increment();
        return i;
    }

    ForEachUtilIter operator++(int bar) {
        increment();
        return *this;
    }

    void advanceToOrIncrement(EntityID id) {
        current1.advanceToOrIncrement(id);
        auto current1ID = current1.getID();
        if (current1 == end1)
            return;
        current2.advanceToOrIncrement(current1ID < id ? id : current1ID);
    }

    bool isValid() {
        return current1.isValid() && current2.isValid() && (*current1)[0_c] == (*current2)[0_c];
    }

    EntityID getID() {
        auto id1 = current1.getID();
        auto id2 = current2.getID();
        return id1 > id2 ? id1 : id2;
    }

private:
    Iter1 current1;
    Iter1 end1;
    Iter2 current2;
    Iter2 end2;

    void increment() {
        if (current1 == end1 || current2 == end2) {
            return;
        }

        // When one of the iterators has reached the end
        // It can never be valid so the iteration can be terminated
        while (true) {
            EntityID current1_id = current1.getID();
            EntityID current2_id = current2.getID();

            if (current1_id == current2_id) {
                current1++;
                if (current1 == end1)
                    return;
                current2.advanceToOrIncrement(current1.getID());
            } else if (current1_id < current2_id) {
                current1.advanceToOrIncrement(current2_id);
            } else {
                current2.advanceToOrIncrement(current1_id);
            }
            if (current1 == end1 || current2 == end2)
                break;
            else if ((*current1)[0_c] == (*current2)[0_c])
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

    template<class StorageTuple>
    static auto cbegin(StorageTuple &st) {
        auto &componentStorage = st.template getComponentStorage<TComponent>();
        return componentStorage.cbegin();
    }

    template<class StorageTuple>
    static auto cend(StorageTuple &st) {
        auto &componentStorage = st.template getComponentStorage<TComponent>();
        return componentStorage.cend();
    }
};

template<typename... Components>
static constexpr auto GetTupleType() {
    auto tuple = boost::hana::tuple_t<Components...>;
    auto id = boost::hana::tuple_t<EntityID>;
    auto transformed =
        boost::hana::transform(tuple, [](auto a) { return boost::hana::metafunction<std::add_pointer>(a); });
    auto concat = boost::hana::concat(id, transformed);

    using type = typename decltype(boost::hana::unpack(concat, boost::hana::template_<boost::hana::tuple>))::type;
    return boost::hana::type_c<type>;
}

template<typename... Components>
using TupleType = typename decltype(GetTupleType<Components...>())::type;

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

    template<class StorageTuple>
    static auto cbegin(StorageTuple &st) {
        auto myBegin = ForEachUtil<Component>::cbegin(st);
        auto myEnd = ForEachUtil<Component>::cend(st);

        auto otherBegin = ForEachUtil<Others...>::cbegin(st);
        auto otherEnd = ForEachUtil<Others...>::cend(st);
        return ForEachUtilIter(myBegin, myEnd, otherBegin, otherEnd);
    }

    template<class StorageTuple>
    static auto cend(StorageTuple &st) {
        auto myEnd = ForEachUtil<Component>::cend(st);
        auto otherEnd = ForEachUtil<Others...>::cend(st);
        return ForEachUtilIter(myEnd, myEnd, otherEnd, otherEnd);
    }
};

template<typename World, typename... Components>
struct CreateEntityUtil;

template<typename World, typename Component>
struct CreateEntityUtil<World, Component> {
    static void addEntity(World &world, EntityID id, Component &&component) {
        world.template getComponentStorage<typename std::remove_reference<Component>::type>().
            template add(id, std::forward<Component>(component));
    }
};

template<typename World, typename Component, typename... Components>
struct CreateEntityUtil<World, Component, Components...> {
    static void addEntity(World &world, EntityID id, Component &&component, Components &&... components) {
        world.template getComponentStorage<typename std::remove_reference<Component>::type>().
            template add<Component>(id, std::forward<Component>(component));
        CreateEntityUtil<World, Components...>::addEntity(world, id, std::forward<Components>(components)...);
    }
};

}

template<typename... SystemComponents>
class TWorld {
    using self_type = TWorld<SystemComponents...>;
    static_assert(DependencyChecker<SystemComponents...>::componentDependenciesSatisfied(), "Unsatisfied component dependencies");

public:
    static constexpr auto SystemComponentsTuple = boost::hana::tuple_t<SystemComponents...>;

    template <typename T>
    static constexpr bool HasComponent() {
        return boost::hana::contains(SystemComponentsTuple, boost::hana::type_c<T>);
    }

    TWorld() = default;
    TWorld(const TWorld &) = delete;
    TWorld(TWorld &&) = delete;
    TWorld &operator=(const TWorld &) = delete;
    TWorld &operator=(TWorld &&) = delete;

    template<typename... EntityComponents>
    EntityID createEntity() {
        static_assert(DependencyChecker<EntityComponents...>::componentDependenciesSatisfied(), "Unsatisfied component dependencies");

        auto id = pool.get();

        auto types = boost::hana::tuple_t<EntityComponents...>;
        boost::hana::for_each(types, [this, &id](auto t) {
            using unwrapped = typename decltype(t)::type;
            getComponentStorage<unwrapped>().add(id);
        });

        return id;
    }

    template<typename... EntityComponents>
    EntityID createEntity(EntityComponents &&... args) {
        static_assert(DependencyChecker<EntityComponents...>::componentDependenciesSatisfied(), "Unsatisfied component dependencies");

        auto id = pool.get();

        detail::CreateEntityUtil<self_type, EntityComponents...>::addEntity(*this,
                                                                            id,
                                                                            std::forward<EntityComponents>(args)...);

        return id;
    }

    template<typename... Components>
    auto begin() {
        return detail::ForEachUtil<Components...>::begin(storage);
    }

    template<typename... Components>
    auto end() {
        return detail::ForEachUtil<Components...>::end(storage);
    }

    template<typename... Components>
    auto cbegin() const {
        return detail::ForEachUtil<Components...>::cbegin(storage);
    }

    template<typename... Components>
    auto cend() const {
        return detail::ForEachUtil<Components...>::cend(storage);
    }

    template<typename... Components, class UnaryFunction>
    void for_each(UnaryFunction f) {
        static_assert(sizeof...(Components) > 0, "Must specify Components");
        auto beginLocal = begin<Components...>();
        auto endLocal = end<Components...>();
        for (auto it = beginLocal; it != endLocal; ++it) {
            f(*it);
        }
    }

    template<typename... Components, class UnaryFunction>
    void for_each(UnaryFunction f) const {
        static_assert(sizeof...(Components) > 0, "Must specify Components");
        auto beginLocal = cbegin<Components...>();
        auto endLocal = cend<Components...>();
        for (auto it = beginLocal; it != endLocal; ++it) {
            f(*it);
        }
    }

    template<typename Component>
    auto &getComponentStorage() {
        return storage.template getComponentStorage<Component>();
    }

    template<typename Component>
    auto &getComponentStorage() const {
        return storage.template getComponentStorage<Component>();
    }

    template<typename Component, class UnaryFunction>
    void tree_for_each(UnaryFunction f) {
        storage.template getComponentStorage<Component>().tree_for_each(f);
    }

private:
    IdentifierPool<EntityID, MaxEntityID> pool;
    ComponentStorageTuple<SystemComponents...> storage;
};

TALON_NS_END