#pragma once
#include "TalonConfig.hpp"
#include "ECSConfig.hpp"

#include "ecs/TWorld.hpp"
#include "ecs/ComponentStorage.hpp"
#include "ecs/ComponentStorageArray.hpp"
#include "ecs/ComponentStorageMap.hpp"
#include "ecs/ComponentStorageTree.hpp"

#include <component/ComponentTransform.hpp>
#include <component/ComponentMeshFilter.hpp>


TALON_NS_BEGIN

using World = TWorld<ComponentTransform, ComponentMeshFilter>;

COMPONENT_STORAGE_DEF_TREE(ComponentTransform, ComponentStorageArray);
COMPONENT_STORAGE_DEF(ComponentMeshFilter, ComponentStorageMap);

TALON_NS_END