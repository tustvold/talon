#pragma once
#include "TalonConfig.hpp"
#include "TalonTypes.hpp"
#include <vulkan/vulkan.hpp>
#include "ecs/IdentifierPool.hpp"

TALON_NS_BEGIN

class DeviceManager;

class Material {
public:
    Material();
    ~Material();

    auto getMaterialID() const {
        return materialID;
    }

private:
    const MaterialID materialID;

    static IdentifierPool<MaterialID, MaxMaterialID> identifierPool;
};

TALON_NS_END