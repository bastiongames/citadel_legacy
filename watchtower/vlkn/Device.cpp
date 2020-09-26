#include "Device.h"
#include "Swapchain.h"
#include "CommandQueue.h"
#include "CommandBuffer.h"
#include "RenderTarget.h"
#include "RenderPass.h"
#include "Buffers/Buffer.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderProgram.h"
#include "Pipelines/PipelineLayout.h"
#include "Pipelines/GraphicsPipeline.h"
#include "Textures/Texture.h"
#include "ResourceHeap.h"

#include <vulkan/vulkan_win32.h>
#pragma comment(lib, "vulkan-1.lib")

#include <vector>
#include <set>

#include <iostream>
#include <sstream>
VKAPI_ATTR VkBool32 VKAPI_CALL
dbgFunc(VkDebugReportFlagsEXT msgFlags, VkDebugReportObjectTypeEXT objType, uint64_t srcObject,
	size_t location, int32_t msgCode, const char* pLayerPrefix, const char* pMsg,
	void* pUserData) {
	std::ostringstream message;

	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		message << "ERROR: ";
	}
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		message << "WARNING: ";
	}
	else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		message << "PERFORMANCE WARNING: ";
	}
	else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		message << "INFO: ";
	}
	else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		message << "DEBUG: ";
	}
	message << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;

	std::cout << message.str() << std::endl;

	/*
	 * false indicates that layer should not bail-out of an
	 * API call that had validation failures. This may mean that the
	 * app dies inside the driver due to invalid parameter(s).
	 * That's what would happen without validation layers, so we'll
	 * keep that behavior here.
	 */
	return false;
}


Citadel::Watchtower::PDevice 
Citadel::Watchtower::Device::CreateDevice(HWND hwnd) {
	return Keep::MakeSPtr<VLKN::Device>(hwnd);
}


namespace Citadel::Watchtower::VLKN {
	using Keep::u32;
	using Keep::MakeSPtr;

	Device::Device(HWND hwnd) {
		this->hwnd = hwnd;
		std::vector<const char*> extensions;
		std::vector<const char*> layers;
		{
			extensions.push_back("VK_KHR_surface");
			extensions.push_back("VK_KHR_win32_surface");
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

			//layers.push_back("VK_LAYER_LUNARG_standard_validation");
			layers.push_back("VK_LAYER_KHRONOS_validation");
		}
	
		// Create Instance
		{
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Citadel Sandbox";
			appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
			appInfo.pEngineName = "Citadel";
			appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_2;

			VkDebugReportCallbackCreateInfoEXT debugReportInfo{};
			debugReportInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			debugReportInfo.pfnCallback = dbgFunc;
			debugReportInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = (u32)extensions.size();
			createInfo.ppEnabledExtensionNames = extensions.data();
			createInfo.enabledLayerCount = (u32)layers.size();
			createInfo.ppEnabledLayerNames = layers.data();
			createInfo.pNext = &debugReportInfo;

			VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);

			auto reportFunc = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
			if (reportFunc != nullptr) {
				res = reportFunc(instance, &debugReportInfo, nullptr, &reportCallback);
			}
		}

