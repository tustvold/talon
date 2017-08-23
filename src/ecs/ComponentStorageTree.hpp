#pragma once
#include "TalonConfig.hpp"
#include "ComponentStorage.hpp"
#include <vector>

TALON_NS_BEGIN

template<typename Component, template<typename> class Storage>
class ComponentStorageTree : ComponentStorageBase {
private:
    struct TreeElement {
        Component component;
        std::vector<EntityID> children;
        EntityID parent;

        TreeElement() : parent(EntityIDInvalid) {}
        explicit TreeElement(const Component& component) : component(component), parent(EntityIDInvalid) {}
        explicit TreeElement(Component&& component) : component(std::move(component)), parent(EntityIDInvalid) {}
    };


    using internal_iterator = typename Storage<TreeElement>::iterator;
    using internal_const_iterator = typename Storage<TreeElement>::const_iterator;
public:
    template <typename InputIterator>
    struct Iterator {
    private:

    public:
        explicit Iterator(InputIterator iterator) : wrapped(iterator) {}

        auto operator*() {
            auto cast = *wrapped;
            return boost::hana::make_tuple(cast[0_c], boost::hana::make_tuple(&cast[1_c][0_c]->component));
        };

        bool operator==(const Iterator &rhs) { return wrapped == rhs.wrapped; }
        bool operator!=(const Iterator &rhs) { return wrapped != rhs.wrapped; }

        Iterator operator++() {
            Iterator i = *this;
            wrapped++;
            return i;
        }

        Iterator operator++(int bar) {
            wrapped++;
            return *this;
        }

        bool isValid() {
            return wrapped.isValid();
        }

    private:
        InputIterator wrapped;
    };

    using iterator = Iterator<internal_iterator>;
    using const_iterator = Iterator<internal_const_iterator>;

    void add(EntityID child_id) {
        wrapped.add(child_id);
    }

    template <typename... Args>
    void add(EntityID child_id, Args&&... args) {
        wrapped.add(child_id, std::forward<Args>(args)...);
    }

    void addWithParent(EntityID child_id, EntityID parent_id) {
        wrapped.add(child_id);
        wrapped.get(child_id)->parent = parent_id;
        wrapped.get(parent_id)->children.push_back(child_id);

        incrementGeneration();
    }

    Component *get(EntityID id) {
        return &wrapped.get(id)->component;
    }

    const Component *get(EntityID id) const {
        return &wrapped.get(id)->component;
    }

    iterator begin() {
        return iterator(wrapped.begin());
    }

    iterator end() {
        return iterator(wrapped.end());
    }

    const_iterator cbegin() const {
        return const_iterator(wrapped.cbegin());
    }

    const_iterator cend() const {
        return const_iterator(wrapped.cend());
    }

    template<class UnaryFunction>
    void for_each(UnaryFunction f) {
        for (auto it = begin(); it != end(); ++it) {
            f(*it);
        }
    }

    template<class UnaryFunction>
    void tree_for_each(UnaryFunction f) {
        auto beginLocal = wrapped.begin();
        auto endLocal = wrapped.end();
        for (auto it = beginLocal; it != endLocal; ++it) {
            TreeElement* element = (*it)[1_c][0_c];
            EntityID parent = element->parent;
            if (parent == EntityIDInvalid)
                tree_for_each_process(f, (*it)[0_c], element, nullptr);
        }
    }

private:
    Storage<TreeElement> wrapped;

    template<class UnaryFunction>
    void tree_for_each_process(UnaryFunction f, EntityID id, TreeElement* element, TreeElement* parent) {
        boost::hana::tuple<EntityID, Component *, EntityID , Component*> ret(
            id, &element->component, element->parent, parent ? &parent->component : nullptr
        );
        f(ret);

        for (auto childId : element->children) {
            tree_for_each_process(f, childId, wrapped.get(childId), element);
        }
    }
};

TALON_NS_END