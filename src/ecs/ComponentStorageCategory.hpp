#pragma once
#include "TalonConfig.hpp"
#include "ComponentStorage.hpp"
#include <vector>
#include <ecs/annotations/AnnotationTree.hpp>
#include <boost/container/flat_map.hpp>
#include <ecs/annotations/Annotations.hpp>

TALON_NS_BEGIN

template<typename Component, template<typename> class Storage>
class ComponentStorageCategory : public ComponentStorageBase {
public:
    using iterator = typename Storage<Component>::iterator;
    using const_iterator = typename Storage<Component>::const_iterator;

    ComponentStorageCategory() {

    }

    template <typename... Args>
    void add(EntityID child_id, Args&&... args) {
        wrapped.add(child_id, std::forward<Args>(args)...);
        auto inserted = wrapped.get(child_id);
        auto it = first_entities.try_emplace(inserted->CATEGORY_ANNOTATION.category, EntityIDInvalid).first;
        addSiblingOf(it->second, child_id);
        incrementGeneration();
    }

    void add(EntityID child_id) {
        wrapped.add(child_id);
        auto inserted = wrapped.get(child_id);
        auto it = first_entities.try_emplace(inserted->CATEGORY_ANNOTATION.category, EntityIDInvalid).first;
        addSiblingOf(it->second, child_id);
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

    template <typename CategoryStart, typename CategoryForEach>
    void category_for_each(CategoryStart categoryStart, CategoryForEach categoryForEach) {
        for (auto it = first_entities.begin(); it != first_entities.end(); ++it) {
            CategoryID categoryID = it->first;
            EntityID entityID = it->second;
            TASSERT(entityID != EntityIDInvalid);

            categoryStart(categoryID);

            category_for_each_process_siblings(categoryForEach, categoryID, entityID);
        }
    }

private:
    boost::container::flat_map<CategoryID, EntityID> first_entities;
    Storage<Component> wrapped;

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
            to_insert_component->CATEGORY_ANNOTATION.next_sibling = sibling_root;
            sibling_root = to_insert;
            return;
        }

        EntityID predecessor = sibling_root;
        Component* current_component = wrapped.get(predecessor);
        while (true) {
            if (current_component->CATEGORY_ANNOTATION.next_sibling > to_insert)
                break;
            predecessor = current_component->CATEGORY_ANNOTATION.next_sibling;
            current_component = wrapped.get(predecessor);
        }
        to_insert_component->CATEGORY_ANNOTATION.next_sibling = current_component->CATEGORY_ANNOTATION.next_sibling;
        current_component->CATEGORY_ANNOTATION.next_sibling = to_insert;
    }

    template<class UnaryFunction>
    void category_for_each_process_siblings(UnaryFunction f, CategoryID categoryID, EntityID entityID) {
        do {
            Component* component = wrapped.get(entityID);
            f(categoryID, entityID, component);
            entityID = component->CATEGORY_ANNOTATION.next_sibling;
        } while (entityID != EntityIDInvalid);
    }
};

TALON_NS_END