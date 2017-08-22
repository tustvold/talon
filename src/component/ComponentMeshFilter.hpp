#pragma once
#include <rendering/Mesh.hpp>
#include <rendering/Material.hpp>
#include "TalonConfig.hpp"

TALON_NS_BEGIN

struct ComponentMeshFilter {
    Mesh* mesh;
    Material* material;
};

TALON_NS_END