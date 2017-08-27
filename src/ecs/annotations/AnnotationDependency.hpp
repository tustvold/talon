#pragma once
#include "TalonConfig.hpp"
#include <boost/hana.hpp>
#include <util/TemplateUtil.hpp>

using namespace boost::hana::literals;

TALON_NS_BEGIN

namespace detail {

CREATE_HAS_STATIC_MEMBER(has_dependencies, getDependencies())

template <typename T, bool = has_dependencies<T>()>
struct DependencyGetter;

template <typename T>
struct DependencyGetter<T, true> {
    static constexpr auto get() {
        return T::getDependencies();
    }
};

template <typename T>
struct DependencyGetter<T, false> {
    static constexpr auto get() {
        return boost::hana::make_tuple();
    }
};

}

template<typename... Components>
struct DependencyChecker {
private:
    static constexpr auto ComponentsTuple = boost::hana::tuple_t<typename std::remove_reference<Components>::type ...>;

public:
    template<typename T>
    static constexpr bool hasType() {
        return boost::hana::contains(ComponentsTuple, boost::hana::type_c<T>);
    }


    template<typename T>
    static constexpr auto getComponentDependencies()  {
        return detail::DependencyGetter<T>::get();
    }

    template <typename T>
    static constexpr bool componentDependenciesSatisfiedIndividual() {
        return boost::hana::fold_left(getComponentDependencies<T>(), true, [](bool acc, auto t){
            return acc &= boost::hana::contains(ComponentsTuple, t);
        });
    }

    static constexpr bool componentDependenciesSatisfied() {
        return boost::hana::fold_left(ComponentsTuple, true, [](bool acc, auto t){
            using T = typename decltype(t)::type;
            return acc &= componentDependenciesSatisfiedIndividual<T>();
        });
    }

    DependencyChecker() = delete;
};

TALON_NS_END