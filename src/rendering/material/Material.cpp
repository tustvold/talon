#include "Material.hpp"
#include "rendering/system/RenderPass.hpp"
#include <fstream>
#include <rendering/singleton/RenderServiceTable.hpp>
#include <rendering/singleton/DeviceManager.hpp>

USING_TALON_NS;

IdentifierPool<MaterialID, MaxMaterialID> Material::identifierPool;

std::vector<char> Material::readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        TLOGFATAL("Failed to open file %s", filename.c_str());
    }

    auto fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

vk::ShaderModule Material::createShaderModule(std::vector<char> code) {
    vk::ShaderModuleCreateInfo createInfo = {};
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    return RenderServiceTable::deviceManager->createShaderModule(createInfo);
}

Material::Material() : materialID(identifierPool.get()){

}

Material::~Material() {
    identifierPool.free(materialID);
}
