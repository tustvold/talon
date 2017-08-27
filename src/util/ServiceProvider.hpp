#pragma once
#include "TalonConfig.hpp"
#include "Logging.hpp"

TALON_NS_BEGIN

template <typename Service, typename Default>
class ServiceProvider {
public:
    ServiceProvider() noexcept : current(&defaultInstance) {

    }

    const Service* get() noexcept {
        return current;
    }

    const Service* operator->() noexcept {
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

template <typename Service>
class ServiceProvider<Service, nullptr_t> {
public:
    ServiceProvider() noexcept : current(nullptr) {

    }

    Service* get() noexcept {
        TASSERT(current != nullptr);
        return current;
    }

    Service* operator->() noexcept {
        TASSERT(current != nullptr);
        return get();
    }

    void set(Service *service) {
        TASSERT(current == nullptr);
        current = service;
    }

    void clear(Service *service) {
        TASSERT(current == service);
        current = nullptr;
    }

private:
    Service* current;
};

TALON_NS_END