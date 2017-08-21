#pragma once
#include "TalonConfig.hpp"
#include "ComponentStorage.hpp"

TALON_NS_BEGIN

template<typename Component, template<typename> class Storage>
class ComponentStorageTree : ComponentStorageBase {
private:
    struct TreeElement {
        Component component;
        std::vector<EntityID> children;
        EntityID parent;

        TreeElement() : parent(EntityIDInvalid) {

        }
    };

public:

    struct Iterator {
    private:
        using InputIterator = typename Storage<TreeElement>::Iterator;
    public:
        explicit Iterator(InputIterator iterator) : wrapped(iterator) {}

        boost::hana::tuple<EntityID, boost::hana::tuple<Component *>> operator*() {
            boost::hana::tuple<EntityID, boost::hana::tuple<TreeElement *>> cast = *wrapped;

            boost::hana::tuple<EntityID, boost::hana::tuple<Component *>> ret(
                cast[0_c], boost::hana::make_tuple(&cast[1_c][0_c]->component)
            );
            return ret;
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

    void add(EntityID child_id) {
        wrapped.add(child_id);
    }

    void add(EntityID child_id, EntityID parent_id) {
        wrapped.add(child_id);
        wrapped.get(child_id)->parent = parent_id;
        wrapped.get(parent_id)->children.push_back(child_id);
    }

    Component *get(EntityID id) {
        return &wrapped.get(id)->component;
    }

    auto begin() {
        return wrapped.begin();
    }

    auto end() {
        return wrapped.end();
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