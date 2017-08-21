#pragma once
#include <Eigen/Core>
#include <vulkan/vulkan.hpp>
#include "Util.hpp"

TALON_NS_BEGIN

struct Vertex {
    Eigen::Vector2f pos;
    Eigen::Vector3f color;

    static_assert(sizeof(pos) == 2 * sizeof(float), "asddas");
    static_assert(sizeof(color) == 3 * sizeof(float), "asddas");

    static vk::VertexInputBindingDescription getBindingDescription() {
        vk::VertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;

        return bindingDescription;
    }

    static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
        attributeDescriptions[0].offset = static_cast<uint32_t>(util::offset_of(&Vertex::pos));

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[1].offset = static_cast<uint32_t>(util::offset_of(&Vertex::color));

        return attributeDescriptions;
    }
};

TALON_NS_END