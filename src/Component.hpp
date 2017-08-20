#pragma once
#include "talon.hpp"
#include <bitset>
#include <stack>
#include <map>
#include <boost/hana.hpp>

using namespace boost::hana::literals;

TALON_NS_BEGIN

using EntityID = size_t;
static const EntityID MaxEntityID = 1024;

class IdentifierPool {
public:
    IdentifierPool() : next(0) {

    }

    EntityID get() {
        if (!reserve.empty()) {
            auto ret = reserve.top();
            reserve.pop();
            return ret;
        }
        TASSERT(next < MaxEntityID);
        return next++;
    }

private:
    std::stack<EntityID> reserve;
    EntityID next;
};

template<typename Component>
struct ComponentArrayStorage {
public:
    struct Iterator {
        using InputIterator = typename std::array<std::optional<Component>, MaxEntityID>::iterator;
    public:
        Iterator(InputIterator begin, InputIterator end) : begin(begin), current(begin), end(end) {
            if (!isValid())
                increment();
        }

        boost::hana::tuple<EntityID, boost::hana::tuple<Component *>> operator*() {
            boost::hana::tuple<EntityID, boost::hana::tuple<Component *>>
                ret(static_cast<EntityID>(current - begin), boost::hana::make_tuple(&current->value()));
            return ret;
        }

        bool operator==(const Iterator &rhs) { return current == rhs.current; }
        bool operator!=(const Iterator &rhs) { return current != rhs.current; }

        Iterator operator++() {
            Iterator i = *this;
            increment();
            return i;
        }

        Iterator operator++(int bar) {
            increment();
            return *this;
        }

        bool isValid() {
            return current->has_value();
        }

    private:
        const InputIterator begin;
        InputIterator current;
        const InputIterator end;

        void increment() {
            while (current != end) {
                current++;
                if (isValid())
                    break;
            }
        }
    };

    ComponentArrayStorage() = default;
    ComponentArrayStorage(const ComponentArrayStorage &) = delete;
    ComponentArrayStorage(ComponentArrayStorage &&other) noexcept = delete;
    ComponentArrayStorage &operator=(const ComponentArrayStorage &) = delete;
    ComponentArrayStorage &operator=(ComponentArrayStorage &&) = delete;

    void add(EntityID id) {
        TASSERT(id < MaxEntityID);
        data[id].emplace();
    }

    auto& get(EntityID id) {
        return data[id].value();
    }

    Iterator begin() {
        return Iterator(data.begin(), data.end());
    }

    Iterator end() {
        return Iterator(data.end(), data.end());
    }

    template<class UnaryFunction>
    void for_each(UnaryFunction f) {
        for (auto it = begin(); it != end(); ++it) {
            f(*it);
        }
    }

private:
    std::array<std::optional<Component>, MaxEntityID> data;
};

template<typename Component>
struct ComponentDictStorage {
public:
    struct Iterator {
        using InputIterator = typename std::map<EntityID, Component>::iterator;
    public:
        explicit Iterator(InputIterator begin) : current(begin) {}

        boost::hana::tuple<EntityID, boost::hana::tuple<Component *>> operator*() {
            boost::hana::tuple<EntityID, boost::hana::tuple<Component *>>
                ret(current->first, boost::hana::make_tuple(&current->second));
            return ret;
        }

        bool operator==(const Iterator &rhs) { return current == rhs.current; }
        bool operator!=(const Iterator &rhs) { return current != rhs.current; }

        Iterator operator++() {
            Iterator i = *this;
            current++;
            return i;
        }

        Iterator operator++(int foo) {
            current++;
            return *this;
        }

        bool isValid() {
            return true;
        }

    private:
        InputIterator current;

    };

    ComponentDictStorage() = default;
    ComponentDictStorage(const ComponentDictStorage &) = delete;
    ComponentDictStorage(ComponentDictStorage &&other) noexcept = delete;
    ComponentDictStorage &operator=(const ComponentDictStorage &) = delete;
    ComponentDictStorage &operator=(ComponentDictStorage &&) = delete;

    void add(EntityID id) {
        TASSERT(id < MaxEntityID);
        data[id];
    }

    auto& get(EntityID id) {
        return data[id];
    }

    Iterator begin() {
        return Iterator(data.begin());
    }

    Iterator end() {
        return Iterator(data.end());
    }

    template<class UnaryFunction>
    void for_each(UnaryFunction f) {
        for (auto it = begin(); it != end(); ++it) {
            f(*it);
        }
    }

private:
    std::map<EntityID, Component> data;
};

template<typename Component>
struct ComponentStorage;

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
        auto begin = ForEachUtil<Components...>::begin(storage);
        auto end = ForEachUtil<Components...>::end(storage);

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