#pragma once
#include "TalonConfig.hpp"
#include <cstdint>

TALON_NS_BEGIN

using EntityID = unsigned int;
static const EntityID MaxEntityID = 1024;
static const EntityID EntityIDInvalid = std::numeric_limits<EntityID>::max();

static_assert(std::numeric_limits<EntityID>::max() > MaxEntityID, "EntityID too small");

using ComponentGenerationID = uint64_t;

using MaterialID = uint32_t;
static const MaterialID MaxMaterialID = 1024;

TALON_NS_END