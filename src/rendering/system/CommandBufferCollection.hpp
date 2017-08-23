#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"

TALON_NS_BEGIN

class CommandBufferCollection {
public:
    explicit CommandBufferCollection(uint32_t size, vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);
    ~CommandBufferCollection();

    const std::vector<vk::CommandBuffer>& get() {
        return collection;
    }

    auto& operator[](size_t index) {
        return collection[index];
    }

    auto data() {
        return collection.data();
    }

    uint32_t size() {
        return static_cast<uint32_t>(collection.size());
    }

private:
    std::vector<vk::CommandBuffer> collection;
};


TALON_NS_END