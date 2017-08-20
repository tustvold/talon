#pragma once
#include "TalonConfig.hpp"
#include "Logging.hpp"

TALON_NS_BEGIN

template <typename Service, typename Default>
class ServiceProvider {
public:
    ServiceProvider() : current(&defaultInstance) {

    }

    const Service* get() const noexcept {
        return current;
    }

    const Service* operator->() const noexcept {
        return get();
    }

    void set(Service *service) {
        TASSERT(current == &defaultInstance);
        current = service;
    }

    void clear(Service *service) {
        TASSERT(current == service);
        current = &defaultInstance;
    }

private:
    Service* current;
    Default defaultInstance;
};

TALON_NS_END