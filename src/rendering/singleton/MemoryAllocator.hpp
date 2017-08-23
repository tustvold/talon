#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>
#include "vulkan_mem_alloc.hpp"
#include "util/Logging.hpp"

TALON_NS_BEGIN

class DeviceManager;

namespace detail {
template<typename T>
struct AllocatedData;

template<typename DataType>
struct AllocatedDataMapHandle {
    explicit AllocatedDataMapHandle(const AllocatedData<DataType> *ref) : ref(ref), ptr(nullptr) {
        auto result = static_cast<vk::Result>(vmaMapMemory(ref->allocator, ref->allocation, &ptr));
        TASSERT(result == vk::Result::eSuccess);
    }

    ~AllocatedDataMapHandle() {
        vmaUnmapMemory(ref->allocator, ref->allocation);
    }

    void copy(const void* data, size_t size) {
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
    VmaAllocator allocator;
    T data;
    VmaAllocation allocation;
    VmaAllocationInfo allocationInfo;
    size_t requestedSize;

    explicit AllocatedData(VmaAllocator allocator, size_t requestedSize)
        : allocator(allocator), data(nullptr), allocation(nullptr), requestedSize(requestedSize) {}

    AllocatedData() : AllocatedData(nullptr, 0) {}

    detail::AllocatedDataMapHandle<T> map() {
        return AllocatedDataMapHandle<T>(this);
    }
};
}

using MappedBufferHandle = detail::AllocatedDataMapHandle<vk::Buffer>;

class MemoryAllocator {
public:
    explicit MemoryAllocator(DeviceManager *deviceManager);
    ~MemoryAllocator();

    VmaAllocator getAllocator() const {
        return allocator;
    }

private:
    VmaAllocator allocator;
};

struct Buffer : public detail::AllocatedData<vk::Buffer> {
    Buffer(const MemoryAllocator *memoryAllocator, const vk::BufferCreateInfo *pCreateInfo,
           const VmaMemoryRequirements *pMemoryRequirements) {
        requestedSize = pCreateInfo->size;
        allocator = memoryAllocator->getAllocator();

        auto result = static_cast<vk::Result>(vmaCreateBuffer(allocator,
                                                              reinterpret_cast<const VkBufferCreateInfo *>(pCreateInfo),
                                                              pMemoryRequirements,
                                                              reinterpret_cast<VkBuffer *>(&data),
                                                              &allocation,
                                                              &allocationInfo));
        TASSERT(result == vk::Result::eSuccess);
    }

    ~Buffer() {
        vmaDestroyBuffer(allocator, data, allocation);
    }
};

TALON_NS_END