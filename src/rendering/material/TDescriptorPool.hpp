#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <boost/hana.hpp>
#include "DescriptorSetLayout.hpp"
#include "DescriptorSet.hpp"
#include "rendering/singleton/DescriptorManager.hpp"
#include <type_traits>
#include <util/TemplateUtil.hpp>

TALON_NS_BEGIN

class DescriptorPoolBase : public DescriptorManager {
    using DescriptorPoolCountBindingsArray = std::array<uint32_t,
                                                        VK_DESCRIPTOR_TYPE_END_RANGE - VK_DESCRIPTOR_TYPE_BEGIN_RANGE
                                                            + 1>;
    DescriptorPoolBase(const DescriptorPoolBase&) = delete;
    DescriptorPoolBase(DescriptorPoolBase&&) = delete;

protected:
    vk::DescriptorPool descriptorPool;

    DescriptorPoolBase(size_t maxSets, const DescriptorPoolCountBindingsArray &array);
    virtual ~DescriptorPoolBase();

private:
    vk::DescriptorSet allocateDescriptorSet(const vk::DescriptorSetLayout &layout) override;
    void freeDescriptorSet(vk::DescriptorSet set) override;
protected:

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
};

template<typename... DescriptorSetLayouts>
class TDescriptorPool : public DescriptorPoolBase {
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

    TDescriptorPool() : DescriptorPoolBase(countMaxSets(), getDescriptorCounts(boost::hana::tuple_t<DescriptorSetLayouts...>)) {
    }

    ~TDescriptorPool() final = default;

    template <typename DescriptorSetLayout>
    auto createDescriptorSet() {
        constexpr auto index = util::getIndexOf<DescriptorSetLayout, DescriptorSetLayouts...>();
        return boost::hana::at(descriptors, index).createDescriptorSet();
    }

protected:
    boost::hana::tuple<DescriptorSetLayouts...> descriptors;
};

TALON_NS_END
