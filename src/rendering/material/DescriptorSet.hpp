#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <boost/hana.hpp>

TALON_NS_BEGIN

class DescriptorSetBase {
protected:
    static vk::DescriptorSetLayout create(vk::DescriptorSetLayoutCreateInfo &createInfo);
    static void destroy(vk::DescriptorSetLayout layout);
};

template<typename... Components>
class DescriptorSet : public DescriptorSetBase {
public:
    DescriptorSet() {
        descriptorSetLayout = getDescriptorSet();
    }

    ~DescriptorSet() {
        destroy(descriptorSetLayout);
    }

    template <typename T>
    static constexpr bool checkComponentsTuple(T t) {
        return boost::hana::fold_left(boost::hana::tuple_t<Components...>, true, [t](auto acc, auto t){
            using unwrapped = typename decltype(t)::type;
            return acc && boost::hana::contains(t, boost::hana::type_c<unwrapped>);
        });
    }

private:
    vk::DescriptorSetLayout descriptorSetLayout;

    static vk::DescriptorSetLayout getDescriptorSet() {
        std::array<vk::DescriptorSetLayoutBinding, sizeof...(Components)> bindings;
        vk::DescriptorSetLayoutCreateInfo createInfo;

        int i = 0;
        boost::hana::for_each(boost::hana::tuple_t<Components...>, [&i, &bindings](auto t) {
            using wrapped = typename decltype(t)::type;
            bindings[i++] = wrapped::getDescriptorBinding();
        });

        createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        createInfo.pBindings = bindings.data();

        return create(createInfo);
    }
};

TALON_NS_END