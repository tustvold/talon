#pragma once
#include "TalonConfig.hpp"
#include <stack>

TALON_NS_BEGIN

class IdentifierPool {
public:
    IdentifierPool() : next(0) {

    }

    EntityID get() {
        if (!reserve.empty()) {
            auto ret = reserve.top();
            reserve.pop();
            return ret;
        }
        TASSERT(next < MaxEntityID);
        return next++;
    }

private:
    std::stack<EntityID> reserve;
    EntityID next;
};

TALON_NS_END