#pragma once
#include <cstddef>
#include <limits>

#define TALON_NS talon
#define TALON_NS_BEGIN namespace TALON_NS {
#define TALON_NS_END }
#define USING_TALON_NS using namespace TALON_NS

TALON_NS_BEGIN

template <typename... Components>
class TWorld;

struct ComponentMeshFilter;
struct ComponentTransform;

using World = TWorld<ComponentTransform, ComponentMeshFilter>;

using EntityID = unsigned int;
static const EntityID MaxEntityID = 1024;

static_assert(std::numeric_limits<EntityID>::max() > MaxEntityID, "EntityID too small");

TALON_NS_END