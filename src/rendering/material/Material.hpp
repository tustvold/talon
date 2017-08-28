#pragma once
#include "TalonConfig.hpp"
#include "TalonTypes.hpp"
#include <vulkan/vulkan.hpp>
#include "ecs/IdentifierPool.hpp"
#include "MaterialPipeline.hpp"

TALON_NS_BEGIN

class SwapChain;
class RenderPass;
class Material;

class Material {
public:
    Material(const Material&) = delete;
    Material(Material&&) = delete;

    virtual ~Material();

    auto getMaterialID() const {
        return materialID;
    }

    virtual std::unique_ptr<MaterialPipeline> createPipeline(SwapChain *swapChain, RenderPass *renderPass) = 0;

protected:
    Material();

    static std::vector<char> readFile(const std::string &filename);
    static vk::ShaderModule createShaderModule(std::vector<char> code);

private:
    const MaterialID materialID;

    static IdentifierPool<MaterialID, MaxMaterialID> identifierPool;
};

TALON_NS_END