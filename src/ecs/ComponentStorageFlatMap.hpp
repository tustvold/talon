#pragma once
#include "TalonConfig.hpp"
#include "TalonTypes.hpp"
#include "util/Logging.hpp"
#include "ComponentStorage.hpp"
#include <boost/hana.hpp>
#include <boost/container/flat_map.hpp>

using namespace boost::hana::literals;

TALON_NS_BEGIN

template<typename Component>
struct ComponentStorageFlatMap : public ComponentStorageBase {
    using internal_iterator = typename boost::container::flat_map<EntityID, Component>::iterator;
    using internal_const_iterator = typename boost::container::flat_map<EntityID, Component>::const_iterator;
public:
    template <typename InputIterator>
    struct Iterator {

    public:
        explicit Iterator(InputIterator begin) : current(begin) {}

        inline auto operator*() {
            return boost::hana::make_tuple(current->first, boost::hana::make_tuple(&current->second));
        }

        inline bool operator==(const Iterator &rhs) { return current == rhs.current; }
        inline bool operator!=(const Iterator &rhs) { return current != rhs.current; }

        inline void advanceToOrIncrement(EntityID id) {
            current++;
        }

        inline Iterator operator++() {
            Iterator i = *this;
            current++;
            return i;
        }

        inline Iterator operator++(int foo) {
            current++;
            return *this;
        }

        inline bool isValid() {
            return true;
        }

        inline EntityID getID() {
            return current->first;
        }

    private:
        InputIterator current;

    };

    using iterator = Iterator<internal_iterator>;
    using const_iterator = Iterator<internal_const_iterator>;

    ComponentStorageFlatMap() = default;
    ComponentStorageFlatMap(const ComponentStorageFlatMap &) = delete;
    ComponentStorageFlatMap(ComponentStorageFlatMap &&other) noexcept = delete;
    ComponentStorageFlatMap &operator=(const ComponentStorageFlatMap &) = delete;
    ComponentStorageFlatMap &operator=(ComponentStorageFlatMap &&) = delete;

    void add(EntityID id) {
        TASSERT(id < MaxEntityID);
        data.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::make_tuple());
        incrementGeneration();
    }

    template <typename Arg>
    void add(EntityID id, Arg&& arg) {
        TASSERT(id < MaxEntityID);
        data.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(std::forward<Arg>(arg)));
        incrementGeneration();
    }

    inline Component* get(EntityID id) {
        auto it = data.find(id);
        return it == data.end() ? nullptr : &it->second;
    }

    inline const Component* get(EntityID id) const {
        auto it = data.find(id);
        return it == data.end() ? nullptr : &it->second;
    }

    inline iterator begin() {
        return iterator(data.begin());
    }

    inline iterator end() {
        return iterator(data.end());
    }

    inline const_iterator cbegin() const {
        return const_iterator(data.cbegin());
    }

    inline const_iterator cend() const {
        return const_iterator(data.cend());
    }

    template<class UnaryFunction>
    void for_each(UnaryFunction f) {
        for (auto it = begin(); it != end(); ++it) {
            f(*it);
        }
    }

private:
    boost::container::flat_map<EntityID, Component> data;
};

TALON_NS_END