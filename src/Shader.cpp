#include "Shader.hpp"
#include <vulkan/vulkan.hpp>
#include <fstream>

USING_TALON_NS;

static std::vector<char> readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    auto fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

Shader::Shader(const std::string &path,
               vk::ShaderStageFlagBits shaderStage,
               const std::shared_ptr<DeviceManager>& deviceManager) : shaderStage_(shaderStage), deviceManager_(deviceManager) {
    auto code = readFile(path);

    vk::ShaderModuleCreateInfo createInfo;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    shaderModule_ = deviceManager_->getDevice().createShaderModule(createInfo);
}
Shader::~Shader() {
    deviceManager_->getDevice().destroyShaderModule(shaderModule_);

}
vk::PipelineShaderStageCreateInfo Shader::getPipelineShaderStageCreateInfo(const char* pName) {
    vk::PipelineShaderStageCreateInfo ret;
    ret.stage = shaderStage_;
    ret.module = shaderModule_;
    ret.pName = pName;
    return ret;
}
