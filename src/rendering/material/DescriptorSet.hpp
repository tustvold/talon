#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <boost/hana.hpp>

TALON_NS_BEGIN

namespace detail {
class DescriptorSetImpl {
protected:
    static vk::DescriptorSetLayout createDescriptorSetInternal(vk::DescriptorSetLayoutCreateInfo& createInfo);
};
}

template<typename... Components>
class DescriptorSet : detail::DescriptorSetImpl {
public:
    static vk::DescriptorSetLayout createDescriptorSet() {
        std::array<vk::DescriptorSetLayoutBinding, sizeof...(Components)> bindings;
        vk::DescriptorSetLayoutCreateInfo createInfo;

        int i = 0;
        boost::hana::for_each(boost::hana::tuple_t<Components...>, [&i, &bindings](auto t) {
            using wrapped = typename decltype(t)::type;
            bindings[i++] = wrapped::getDescriptorBinding();
        });

        createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        createInfo.pBindings = bindings.data();

        return detail::DescriptorSetImpl::createDescriptorSetInternal(createInfo);
    }

private:

};

TALON_NS_END