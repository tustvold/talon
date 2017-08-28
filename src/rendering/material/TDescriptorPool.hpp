#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <boost/hana.hpp>
#include "DescriptorSet.hpp"
#include <type_traits>

TALON_NS_BEGIN

class DescriptorPoolBase {
public:
    using DescriptorPoolCountBindingsArray = std::array<int, VK_DESCRIPTOR_TYPE_END_RANGE - VK_DESCRIPTOR_TYPE_BEGIN_RANGE + 1>;

    template <typename T>
    static constexpr int DescriptorPoolCountBindings(T tuple, vk::DescriptorType type) {
        int count = 0;

        boost::hana::for_each(tuple, [&count, type](auto t){
            using unwrapped = typename decltype(t)::type;

            if (unwrapped::getDescriptorType() == type)
                count++;
        });

        return count;
    }

    template <typename T>
    static constexpr DescriptorPoolCountBindingsArray getDescriptorCounts(T tuple) {
        DescriptorPoolCountBindingsArray array = {};
        for (int i = 0; i < VK_DESCRIPTOR_TYPE_END_RANGE - VK_DESCRIPTOR_TYPE_BEGIN_RANGE + 1; i++) {
            array[i] = DescriptorPoolCountBindings(tuple, static_cast<vk::DescriptorType>(i + VK_DESCRIPTOR_TYPE_BEGIN_RANGE));
        }

        return array;
    }

protected:
    vk::DescriptorPool create(size_t num_descriptorSets, const DescriptorPoolCountBindingsArray& array);
    void destroy(vk::DescriptorPool pool);
};

template <typename World, typename... DescriptorSets>
class TDescriptorPool : DescriptorPoolBase {
    static constexpr auto check_inheritance = boost::hana::fold_left(boost::hana::tuple_t<DescriptorSets...>, true, [](auto acc, auto t){
        using unwrapped = typename decltype(t)::type;
        return acc && std::is_base_of<DescriptorSetBase, unwrapped>::value;
    });

    static constexpr auto check_world = boost::hana::fold_left(boost::hana::tuple_t<DescriptorSets...>, true, [](auto acc, auto t){
        using unwrapped = typename decltype(t)::type;
        return acc && unwrapped::checkComponentsTuple(World::SystemComponentsTuple);
    });


    static_assert(check_inheritance, "All DescriptorSets must inherit from DescriptorSetBase");
    static_assert(check_world, "Cannot use Binding that doesn't exist in World type");
public:

    TDescriptorPool() {
        create(sizeof...(DescriptorSets), getDescriptorCounts(World::SystemComponentsTuple));
    }

    ~TDescriptorPool() {
        destroy(descriptorPool);
    }

protected:
    vk::DescriptorPool descriptorPool;
    boost::hana::tuple<DescriptorSets...> descriptors;
};

TALON_NS_END
