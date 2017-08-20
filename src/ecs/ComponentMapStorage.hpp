#pragma once
#include "TalonConfig.hpp"
#include "Entity.hpp"
#include <map>

TALON_NS_BEGIN

template<typename Component>
struct ComponentMapStorage {
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

    ComponentMapStorage() = default;
    ComponentMapStorage(const ComponentMapStorage &) = delete;
    ComponentMapStorage(ComponentMapStorage &&other) noexcept = delete;
    ComponentMapStorage &operator=(const ComponentMapStorage &) = delete;
    ComponentMapStorage &operator=(ComponentMapStorage &&) = delete;

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

TALON_NS_END