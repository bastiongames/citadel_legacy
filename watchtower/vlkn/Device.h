#ifndef __VLKN__DEVICE_H__
#define __VLKN__DEVICE_H__

#include "watchtower/Device.h"

#include <vulkan/vulkan.h>
#include <Windows.h>

namespace Citadel::Watchtower::VLKN {
	struct QueueFamily {
		QueueFamily() {
			graphicsFamily = ~0;
			presentFamily = ~0;
		}

		union {
			Keep::u32 indices[2];
			struct {
				Keep::u32 graphicsFamily;
				Keep::u32 presentFamily;
			};
		};

		inline bool IsComplete() {
			return (graphicsFamily != ~0 && presentFamily != ~0);
		}
	};

	class Device : public Citadel::Watchtower::Device {
	public:
		Device(HWND hwnd);

		virtual PSwapchain CreateSwapchain(const SwapchainDescriptor& descriptor);
		virtual PCommandQueue CreateCommandQueue(const CommandQueueDescriptor& descriptor);
		virtual PCommandBuffer CreateCommandBuffer(const CommandBufferDescriptor& descriptor);
		virtual PRenderPass CreateRenderPass(const PRenderTarget renderTarget);
		virtual Buffers::PBuffer CreateBuffer(const Buffers::BufferDescriptor& descriptor, void* data);
		virtual Shaders::PShader CreateShader(const Shaders::ShaderDescriptor& descriptor);
		virtual Shaders::PShaderProgram CreateShaderProgram(const Shaders::ShaderProgramDescriptor& descriptor);
		virtual Pipelines::PPipelineLayout CreatePipelineLayout(const Pipelines::PipelineLayoutDescriptor& descriptor);
		virtual Pipelines::PGraphicsPipeline CreateGraphicsPipeline(const Pipelines::GraphicsPipelineDescriptor& descriptor);
		virtual Textures::PTexture CreateTexture(const Textures::TextureDescriptor& descriptor);
		virtual PResourceHeap CreateResourceHeap(const ResourceHeapDescriptor& descriptor);

	private:
		VkCommandBuffer BeginStagingCommand();
		void EndStagingCommand(VkCommandBuffer commandBuffer);

	private:
		VkInstance instance;
		VkSurfaceKHR surface;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkCommandPool commandPool;
		VkQueue commandQueue;
		VkQueue presentQueue;
		VkDebugReportCallbackEXT reportCallback;

		// Acts as a staging buffer
		// VkCommandBuffer commandBuffer;

		QueueFamily queueFamily;

		HWND hwnd;

	};
}

#endif