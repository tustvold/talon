#include "Material.hpp"
#include "rendering/mesh/Vertex.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/DeviceManager.hpp"
#include "rendering/system/RenderPass.hpp"
#include "rendering/system/SwapChain.hpp"

USING_TALON_NS;

IdentifierPool<MaterialID, MaxMaterialID> Material::identifierPool;

Material::Material() : materialID(identifierPool.get()){

}

Material::~Material() {
    identifierPool.free(materialID);
}
