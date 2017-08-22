#pragma once
#include "TalonConfig.hpp"
#include <stack>
#include <ECSConfig.hpp>
#include <Logging.hpp>

TALON_NS_BEGIN

template <typename T>
class IdentifierPool {
public:
    IdentifierPool() : next(0) {

    }

    T get() {
        if (!reserve.empty()) {
            auto ret = reserve.top();
            reserve.pop();
            return ret;
        }
        TASSERT(next < MaxEntityID);
        return next++;
    }

private:
    std::stack<T> reserve;
    T next;
};

TALON_NS_END