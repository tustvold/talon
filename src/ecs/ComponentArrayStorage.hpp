#pragma once
#include "talon.hpp"
#include "ecs.hpp"
#include <array>
#include <optional>

TALON_NS_BEGIN
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

TALON_NS_END