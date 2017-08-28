#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include "vulkan_mem_alloc.hpp"


TALON_NS_BEGIN

template<typename T>
struct AllocatedData;

struct AllocatedDataMapHandleBase {
    void mapMemory(VmaAllocation allocation, void** ptr);
    void unmapMemory(VmaAllocation allocation);
};

template<typename DataType>
struct AllocatedDataMapHandle : AllocatedDataMapHandleBase {
    explicit AllocatedDataMapHandle(const AllocatedData<DataType> *ref) : ref(ref), ptr(nullptr) {
        mapMemory(ref->allocation, &ptr);
    }

    AllocatedDataMapHandle(const AllocatedDataMapHandle&) = delete;
    AllocatedDataMapHandle(AllocatedDataMapHandle&&) = delete;

    ~AllocatedDataMapHandle() {
        unmapMemory(ref->allocation);
    }

    void copy(const void *data, size_t size) {
        TASSERT(ref->requestedSize >= size);
        memcpy(ptr, data, size);
    }

    template<typename T>
    void copy(const std::vector<T> &proxy) {
        auto size = proxy.size() * sizeof(T);
        copy(proxy.data(), size);
    }

private:
    const AllocatedData<DataType> *ref;
    void *ptr;
};

template<typename T>
struct AllocatedData {
    T data;
    VmaAllocation allocation;
    VmaAllocationInfo allocationInfo;
    size_t requestedSize;

    AllocatedDataMapHandle<T> map() {
        return AllocatedDataMapHandle<T>(this);
    }

protected:

    explicit AllocatedData(size_t requestedSize)
        : data(nullptr), allocation(nullptr), allocationInfo({}), requestedSize(requestedSize) {}

    AllocatedData() : AllocatedData(0) {}
    AllocatedData(const AllocatedData&) = delete;
    AllocatedData(AllocatedData&&) = delete;
};


TALON_NS_END