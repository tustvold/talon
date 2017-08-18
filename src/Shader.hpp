#pragma once
#include "talon.hpp"
#include <vulkan/vulkan.hpp>
#include <DeviceManager.hpp>

TALON_NS_BEGIN

class Shader {
public:
    Shader(const std::string &path,
               vk::ShaderStageFlagBits shaderStage,
               const std::shared_ptr<DeviceManager>& deviceManager);

    ~Shader();

    vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo(const char* pName = "main");

private:
    vk::ShaderModule shaderModule_;
    vk::ShaderStageFlagBits shaderStage_;
    std::shared_ptr<DeviceManager> deviceManager_;
};


TALON_NS_END