		// Create Surface
		{
			VkWin32SurfaceCreateInfoKHR createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			createInfo.hwnd = hwnd;
			createInfo.hinstance = GetModuleHandle(nullptr);
			createInfo.pNext = nullptr;
			createInfo.flags = 0;

			vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);
		}

		// Pick Physical Device
		{
			u32 deviceCount = 0;
			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
			for (const auto& device : devices) {
				physicalDevice = device;
				break;
			}
		}

		// Find Queue Families
		{
			u32 count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
			std::vector<VkQueueFamilyProperties> families(count);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, families.data());

			int i = 0;
			for (const auto& family : families) {
				if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) queueFamily.graphicsFamily = i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
				if (presentSupport) queueFamily.presentFamily = i;
				if (queueFamily.IsComplete()) break;
				++i;
			}
		}
	
		// Create Logical Device
		{
			const std::vector<const char*> deviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			const std::vector<const char*> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};

			float priority = 1.0f;
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

			// One per unique queue family
			std::set<u32> uniqueQueueFamilies = { queueFamily.graphicsFamily, queueFamily.presentFamily };
			for (auto family : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				createInfo.flags = 0;
				createInfo.queueFamilyIndex = family;
				createInfo.queueCount = 1;
				createInfo.pQueuePriorities = &priority;
				queueCreateInfos.push_back(createInfo);
			}

			VkPhysicalDeviceFeatures features{};
			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.queueCreateInfoCount = (u32)queueCreateInfos.size();
			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.enabledLayerCount = (u32)validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();
			createInfo.enabledExtensionCount = (u32)deviceExtensions.size();
			createInfo.ppEnabledExtensionNames = deviceExtensions.data();
			createInfo.pEnabledFeatures = &features;

			auto res = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
		}
	
		// Create Command Queue
		{
			VkCommandPoolCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.queueFamilyIndex = queueFamily.graphicsFamily;

			vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
			vkGetDeviceQueue(device, queueFamily.graphicsFamily, 0, &commandQueue);
			vkGetDeviceQueue(device, queueFamily.presentFamily, 0, &presentQueue);
		}
	}

	PSwapchain
	Device::CreateSwapchain(const SwapchainDescriptor& descriptor) {

		SwapchainCreateInfo createInfo{};
		createInfo.descriptor = descriptor;
		createInfo.device = device;
		createInfo.instance = instance;
		createInfo.physicalDevice = physicalDevice;
		createInfo.presentQueue = presentQueue;
		createInfo.surface = surface;
		createInfo.presentFamily = queueFamily.presentFamily;
	
		return Keep::MakeSPtr<Swapchain>(createInfo);
	}

	PCommandQueue
	Device::CreateCommandQueue(const CommandQueueDescriptor& descriptor) {
		CommandQueueCreateInfo createInfo;
		createInfo.device = device;
		createInfo.queue = commandQueue;
		return Keep::MakeSPtr<CommandQueue>(createInfo);
	}

	PCommandBuffer
	Device::CreateCommandBuffer(const CommandBufferDescriptor& descriptor) {
		CommandBufferCreateInfo createInfo;
		createInfo.device = device;
		createInfo.queue = commandQueue;
		createInfo.queueFamily = queueFamily.graphicsFamily;

		return Keep::MakeSPtr<CommandBuffer>(createInfo);
	}

	PRenderPass
	Device::CreateRenderPass(const PRenderTarget renderTarget) {
		return Keep::MakeSPtr<VLKN::RenderPass>(device, renderTarget);
	}

	Buffers::PBuffer
	Device::CreateBuffer(const Buffers::BufferDescriptor& descriptor, void* data) {
		VkCommandBuffer commandBuffer = BeginStagingCommand();
		Buffers::VLKN::DevicePair devices{ device, physicalDevice };
		auto buffer = MakeSPtr<Buffers::VLKN::Buffer>(devices, commandBuffer, descriptor, data);
		EndStagingCommand(commandBuffer);

		return buffer;
	}

	Shaders::PShader
	Device::CreateShader(const Shaders::ShaderDescriptor& descriptor) {
		return MakeSPtr<Shaders::VLKN::Shader>(device, descriptor);
	}

	Shaders::PShaderProgram
	Device::CreateShaderProgram(const Shaders::ShaderProgramDescriptor& descriptor) {
		return MakeSPtr<Shaders::VLKN::ShaderProgram>(descriptor);
	}

	Pipelines::PPipelineLayout
	Device::CreatePipelineLayout(const Pipelines::PipelineLayoutDescriptor& descriptor) {
		return Keep::MakeSPtr<Pipelines::VLKN::PipelineLayout>(device, descriptor);
	}

	Pipelines::PGraphicsPipeline
	Device::CreateGraphicsPipeline(const Pipelines::GraphicsPipelineDescriptor& descriptor) {
		return Keep::MakeSPtr<Pipelines::VLKN::GraphicsPipeline>(device, descriptor);
	}

	Textures::PTexture
	Device::CreateTexture(const Textures::TextureDescriptor& descriptor) {
		VkCommandBuffer commandBuffer = BeginStagingCommand();
		Textures::VLKN::DevicePair devices{ device, physicalDevice };
		auto texture = MakeSPtr<Textures::VLKN::Texture>(devices, commandBuffer, descriptor);
		EndStagingCommand(commandBuffer);
		return texture;
	}

	PResourceHeap
	Device::CreateResourceHeap(const ResourceHeapDescriptor& descriptor) {
		return Keep::MakeSPtr<VLKN::ResourceHeap>(device, descriptor);
	}

	VkCommandBuffer
	Device::BeginStagingCommand() {
		VkCommandBuffer commandBuffer;
		VkCommandBufferAllocateInfo allocInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				nullptr,
				commandPool,
				VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				1
		};
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
		};
		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		return commandBuffer;
	}

	void
	Device::EndStagingCommand(VkCommandBuffer commandBuffer) {
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(commandQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(commandQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}
}