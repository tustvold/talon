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
    using internal_iterator = typename std::array<std::optional<Component>, MaxEntityID>::iterator;
    using internal_const_iterator = typename std::array<std::optional<Component>, MaxEntityID>::const_iterator;
public:
    template <typename InputIterator>
    struct Iterator {
    public:
        Iterator(InputIterator begin, InputIterator end) : begin(begin), current(begin), end(end) , size(end - begin) {
            if (!isValid())
                increment();
        }

        auto operator*() {
            return boost::hana::make_tuple(static_cast<EntityID>(current - begin), boost::hana::make_tuple(&current->value()));
        }

        bool operator==(const Iterator &rhs) { return current == rhs.current; }
        bool operator!=(const Iterator &rhs) { return current != rhs.current; }

        Iterator operator++() {
            Iterator i = *this;
            increment();
            return i;
        }

        void advanceToOrIncrement(EntityID id) {
            if (id >= size) {
                current = end;
                return;
            }
            current = begin + id;
            if (!isValid())
                increment();
        }

        Iterator operator++(int bar) {
            increment();
            return *this;
        }

        bool isValid() {
            return current->has_value();
        }

        EntityID getID() {
            return static_cast<EntityID >(current - begin);
        }

    private:
        InputIterator begin;
        InputIterator current;
        InputIterator end;
        size_t size;

        void increment() {
            while (current != end) {
                current++;
                if (isValid())
                    break;
            }
        }
    };

    using iterator = Iterator<internal_iterator>;
    using const_iterator = Iterator<internal_const_iterator>;

    ComponentStorageArray() : maxAddedEntityID(0) {

    }
    ComponentStorageArray(const ComponentStorageArray &) = delete;
    ComponentStorageArray(ComponentStorageArray &&other) noexcept = delete;
    ComponentStorageArray &operator=(const ComponentStorageArray &) = delete;
    ComponentStorageArray &operator=(ComponentStorageArray &&) = delete;

    void add(EntityID id) {
        TASSERT(id < MaxEntityID);
        maxAddedEntityID = id > maxAddedEntityID ? id : maxAddedEntityID;
        data[id].emplace();
        incrementGeneration();
    }

    template <typename... Args>
    void add(EntityID id, Args&&... args) {
        TASSERT(id < MaxEntityID);
        maxAddedEntityID = id > maxAddedEntityID ? id : maxAddedEntityID;
        data[id].emplace(std::forward<Args>(args)...);
        incrementGeneration();
    }

    Component* get(EntityID id) {
        return data[id].has_value() ? &data[id].value() : nullptr;
    }

    const Component* get(EntityID id) const {
        return data[id].has_value() ? &data[id].value() : nullptr;
    }

    iterator begin() {
        return iterator(data.begin(), data.begin() + maxAddedEntityID + 1);
    }

    iterator end() {
        return iterator(data.begin() + maxAddedEntityID + 1, data.begin() + maxAddedEntityID + 1);
    }

    const_iterator cbegin() const {
        return const_iterator(data.cbegin(), data.cbegin() + maxAddedEntityID + 1);
    }

    const_iterator cend() const {
        return const_iterator(data.cbegin() + maxAddedEntityID + 1, data.cbegin() + maxAddedEntityID + 1);
    }

    template<class UnaryFunction>
    void for_each(UnaryFunction f) {
        for (auto it = begin(); it != end(); ++it) {
            f(*it);
        }
    }

private:
    std::array<std::optional<Component>, MaxEntityID> data;
    EntityID maxAddedEntityID;
};

TALON_NS_END