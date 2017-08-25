#pragma once
#include "TalonConfig.hpp"
#include "ComponentStorage.hpp"
#include <vector>
#include <optional>
#include <util/Logging.hpp>
#include <boost/container/static_vector.hpp>

#include <boost/hana.hpp>
using namespace boost::hana::literals;

TALON_NS_BEGIN
template<typename Component>
struct ComponentStorageVector : public ComponentStorageBase {
    using internal_iterator = typename boost::container::static_vector<std::optional<Component>, MaxEntityID>::iterator;
    using internal_const_iterator = typename boost::container::static_vector<std::optional<Component>, MaxEntityID>::const_iterator;
public:
    template <typename InputIterator>
    struct Iterator {
    public:
        Iterator(InputIterator begin, InputIterator end) : begin(begin), current(begin), end(end) , size(end - begin) {
            if (!isValid())
                increment();
        }

        inline auto operator*() {
            return boost::hana::make_tuple(static_cast<EntityID>(current - begin), boost::hana::make_tuple(&current->value()));
        }

        inline bool operator==(const Iterator &rhs) { return current == rhs.current; }
        inline bool operator!=(const Iterator &rhs) { return current != rhs.current; }

        inline Iterator operator++() {
            Iterator i = *this;
            increment();
            return i;
        }

        inline void advanceToOrIncrement(EntityID id) {
            if (id >= size) {
                current = end;
                return;
            }
            current = begin + id;
            if (!isValid())
                increment();
        }

        inline Iterator operator++(int bar) {
            increment();
            return *this;
        }

        inline bool isValid() {
            return current->has_value();
        }

        inline EntityID getID() {
            return static_cast<EntityID >(current - begin);
        }

    private:
        InputIterator begin;
        InputIterator current;
        InputIterator end;
        size_t size;

        inline void increment() {
            while (current != end) {
                current++;
                if (isValid())
                    break;
            }
        }
    };

    using iterator = Iterator<internal_iterator>;
    using const_iterator = Iterator<internal_const_iterator>;

    ComponentStorageVector() : data(MaxEntityID), maxAddedEntityID(0) {

    }
    ComponentStorageVector(const ComponentStorageVector &) = delete;
    ComponentStorageVector(ComponentStorageVector &&other) noexcept = delete;
    ComponentStorageVector &operator=(const ComponentStorageVector &) = delete;
    ComponentStorageVector &operator=(ComponentStorageVector &&) = delete;

    void add(EntityID id) {
        TASSERT(id < MaxEntityID);
        TASSERT(!data[id].has_value());
        maxAddedEntityID = id > maxAddedEntityID ? id : maxAddedEntityID;
        data[id].emplace();
        incrementGeneration();
    }

    template <typename... Args>
    void add(EntityID id, Args&&... args) {
        TASSERT(id < MaxEntityID);
        TASSERT(!data[id].has_value());
        maxAddedEntityID = id > maxAddedEntityID ? id : maxAddedEntityID;
        data[id].emplace(std::forward<Args>(args)...);
        incrementGeneration();
    }

    inline Component* get(EntityID id) {
        return data[id].has_value() ? &data[id].value() : nullptr;
    }

    inline const Component* get(EntityID id) const {
        return data[id].has_value() ? &data[id].value() : nullptr;
    }

    inline iterator begin() {
        return iterator(data.begin(), data.begin() + maxAddedEntityID + 1);
    }

    inline iterator end() {
        return iterator(data.begin() + maxAddedEntityID + 1, data.begin() + maxAddedEntityID + 1);
    }

    inline const_iterator cbegin() const {
        return const_iterator(data.cbegin(), data.cbegin() + maxAddedEntityID + 1);
    }

    inline const_iterator cend() const {
        return const_iterator(data.cbegin() + maxAddedEntityID + 1, data.cbegin() + maxAddedEntityID + 1);
    }

    template<class UnaryFunction>
    void for_each(UnaryFunction f) {
        for (auto it = begin(); it != end(); ++it) {
            f(*it);
        }
    }

private:
    boost::container::static_vector<std::optional<Component>, MaxEntityID> data;
    EntityID maxAddedEntityID;
};

TALON_NS_END