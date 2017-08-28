#pragma once
#include <Eigen/Core>
#include <vulkan/vulkan.hpp>
#include "util/Util.hpp"

TALON_NS_BEGIN

struct TestVertex {
    Eigen::Vector2f pos;
    Eigen::Vector3f color;

    static vk::VertexInputBindingDescription getBindingDescription() {
        vk::VertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(TestVertex);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;

        return bindingDescription;
    }

    static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
        attributeDescriptions[0].offset = static_cast<uint32_t>(util::offset_of(&TestVertex::pos));

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[1].offset = static_cast<uint32_t>(util::offset_of(&TestVertex::color));

        return attributeDescriptions;
    }
};

TALON_NS_END