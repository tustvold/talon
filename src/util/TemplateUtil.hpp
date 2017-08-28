#pragma once
#include <boost/hana.hpp>

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __COUNTER__)


#define CREATE_HAS_STATIC_MEMBER(func_name, member) static constexpr auto func_name ## _helper = boost::hana::is_valid([](auto t) -> decltype((void) decltype(t)::type::member) {}); \
template <typename THIS_IS_UNIQUE> \
static constexpr bool func_name() { \
    return decltype(func_name ## _helper(boost::hana::type_c<THIS_IS_UNIQUE>))::value; \
}

#define ASSERT_HAS_STATIC_MEMBER_FUNC_NAME(func_name, member, type) \
    CREATE_HAS_STATIC_MEMBER(func_name, member) \
    static_assert(func_name<type>(), "Type - type doesn't have member: " # member)

#define ASSERT_HAS_STATIC_MEMBER(member, type) ASSERT_HAS_STATIC_MEMBER_FUNC_NAME(MAKE_UNIQUE(assert_has_static_member), member, type)

#define CREATE_HAS_MEMBER(func_name, member) auto func_name ## _helper = boost::hana::is_valid([](auto t) -> decltype((void)boost::hana::traits::declval(t).member) { }); \
template <typename THIS_IS_UNIQUE> \
static constexpr bool func_name() { \
    return decltype(func_name ## _helper(boost::hana::type_c<THIS_IS_UNIQUE>))::value; \
}

#define ASSERT_HAS_MEMBER_FUNC_NAME(func_name, member, type) \
    CREATE_HAS_MEMBER(func_name, member) \
    static_assert(func_name<type>(), "Type - type doesn't have member: " # member)

#define ASSERT_HAS_MEMBER(member, type) ASSERT_HAS_MEMBER_FUNC_NAME(MAKE_UNIQUE(assert_has_static_member), member, type)

TALON_NS_BEGIN

namespace util {
    template <typename ToGet, typename... Types>
    constexpr auto getIndexOf() {
        constexpr auto
        index = boost::hana::index_if(boost::hana::tuple_t<Types...>, [](auto t) {
            return t == boost::hana::type_c<ToGet>;
        });
        static_assert(index != boost::hana::nothing, "Could not find requested type in parameter pack");
        return *index;
    }
}

TALON_NS_END