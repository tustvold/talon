#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <boost/hana.hpp>
#include <util/TemplateUtil.hpp>
#include "DescriptorBinding.hpp"

TALON_NS_BEGIN

class DescriptorSetBase {
    DescriptorSetBase(const DescriptorSetBase &) = delete;
    DescriptorSetBase(DescriptorSetBase &&) = delete;

protected:
    vk::DescriptorSet descriptorSet;

    DescriptorSetBase(const vk::DescriptorSetLayout &layout);
    ~DescriptorSetBase();
};

template<typename... Components>
class DescriptorSet : DescriptorSetBase {
    template<uint32_t maxAllocated, typename... _Components>
    friend
    class DescriptorSetLayout;
public:
    template<typename Component>
    DescriptorBinding<Component> &getBinding() {
        constexpr auto index = util::getIndexOf<DescriptorBinding<Component>, DescriptorBinding<Components>...>();
        return boost::hana::at(bindings, index);
    }

private:
    boost::hana::tuple<DescriptorBinding<Components>...> bindings;

    DescriptorSet(const vk::DescriptorSetLayout &layout) :
        DescriptorSetBase(layout),
        bindings(makeBindings(std::make_index_sequence<sizeof...(Components)>())) {

    }

    template <size_t... I>
    boost::hana::tuple<DescriptorBinding<Components>...> makeBindings(std::index_sequence<I...>) {
        return boost::hana::tuple<DescriptorBinding<Components>...>(I...);
    }

};

TALON_NS_END


