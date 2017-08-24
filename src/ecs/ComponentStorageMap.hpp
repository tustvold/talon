#pragma once
#include "TalonConfig.hpp"
#include "TalonTypes.hpp"
#include "util/Logging.hpp"
#include "ComponentStorage.hpp"
#include <boost/hana.hpp>
#include <map>

using namespace boost::hana::literals;

TALON_NS_BEGIN

template<typename Component>
struct ComponentStorageMap : ComponentStorageBase {
    using internal_iterator = typename std::map<EntityID, Component>::iterator;
    using internal_const_iterator = typename std::map<EntityID, Component>::const_iterator;
public:
    template <typename InputIterator>
    struct Iterator {

    public:
        explicit Iterator(InputIterator begin) : current(begin) {}

        auto operator*() {
            return boost::hana::make_tuple(current->first, boost::hana::make_tuple(&current->second));
        }

        bool operator==(const Iterator &rhs) { return current == rhs.current; }
        bool operator!=(const Iterator &rhs) { return current != rhs.current; }

        void advanceToOrIncrement(EntityID id) {
            current++;
        }

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

        EntityID getID() {
            return current->first;
        }

    private:
        InputIterator current;

    };

    using iterator = Iterator<internal_iterator>;
    using const_iterator = Iterator<internal_const_iterator>;

    ComponentStorageMap() = default;
    ComponentStorageMap(const ComponentStorageMap &) = delete;
    ComponentStorageMap(ComponentStorageMap &&other) noexcept = delete;
    ComponentStorageMap &operator=(const ComponentStorageMap &) = delete;
    ComponentStorageMap &operator=(ComponentStorageMap &&) = delete;

    void add(EntityID id) {
        TASSERT(id < MaxEntityID);
        data[id];
        incrementGeneration();
    }

    template <typename Arg>
    void add(EntityID id, Arg&& arg) {
        TASSERT(id < MaxEntityID);
        data.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(std::forward<Arg>(arg)));
        incrementGeneration();
    }

    Component* get(EntityID id) {
        auto it = data.find(id);
        return it == data.end() ? nullptr : &it->second;
    }

    const Component* get(EntityID id) const {
        auto it = data.find(id);
        return it == data.end() ? nullptr : &it->second;
    }

    iterator begin() {
        return iterator(data.begin());
    }

    iterator end() {
        return iterator(data.end());
    }

    const_iterator cbegin() const {
        return const_iterator(data.cbegin());
    }

    const_iterator cend() const {
        return const_iterator(data.cend());
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