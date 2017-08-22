#pragma once
#include "TalonConfig.hpp"
#include <vector>
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class Buffer;

template<typename Vertex>
struct MeshData {
    std::vector<Vertex> vertices;

    explicit MeshData(const std::vector<Vertex> &vertices) : vertices(vertices) {

    }
};

template<typename Vertex>
auto makeMeshData(const std::vector<Vertex> &vertices) {
    return MeshData(vertices);
}

class Mesh {
private:
    struct GenericMeshData {
        const void *vertices;
        uint32_t num_vertices;
        uint32_t vertex_size;

        GenericMeshData(const void *vertices, uint32_t num_vertices, uint32_t vertex_size)
            : vertices(vertices), num_vertices(num_vertices), vertex_size(vertex_size) {}
    };

    explicit Mesh(const GenericMeshData &data);
public:

    template<typename Vertex>
    explicit Mesh(const MeshData<Vertex> &meshData) : Mesh(
        GenericMeshData(meshData.vertices.data(), static_cast<uint32_t>(meshData.vertices.size()), sizeof(Vertex))
    ) {}

    ~Mesh();

    void bind(vk::CommandBuffer commandBuffer);

    void draw(vk::CommandBuffer commandBuffer);

    uint32_t getNumVertices() const {
        return num_vertices;
    }

private:
    std::unique_ptr<Buffer> vertexBuffer;
    uint32_t num_vertices;
};

TALON_NS_END