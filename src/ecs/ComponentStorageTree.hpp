#pragma once
#include "TalonConfig.hpp"
#include "ComponentStorage.hpp"
#include <vector>
#include <ecs/component/TreeComponentData.hpp>

TALON_NS_BEGIN

template<typename Component, template<typename> class Storage>
class ComponentStorageTree : public ComponentStorageBase {
public:
    using iterator = typename Storage<Component>::iterator;
    using const_iterator = typename Storage<Component>::const_iterator;

    ComponentStorageTree() : first(EntityIDInvalid) {

    }

    template <typename... Args>
    void add(EntityID child_id, Args&&... args) {
        wrapped.add(child_id, std::forward<Args>(args)...);
        addSiblingOf(first, child_id);
        incrementGeneration();
    }

    void add(EntityID child_id) {
        wrapped.add(child_id);
        addSiblingOf(first, child_id);
        incrementGeneration();

    }

    void addWithParent(EntityID child_id, EntityID parent_id) {
        wrapped.add(child_id);
        auto parent = wrapped.get(parent_id);
        addSiblingOf(parent->treeComponentData.first_child, child_id);
        incrementGeneration();
    }

    inline Component *get(EntityID id) {
        return wrapped.get(id);
    }

    inline const Component *get(EntityID id) const {
        return wrapped.get(id);
    }

    inline iterator begin() {
        return wrapped.begin();
    }

    inline iterator end() {
        return wrapped.end();
    }

    inline const_iterator cbegin() const {
        return wrapped.cbegin();
    }

    inline const_iterator cend() const {
        return wrapped.cend();
    }

    template<class UnaryFunction>
    void for_each(UnaryFunction f) {
        for (auto it = begin(); it != end(); ++it) {
            f(*it);
        }
    }

    template<class UnaryFunction>
    void tree_for_each(UnaryFunction f) {
        if (first == EntityIDInvalid)
            return;
        tree_for_each_process_siblings(f, first, EntityIDInvalid, nullptr);
    }

private:
    Storage<Component> wrapped;
    EntityID first;

    template<class UnaryFunction>
    void tree_for_each_process_siblings(UnaryFunction f, EntityID id, EntityID  parent_id, Component* parent) {
        do {
            Component* component = wrapped.get(id);
            tree_for_each_process(f, id, component, parent_id, parent);
            id = component->treeComponentData.next_sibling;
        } while (id != EntityIDInvalid);
    }

    template<class UnaryFunction>
    void tree_for_each_process(UnaryFunction f, EntityID id, Component* element, EntityID  parent_id, Component* parent) {
        boost::hana::tuple<EntityID, Component*, EntityID , Component*> ret(
            id, element, parent_id, parent
        );
        f(ret);
        if (element->treeComponentData.first_child != EntityIDInvalid)
            tree_for_each_process_siblings(f, element->treeComponentData.first_child, id, element);
    }

    static_assert(EntityIDInvalid > MaxEntityID);

    void addSiblingOf(EntityID& sibling_root, EntityID to_insert) {
        // Search through linked list to find insertion position
        // We maintain sorted order to improve performance
        // By making accesses more prefetch friendly

        if (sibling_root == EntityIDInvalid) {
            sibling_root = to_insert;
            return;
        }
        Component* to_insert_component = wrapped.get(to_insert);

        if (sibling_root > to_insert) {
            to_insert_component->treeComponentData.next_sibling = sibling_root;
            sibling_root = to_insert;
            return;
        }

        EntityID predecessor = sibling_root;
        Component* current_component = wrapped.get(predecessor);
        while (true) {
            if (current_component->treeComponentData.next_sibling > to_insert)
                break;
            predecessor = current_component->treeComponentData.next_sibling;
            current_component = wrapped.get(predecessor);
        }
        to_insert_component->treeComponentData.next_sibling = current_component->treeComponentData.next_sibling;
        current_component->treeComponentData.next_sibling = to_insert;
    }
};

TALON_NS_END