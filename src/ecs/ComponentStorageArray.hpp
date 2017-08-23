#pragma once
#include "TalonConfig.hpp"
#include "ComponentStorage.hpp"
#include <array>
#include <optional>
#include <util/Logging.hpp>

#include <boost/hana.hpp>
using namespace boost::hana::literals;

TALON_NS_BEGIN
template<typename Component>
struct ComponentStorageArray : ComponentStorageBase {
public:
    struct Iterator {
    public:
        using InputIterator = typename std::array<std::optional<Component>, MaxEntityID>::iterator;

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

    ComponentStorageArray() = default;
    ComponentStorageArray(const ComponentStorageArray &) = delete;
    ComponentStorageArray(ComponentStorageArray &&other) noexcept = delete;
    ComponentStorageArray &operator=(const ComponentStorageArray &) = delete;
    ComponentStorageArray &operator=(ComponentStorageArray &&) = delete;

    void add(EntityID id) {
        TASSERT(id < MaxEntityID);
        data[id].emplace();
        incrementGeneration();
    }

    template <typename... Args>
    void add(EntityID id, Args&&... args) {
        TASSERT(id < MaxEntityID);
        data[id].emplace(std::forward<Args>(args)...);
        incrementGeneration();
    }

    Component* get(EntityID id) {
        return data[id].has_value() ? &data[id].value() : nullptr;
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