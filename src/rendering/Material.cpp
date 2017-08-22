#include "Material.hpp"
#include "Vertex.hpp"
#include "ApplicationServiceTable.hpp"
#include "DeviceManager.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

USING_TALON_NS;

IdentifierPool<MaterialID, MaxMaterialID> Material::identifierPool;

Material::Material() : materialID(identifierPool.get()){

}

Material::~Material() {
    identifierPool.free(materialID);
}
