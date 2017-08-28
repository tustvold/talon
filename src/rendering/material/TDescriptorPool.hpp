#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <boost/hana.hpp>
#include "DescriptorSetLayout.hpp"
#include <type_traits>

TALON_NS_BEGIN

class DescriptorPoolBase {
public:
    using DescriptorPoolCountBindingsArray = std::array<uint32_t,
                                                        VK_DESCRIPTOR_TYPE_END_RANGE - VK_DESCRIPTOR_TYPE_BEGIN_RANGE
                                                            + 1>;

    template<typename DescriptorSets>
    static constexpr DescriptorPoolCountBindingsArray getDescriptorCounts(DescriptorSets sets) {
        DescriptorPoolCountBindingsArray array = {};
        for (int i = 0; i < VK_DESCRIPTOR_TYPE_END_RANGE - VK_DESCRIPTOR_TYPE_BEGIN_RANGE + 1; i++) {
            array[i] = boost::hana::fold_left(sets, 0, [i](auto acc, auto t) {
                using unwrapped = typename decltype(t)::type;
                return acc
                    + unwrapped::countBindings(static_cast<vk::DescriptorType>(i + VK_DESCRIPTOR_TYPE_BEGIN_RANGE));
            });
        }
        return array;
    }

protected:
    vk::DescriptorPool create(size_t maxSets, const DescriptorPoolCountBindingsArray &array);
    void destroy(vk::DescriptorPool pool);
};

template<typename... DescriptorSetLayouts>
class TDescriptorPool : DescriptorPoolBase {
    static constexpr auto check_inheritance =
        boost::hana::fold_left(boost::hana::tuple_t<DescriptorSetLayouts...>, true, [](auto acc, auto t) {
            using unwrapped = typename decltype(t)::type;
            return acc && std::is_base_of<DescriptorSetLayoutBase, unwrapped>::value;
        });

    static constexpr uint32_t countMaxSets() {
        return boost::hana::fold_left(boost::hana::tuple_t<DescriptorSetLayouts...>,
                                      (uint32_t) 0,
                                      [](uint32_t acc, auto t) {
                                          using unwrapped = typename decltype(t)::type;
                                          return acc + unwrapped::getMaxAllocated();
                                      });
    }

    static_assert(check_inheritance, "All DescriptorSetLayouts must inherit from DescriptorSetLayoutBase");
public:

    TDescriptorPool() {
        create(countMaxSets(), getDescriptorCounts(boost::hana::tuple_t<DescriptorSetLayouts...>));
    }

    ~TDescriptorPool() {
        destroy(descriptorPool);
    }

protected:
    vk::DescriptorPool descriptorPool;
    boost::hana::tuple<DescriptorSetLayouts...> descriptors;
};

TALON_NS_END
