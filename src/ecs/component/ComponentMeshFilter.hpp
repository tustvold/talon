#pragma once
#include <rendering/mesh/Mesh.hpp>
#include <rendering/material/Material.hpp>
#include "TalonConfig.hpp"

TALON_NS_BEGIN

struct ComponentMeshFilter {
    Mesh* mesh;
    Material* material;
};

TALON_NS_END