#pragma once
#include "TalonConfig.hpp"

TALON_NS_BEGIN
namespace util {

template <typename T1, typename T2>
inline size_t offset_of(T1 T2::*member) {
    static T2 obj;
    return size_t(&(obj.*member)) - size_t(&obj);
}

}


TALON_NS_END
