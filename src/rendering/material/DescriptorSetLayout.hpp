#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <boost/hana.hpp>

TALON_NS_BEGIN

class DescriptorSetLayoutBase {
public:
    DescriptorSetLayoutBase(const DescriptorSetLayoutBase&) = delete;
    DescriptorSetLayoutBase(DescriptorSetLayoutBase&&) = delete;

protected:
    vk::DescriptorSetLayout descriptorSetLayout;

    DescriptorSetLayoutBase() = default;
    
    static vk::DescriptorSetLayout create(vk::DescriptorSetLayoutCreateInfo &createInfo);
    static void destroy(vk::DescriptorSetLayout layout);
};

template<uint32_t maxAllocated, typename... Components>
class DescriptorSetLayout : public DescriptorSetLayoutBase {
public:
    DescriptorSetLayout() {
        descriptorSetLayout = getDescriptorSet();
    }
    DescriptorSetLayout(const DescriptorSetLayout&) = delete;
    DescriptorSetLayout(DescriptorSetLayout&&) = delete;

    ~DescriptorSetLayout() {
        destroy(descriptorSetLayout);
    }

    template <typename T>
    static constexpr bool checkComponentsTuple(T t) {
        return boost::hana::fold_left(boost::hana::tuple_t<Components...>, true, [t](auto acc, auto t){
            using unwrapped = typename decltype(t)::type;
            return acc && boost::hana::contains(t, boost::hana::type_c<unwrapped>);
        });
    }

    static constexpr uint32_t countBindings(vk::DescriptorType type) {
        uint32_t count = 0;

        boost::hana::for_each(boost::hana::tuple_t<Components...>, [&count, type](auto t){
            using unwrapped = typename decltype(t)::type;

            if (unwrapped::getDescriptorType() == type)
                count += getMaxAllocated();
        });

        return count;
    }

    static constexpr uint32_t getMaxAllocated() {
        return maxAllocated;
    }

private:
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