#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <boost/hana.hpp>
#include "DescriptorBinding.hpp"

TALON_NS_BEGIN

class DescriptorSetBase {
    DescriptorSetBase(const DescriptorSetBase&) = delete;
    DescriptorSetBase(DescriptorSetBase&&) = delete;

protected:
    vk::DescriptorSet descriptorSet;

    DescriptorSetBase(const vk::DescriptorSetLayout& layout);
    ~DescriptorSetBase();
};

template <typename... Components>
class DescriptorSet : DescriptorSetBase {
    template <uint32_t maxAllocated, typename... _Components>
    friend class DescriptorSetLayout;

private:
    boost::hana::tuple<DescriptorBinding<Components>...> data;

    DescriptorSet(const vk::DescriptorSetLayout& layout) : DescriptorSetBase(layout) {

    }
};

TALON_NS_END


