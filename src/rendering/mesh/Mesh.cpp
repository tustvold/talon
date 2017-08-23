#include <rendering/system/CommandBuffer.hpp>
#include "Mesh.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/MemoryAllocator.hpp"

USING_TALON_NS;

Mesh::Mesh(const Mesh::GenericMeshData &data) : num_vertices(data.num_vertices) {
    vk::BufferCreateInfo bufferInfo = {};

    uint32_t vertices_size_in_bytes = data.num_vertices * data.vertex_size;
    bufferInfo.size = vertices_size_in_bytes;
    bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    VmaMemoryRequirements requirements = {};
    requirements.requiredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    vertexBuffer = std::make_unique<Buffer>(RenderServiceTable::memoryAllocator.get(), &bufferInfo, &requirements);

    auto handle = vertexBuffer->map();
    handle.copy(data.vertices, vertices_size_in_bytes);
}

Mesh::~Mesh() = default;

void Mesh::bind(CommandBuffer* commandBuffer) {
    vk::Buffer vertexBuffers[] = {vertexBuffer->data};
    vk::DeviceSize offsets[] = {0};

    commandBuffer->bindVertexBuffers(0, 1, vertexBuffers, offsets);
}

void Mesh::draw(CommandBuffer* commandBuffer) {
    commandBuffer->draw(getNumVertices(), 1, 0, 0);
}


