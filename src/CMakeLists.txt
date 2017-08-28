
set(SOURCE_FILES application/Application.hpp
        application/Application.cpp application/VulkanLoader.cpp application/VulkanLoader.hpp ../example/TestVertex.hpp rendering/singleton/impl/VulkanDeviceManager.cpp
        rendering/singleton/impl/VulkanDeviceManager.hpp rendering/singleton/impl/VulkanInstanceManager.cpp rendering/singleton/impl/VulkanInstanceManager.hpp rendering/singleton/impl/VulkanWindowManager.cpp
        rendering/singleton/impl/VulkanWindowManager.hpp application/ApplicationInitSettings.hpp rendering/system/SwapChain.cpp
        rendering/system/SwapChain.hpp rendering/system/RenderPass.cpp rendering/system/RenderPass.hpp rendering/material/Material.cpp rendering/material/Material.hpp
        rendering/singleton/RenderServiceTable.hpp util/ServiceProvider.hpp rendering/singleton/RenderServiceTable.cpp ecs/TWorld.hpp ecs/ComponentStorageVector.hpp ecs/ComponentStorageMap.hpp
        ecs/ComponentStorage.hpp ecs/IdentifierPool.hpp util/Logging.hpp ${TALON_CONFIG_DIR}/TalonConfig.hpp ${TALON_CONFIG_DIR}/WorldConfig.hpp
        rendering/singleton/MemoryAllocator.hpp ecs/View.hpp rendering/mesh/Mesh.cpp rendering/mesh/Mesh.hpp rendering/singleton/impl/VulkanCommandPool.cpp rendering/singleton/impl/VulkanCommandPool.hpp
        application/GameLoop.cpp application/GameLoop.hpp rendering/system/VSemaphore.hpp rendering/system/VSemaphore.cpp rendering/system/CommandBuffer.cpp
        rendering/system/CommandBuffer.hpp application/DebugCallback.cpp application/DebugCallback.hpp
        ecs/ComponentStorageTree.hpp ecs/component/ComponentTransformTree.hpp util/Util.hpp ecs/system/GameSystemUpdateTransforms.cpp
        ecs/system/GameSystemUpdateTransforms.hpp ecs/component/ComponentMeshFilter.hpp
        ecs/system/RenderSystemMeshFilter.cpp ecs/system/RenderSystemMeshFilter.hpp ecs/system/GameSystem.hpp
        ecs/system/RenderSystem.hpp application/Scene.cpp application/Scene.hpp rendering/material/MaterialPipeline.cpp rendering/material/MaterialPipeline.hpp
        ecs/system/RenderSystem.cpp rendering/material/MaterialPipelineCache.cpp rendering/material/MaterialPipelineCache.hpp ecs/component/ComponentCamera.cpp
        ecs/component/ComponentCamera.hpp util/Math.hpp ecs/component/ComponentTransformTree.cpp ecs/annotations/AnnotationTree.hpp ecs/component/ComponentModelMatrix.cpp
        ecs/component/ComponentModelMatrix.hpp ecs/ComponentStorageFlatMap.hpp ecs/component/ComponentTransform.cpp ecs/component/ComponentTransform.hpp ecs/ComponentStorageCategory.hpp
        ecs/annotations/AnnotationCategory.hpp ecs/annotations/Annotations.hpp ecs/annotations/AnnotationDependency.hpp util/TypeName.hpp util/TemplateUtil.hpp rendering/material/DescriptorSetLayout.hpp rendering/material/DescriptorSetLayout.cpp rendering/singleton/DeviceManager.hpp rendering/singleton/InstanceManager.hpp rendering/system/Buffer.cpp rendering/system/Buffer.hpp rendering/system/AllocatedData.cpp rendering/system/AllocatedData.hpp rendering/singleton/impl/VulkanMemoryAllocator.hpp rendering/singleton/impl/VulkanMemoryAllocator.cpp rendering/singleton/impl/VulkanSurfaceManager.cpp rendering/singleton/impl/VulkanSurfaceManager.hpp rendering/singleton/SurfaceManager.hpp rendering/singleton/WindowManager.hpp rendering/singleton/CommandPool.hpp rendering/material/TDescriptorPool.hpp rendering/material/TDescriptorPool.cpp rendering/material/DescriptorSet.cpp rendering/material/DescriptorSet.hpp rendering/material/DescriptorBinding.hpp rendering/singleton/DescriptorManager.hpp)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(talon ${SOURCE_FILES})
target_include_directories(talon PRIVATE ${GLBINDING_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR})
target_include_directories(talon PUBLIC ${INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${TALON_CONFIG_DIR} ${EIGEN3_INCLUDE_DIR})
target_link_libraries(talon PUBLIC Vulkan::Vulkan ${GLFW_LIBRARIES} signals vulkan_mem_alloc)

