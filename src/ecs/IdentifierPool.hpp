#pragma once
#include "TalonConfig.hpp"
#include <stack>
#include <TalonTypes.hpp>
#include <Logging.hpp>

TALON_NS_BEGIN

template <typename T, size_t max>
class IdentifierPool {
public:
    IdentifierPool() noexcept : next(0) {

    }

    T get() {
        if (!reserve.empty()) {
            auto ret = reserve.top();
            reserve.pop();
            return ret;
        }
        TASSERT(next < max);
        return next++;
    }

    void free(T id) {
        reserve.push(id);
    }

private:
    std::stack<T> reserve;
    T next;
};

TALON_NS_END