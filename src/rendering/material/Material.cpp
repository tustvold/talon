#include "Material.hpp"
#include "rendering/system/RenderPass.hpp"

USING_TALON_NS;

IdentifierPool<MaterialID, MaxMaterialID> Material::identifierPool;

Material::Material() : materialID(identifierPool.get()){

}

Material::~Material() {
    identifierPool.free(materialID);
}
