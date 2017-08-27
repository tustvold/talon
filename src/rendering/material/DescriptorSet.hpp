#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <boost/hana.hpp>

TALON_NS_BEGIN

class DescriptorSet {
public:
    template<typename... Components>
    static vk::DescriptorSetLayout createDescriptorSet() {
        std::array<vk::DescriptorSetLayoutBinding, sizeof...(Components)> bindings;
        vk::DescriptorSetLayoutCreateInfo createInfo;

        int i = 0;
        boost::hana::for_each(boost::hana::tuple_t<Components...>, [&i, &bindings](auto &&t) {
            using wrapped = typename decltype(t)::type;
            bindings[i++] = wrapped::getDescriptorBinding();
        });

        createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        createInfo.pBindings = bindings.data();

        return createDescriptorSetInternal(createInfo);
    }

private:
    static vk::DescriptorSetLayout createDescriptorSetInternal(vk::DescriptorSetLayoutCreateInfo& createInfo);
};

TALON_NS_END