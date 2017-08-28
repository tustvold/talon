#pragma once
#include "TalonConfig.hpp"
#include "TalonTypes.hpp"

#include "ecs/TWorld.hpp"
#include "ecs/ComponentStorage.hpp"
#include "ecs/ComponentStorageVector.hpp"
#include "ecs/ComponentStorageMap.hpp"
#include "ecs/ComponentStorageTree.hpp"

#include <ecs/component/ComponentTransform.hpp>
#include <ecs/component/ComponentTransformTree.hpp>
#include <ecs/component/ComponentMeshFilter.hpp>
#include <ecs/ComponentStorageFlatMap.hpp>
#include <ecs/component/ComponentModelMatrix.hpp>

TALON_NS_BEGIN

using World = TWorld<ComponentModelMatrix, ComponentTransform, ComponentTransformTree, ComponentMeshFilter>;

COMPONENT_STORAGE_DEF_TREE(ComponentTransformTree, ComponentStorageFlatMap);
COMPONENT_STORAGE_DEF(ComponentMeshFilter, ComponentStorageFlatMap);
COMPONENT_STORAGE_DEF(ComponentTransform, ComponentStorageFlatMap);
COMPONENT_STORAGE_DEF(ComponentModelMatrix, ComponentStorageVector);

TALON_NS_END