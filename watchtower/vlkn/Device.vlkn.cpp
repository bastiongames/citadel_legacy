#include "watchtower/Device.h"
#include "watchtower/VLKN/DeviceData.h"

#include "watchtower/vlkn/Shader.h"
#include "watchtower/vlkn/PipelineData.h"
#include "watchtower/vlkn/ContextData.h"
#include "watchtower/vlkn/RenderTargetData.h"
#include "watchtower/vlkn/VertexBufferData.h"
#include "watchtower/vlkn/TextureData.h"

#include "keep/base64.h"

#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#pragma comment(lib, "vulkan-1.lib")

using Citadel::Watchtower::Vlkn::DeviceData;
using Citadel::Watchtower::Device;

using Citadel::Watchtower::Vlkn::ShaderData;
using Citadel::Watchtower::Vlkn::VertexShaderData;
using Citadel::Watchtower::VertexShader;
using Citadel::Watchtower::PixelShader;

using Citadel::Watchtower::Vlkn::ContextData;
using Citadel::Watchtower::Context2D;

using Citadel::Watchtower::Vlkn::RenderTargetData;
using Citadel::Watchtower::RenderTarget;

using Citadel::Watchtower::Vlkn::PipelineData;
using Citadel::Watchtower::Pipeline;

using Citadel::Watchtower::Vlkn::VertexBufferData;
using Citadel::Watchtower::VertexBuffer;

using Citadel::Watchtower::Vlkn::TextureData;
using Citadel::Watchtower::Texture;

using Citadel::Keep::SPtr;
using Citadel::Keep::MakeSPtr;
using Citadel::Keep::HandleFromSPtr;
using Citadel::Keep::SPtrFromHandle;

using Citadel::Keep::Encoding::Base64;

#include <vector>
#include <array>
#include <set>
#include <optional>

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


std::vector<const char*>
getRequiredExtensions(SPtr<DeviceData> deviceData) {
	uint32_t extensionCount = 0;
	
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensionProperties(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());

	std::vector<const char*> extensions;
	extensions.push_back("VK_KHR_surface");
	extensions.push_back("VK_KHR_win32_surface");
	extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	return extensions;
}

void
findQueueFamilies(SPtr<DeviceData> deviceData, std::optional<uint32_t>& graphics, std::optional<uint32_t>& present) {
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(deviceData->physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(deviceData->physicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphics = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(deviceData->physicalDevice, i, deviceData->surface, &presentSupport);

		if (presentSupport) {
			present = i;
		}
		if (graphics.has_value() && present.has_value()) {
			break;
		}
		++i;
	}
}

void
createInstance(SPtr<DeviceData> deviceData) {
	// Annoying "app" info
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Citadel";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "Citadel";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	auto extensions = getRequiredExtensions(deviceData);
	std::vector<const char*> layers = {
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_KHRONOS_validation"
	};


	VkDebugReportCallbackCreateInfoEXT debugReportInfo{};
	debugReportInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugReportInfo.pfnCallback = dbgFunc;
	debugReportInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = (u32)extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = layers.size();
	createInfo.ppEnabledLayerNames = layers.data();
	createInfo.pNext = &debugReportInfo;

	VkResult res = vkCreateInstance(&createInfo, nullptr, &deviceData->instance);
	
	auto reportFunc = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(deviceData->instance, "vkCreateDebugReportCallbackEXT");
	if (reportFunc != nullptr) {
		res = reportFunc(deviceData->instance, &debugReportInfo, nullptr, &deviceData->reportCallback);
	}
}

void
createSurface(SPtr<DeviceData> deviceData, HWND hwnd) {
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hwnd = hwnd;
	surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
	vkCreateWin32SurfaceKHR(deviceData->instance, &surfaceCreateInfo, nullptr, &deviceData->surface);
}

void
pickPhysicalDevice(SPtr<DeviceData> deviceData) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(deviceData->instance, &deviceCount, nullptr);
	
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(deviceData->instance, &deviceCount, devices.data());
	for (const auto& device : devices) {
		deviceData->physicalDevice = device;
		break;
	}
}

void
createLogicalDevice(SPtr<DeviceData> deviceData) {
	std::optional<uint32_t> graphicsFamilyIndex;
	std::optional<uint32_t> presentFamilyIndex;
	findQueueFamilies(deviceData, graphicsFamilyIndex, presentFamilyIndex);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { graphicsFamilyIndex.value(), presentFamilyIndex.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		createInfo.queueFamilyIndex = queueFamily;
		createInfo.queueCount = 1;
		createInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(createInfo);
	}

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	VkPhysicalDeviceFeatures deviceFeatures{};
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = queueCreateInfos.size();
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.enabledLayerCount = validationLayers.size();
	createInfo.ppEnabledLayerNames = validationLayers.data();

	vkCreateDevice(deviceData->physicalDevice, &createInfo, nullptr, &deviceData->device);
	vkGetDeviceQueue(deviceData->device, graphicsFamilyIndex.value(), 0, &deviceData->graphicsQueue);
	vkGetDeviceQueue(deviceData->device, presentFamilyIndex.value(), 0, &deviceData->presentQueue);
}

void
createSwapchain(SPtr<DeviceData> deviceData) {
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(deviceData->physicalDevice, deviceData->surface, &capabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(deviceData->physicalDevice, deviceData->surface, &formatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(deviceData->physicalDevice, deviceData->surface, &formatCount, surfaceFormats.data());

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(deviceData->physicalDevice, deviceData->surface, &presentModeCount, nullptr);
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(deviceData->physicalDevice, deviceData->surface, &presentModeCount, presentModes.data());

	VkSurfaceFormatKHR surfaceFormat = surfaceFormats[0];
	for (const auto& availableFormat : surfaceFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surfaceFormat = availableFormat;
			break;
		}
	}

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto& availableMode : presentModes) {
		if (availableMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = availableMode;
			break;
		}
	}

	VkExtent2D extent = { 1280, 720 };

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = deviceData->surface;
	createInfo.minImageCount = 2;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;
	vkCreateSwapchainKHR(deviceData->device, &createInfo, nullptr, &deviceData->swapchain);

	uint32_t imageCount = 0;
	vkGetSwapchainImagesKHR(deviceData->device, deviceData->swapchain, &imageCount, nullptr);
	deviceData->swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(deviceData->device, deviceData->swapchain, &imageCount, deviceData->swapchainImages.data());
	deviceData->swapchainImageFormat = surfaceFormat.format;
	deviceData->swapchainExtent = extent;

	//deviceData->swapchainImageViews.resize(imageCount);
	for (const auto& image : deviceData->swapchainImages) {
		deviceData->swapchainImageViews.push_back(
			createImageView(deviceData, image, deviceData->swapchainImageFormat)
		);
	}
}

VkRenderPass
createRenderPass(SPtr<DeviceData> deviceData) {
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = deviceData->swapchainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = 1;
	createInfo.pAttachments = &colorAttachment;
	createInfo.subpassCount = 1;
	createInfo.pSubpasses = &subpass;
	createInfo.dependencyCount = 1;
	createInfo.pDependencies = &dependency;
	
	VkRenderPass renderPass{};
	VkResult res = vkCreateRenderPass(deviceData->device, &createInfo, nullptr, &renderPass);
	return renderPass;
}

VkPipelineLayout
createGraphicsPipelineLayout(SPtr<DeviceData> deviceData, VkDescriptorSetLayout descriptorLayout) {
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	VkPipelineLayout layout{};
	vkCreatePipelineLayout(deviceData->device, &pipelineLayoutCreateInfo, nullptr, &layout);
	return layout;
}

void
createGraphicsPipeline(SPtr<DeviceData> deviceData, SPtr<VertexShaderData> vsData, SPtr<ShaderData> psData,SPtr<PipelineData> pipelineData) {
	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{};
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputCreateInfo.pVertexBindingDescriptions = &vsData->bindings;
	vertexInputCreateInfo.vertexAttributeDescriptionCount = vsData->attributes.size();
	vertexInputCreateInfo.pVertexAttributeDescriptions = vsData->attributes.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{};
	inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0;
	viewport.y = deviceData->swapchainExtent.height;
	viewport.width = deviceData->swapchainExtent.width;
	viewport.height = -(float)deviceData->swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = deviceData->swapchainExtent;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
	rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationStateCreateInfo.lineWidth = 1;
	rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizationStateCreateInfo.depthBiasConstantFactor = 0;
	rasterizationStateCreateInfo.depthBiasClamp = 0;
	rasterizationStateCreateInfo.depthBiasSlopeFactor = 0;

	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
	multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;


	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlend{};
	colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlend.logicOpEnable = VK_FALSE;
	colorBlend.logicOp = VK_LOGIC_OP_COPY;
	colorBlend.attachmentCount = 1;
	colorBlend.pAttachments = &colorBlendAttachment;
	colorBlend.blendConstants[0] = 0.0f;
	colorBlend.blendConstants[1] = 0.0f;
	colorBlend.blendConstants[2] = 0.0f;
	colorBlend.blendConstants[3] = 0.0f;

	VkPipelineShaderStageCreateInfo vsStageCreateInfo{};
	vsStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vsStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vsStageCreateInfo.module = vsData->module;
	vsStageCreateInfo.pName = "main"; // put the "entrypoint" in the data?

	VkPipelineShaderStageCreateInfo psStageCreateInfo{};
	psStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	psStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	psStageCreateInfo.module = psData->module;
	psStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {
		vsStageCreateInfo,
		psStageCreateInfo
	};



	VkGraphicsPipelineCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	
	createInfo.stageCount = _countof(shaderStages);
	createInfo.pStages = shaderStages;
	createInfo.pVertexInputState = &vertexInputCreateInfo;
	createInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
	createInfo.pViewportState = &viewportStateCreateInfo;
	createInfo.pRasterizationState = &rasterizationStateCreateInfo;
	createInfo.pMultisampleState = &multisampleStateCreateInfo;
	createInfo.pDepthStencilState = nullptr;
	createInfo.pColorBlendState = &colorBlend;
	createInfo.pDynamicState = nullptr;
	createInfo.layout = pipelineData->layout;
	createInfo.renderPass = pipelineData->renderPass;
	createInfo.subpass = 0;
	createInfo.basePipelineHandle = VK_NULL_HANDLE;
	
	try {
		vkCreateGraphicsPipelines(deviceData->device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipelineData->graphicsPipeline);
	}
	catch(...) {
		std::cout << "FAIL";
	}
}

u32
findMemoryType(SPtr<DeviceData> deviceData, u32 filter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProps{};
	vkGetPhysicalDeviceMemoryProperties(deviceData->physicalDevice, &memProps);
	for (u32 i = 0; i < memProps.memoryTypeCount; ++i) {
		if ((filter & (1 << i)) && (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	return 0;
}

VkCommandBuffer
beginOneOffCommand(SPtr<DeviceData> deviceData) {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = deviceData->commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer buffer;
	vkAllocateCommandBuffers(deviceData->device, &allocInfo, &buffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(buffer, &beginInfo);
	return buffer;
}

void
endOneOffCommand(SPtr<DeviceData> deviceData, VkCommandBuffer buffer) {
	vkEndCommandBuffer(buffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &buffer;

	vkQueueSubmit(deviceData->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(deviceData->graphicsQueue);

	vkFreeCommandBuffers(deviceData->device, deviceData->commandPool, 1, &buffer);
}

void
createBuffer(SPtr<DeviceData> deviceData, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& memory) {
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateBuffer(deviceData->device, &bufferInfo, nullptr, &buffer);

	VkMemoryRequirements memReqs{};
	vkGetBufferMemoryRequirements(deviceData->device, buffer, &memReqs);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memReqs.size;
	allocInfo.memoryTypeIndex = findMemoryType(deviceData, memReqs.memoryTypeBits, properties);

	vkAllocateMemory(deviceData->device, &allocInfo, nullptr, &memory);
	vkBindBufferMemory(deviceData->device, buffer, memory, 0);
}

void
copyBuffer(SPtr<DeviceData> deviceData, VkBuffer src, VkBuffer dst, VkDeviceSize size) {
	VkCommandBuffer commandBuffer = beginOneOffCommand(deviceData);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

	endOneOffCommand(deviceData, commandBuffer);
}

void
copyBufferToImage(SPtr<DeviceData> deviceData, VkBuffer buffer, VkImage image, u32 width, u32 height) {
	VkCommandBuffer commandBuffer = beginOneOffCommand(deviceData);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	endOneOffCommand(deviceData, commandBuffer);
}

void
transitionImageLayout(SPtr<DeviceData> deviceData, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
	VkCommandBuffer commandBuffer = beginOneOffCommand(deviceData);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = 0;
	
	VkPipelineStageFlagBits sourceStage{};
	VkPipelineStageFlags destinationStage{};

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);


	endOneOffCommand(deviceData, commandBuffer);
}

VkImageView
createImageView(SPtr<DeviceData> deviceData, VkImage image, VkFormat format) {
	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;
	createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	vkCreateImageView(deviceData->device, &createInfo, nullptr, &imageView);
	return imageView;
}

handle
Device::AcquireDevice(handle platformData) {
	auto deviceData = MakeSPtr<DeviceData>();
	HWND hwnd = *(SPtrFromHandle<HWND>(platformData).get());

	createInstance(deviceData);
	createSurface(deviceData, hwnd);
	pickPhysicalDevice(deviceData);
	createLogicalDevice(deviceData);
	createSwapchain(deviceData);

	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> _;
	findQueueFamilies(deviceData, graphicsFamily, _);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = graphicsFamily.value();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	vkCreateCommandPool(deviceData->device, &poolInfo, nullptr, &deviceData->commandPool);

	VkSemaphoreCreateInfo semaphoreCreateInfo{};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	deviceData->imageAvailableSemaphores.resize(2);
	deviceData->renderFinishedSemaphores.resize(2);

	for (size_t i = 0; i < 2; ++i) {
		vkCreateSemaphore(deviceData->device, &semaphoreCreateInfo, nullptr, &deviceData->imageAvailableSemaphores[i]);
		vkCreateSemaphore(deviceData->device, &semaphoreCreateInfo, nullptr, &deviceData->renderFinishedSemaphores[i]);
	}

	return HandleFromSPtr<DeviceData>(deviceData);
}

void
Device::ReleaseDevice() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	if (deviceData != nullptr) {
		for (int i = 0; i < 2; ++i) {
			vkDestroySemaphore(deviceData->device, deviceData->renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(deviceData->device, deviceData->imageAvailableSemaphores[i], nullptr);
		}

		for (auto imageView : deviceData->swapchainImageViews) {
			vkDestroyImageView(deviceData->device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(deviceData->device, deviceData->swapchain, nullptr);

		auto destroyDebugUtils = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(deviceData->instance, "vkDestroyDebugUtilsMessengerEXT");
		if (destroyDebugUtils != nullptr) {
			destroyDebugUtils(deviceData->instance, deviceData->debugMessenger, nullptr);
		}

		auto destroyDebugReport = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(deviceData->instance, "vkDestroyDebugReportCallbackEXT");
		if (destroyDebugReport != nullptr) {
			destroyDebugReport(deviceData->instance, deviceData->reportCallback, nullptr);
		}

		vkDestroyCommandPool(deviceData->device, deviceData->commandPool, nullptr);

		vkDestroyDevice(deviceData->device, nullptr);
		vkDestroySurfaceKHR(deviceData->instance, deviceData->surface, nullptr);
		vkDestroyInstance(deviceData->instance, nullptr);
	}
}

Context2D
Device::CreateContext2D() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto contextData = MakeSPtr<ContextData>();

	contextData->commandBuffers.resize(deviceData->swapchainImageViews.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = deviceData->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)contextData->commandBuffers.size();

	vkAllocateCommandBuffers(deviceData->device, &allocInfo, contextData->commandBuffers.data());

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	contextData->consumedFences.resize(deviceData->swapchainImageViews.size());
	for (int i = 0; i < contextData->commandBuffers.size(); ++i) {
		vkCreateFence(deviceData->device, &fenceInfo, nullptr, &contextData->consumedFences[i]);
	}

	contextData->device = deviceData->device;
	return Context2D(HandleFromSPtr<ContextData>(contextData));
}

VertexShader
Device::CreateVertexShader(std::string src, std::string entrypoint, std::string model) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto vertexShaderData = MakeSPtr<VertexShaderData>();

	std::vector<u8> code = Base64::Decode(src);
	
	u32 size = code.size();
	auto t = (u32*)code.data();

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkResult res = vkCreateShaderModule(deviceData->device, &createInfo, nullptr, &vertexShaderData->module);

	vertexShaderData->bindings.binding = 0;
	vertexShaderData->bindings.stride = 20;
	vertexShaderData->bindings.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription attr1{};
	attr1.binding = 0;
	attr1.location = 0;
	attr1.offset = 0;
	attr1.format = VK_FORMAT_R32G32_SFLOAT;
	vertexShaderData->attributes.push_back(attr1);

	VkVertexInputAttributeDescription attr2{};
	attr2.binding = 0;
	attr2.location = 1;
	attr2.offset = 8;
	attr2.format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexShaderData->attributes.push_back(attr2);

	VkVertexInputAttributeDescription attr3{};
	attr3.binding = 0;
	attr3.location = 2;
	attr3.offset = 20;
	attr3.format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexShaderData->attributes.push_back(attr3);

	vertexShaderData->device = deviceData->device;

	return VertexShader(HandleFromSPtr<ShaderData>(vertexShaderData));
}

PixelShader
Device::CreatePixelShader(std::string src, std::string entrypoint, std::string model) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto shaderData = MakeSPtr<ShaderData>();

	std::vector<u8> code = Base64::Decode(src);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkResult res = vkCreateShaderModule(deviceData->device, &createInfo, nullptr, &shaderData->module);
	shaderData->device = deviceData->device;

	return PixelShader(HandleFromSPtr<ShaderData>(shaderData));
}

Pipeline
Device::CreatePipeline(VertexShader& vs, PixelShader& ps) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto vsData = SPtrFromHandle<VertexShaderData>(vs.Get());
	auto psData = SPtrFromHandle<ShaderData>(ps.Get());
	auto pipelineData = MakeSPtr<PipelineData>();

	std::vector<VkDescriptorSetLayoutBinding> bindings;
	VkDescriptorSetLayoutBinding samplerBinding{};
	samplerBinding.binding = 0;
	samplerBinding.descriptorCount = 1;
	samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerBinding.pImmutableSamplers = nullptr;
	samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings.push_back(samplerBinding);

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = (u32)bindings.size();
	layoutInfo.pBindings = bindings.data();
	vkCreateDescriptorSetLayout(deviceData->device, &layoutInfo, nullptr, &pipelineData->descriptorSetLayout);

	pipelineData->device = deviceData->device;
	pipelineData->renderPass = createRenderPass(deviceData);
	pipelineData->layout = createGraphicsPipelineLayout(deviceData, pipelineData->descriptorSetLayout);
	createGraphicsPipeline(deviceData, vsData, psData, pipelineData);

	pipelineData->framebuffers.resize(deviceData->swapchainImageViews.size());
	for (size_t i = 0; i < deviceData->swapchainImageViews.size(); ++i) {
		VkImageView attachments[] = {
			deviceData->swapchainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = pipelineData->renderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = attachments;
		framebufferCreateInfo.width = deviceData->swapchainExtent.width;
		framebufferCreateInfo.height = deviceData->swapchainExtent.height;
		framebufferCreateInfo.layers = 1;

		vkCreateFramebuffer(deviceData->device, &framebufferCreateInfo, nullptr, &pipelineData->framebuffers[i]);
	}


	return Pipeline(HandleFromSPtr<PipelineData>(pipelineData));
}

VertexBuffer
Device::CreateVertexBuffer(void* data, u32 size, u32 stride) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto bufferData = MakeSPtr<VertexBufferData>();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;
	createBuffer(deviceData, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingMemory);

	void* tData;
	vkMapMemory(deviceData->device, stagingMemory, 0, size, 0, &tData);
	memcpy(tData, data, size);
	vkUnmapMemory(deviceData->device, stagingMemory);

	createBuffer(deviceData, size, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferData->buffer, bufferData->bufferMemory);

	copyBuffer(deviceData, stagingBuffer, bufferData->buffer, size);

	vkDestroyBuffer(deviceData->device, stagingBuffer, nullptr);
	vkFreeMemory(deviceData->device, stagingMemory, nullptr);

	bufferData->device = deviceData->device;

	return VertexBuffer(HandleFromSPtr<VertexBufferData>(bufferData));
}

Texture
Device::CreateTexture(handle data, u32 width, u32 height, u32 pixelSize) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto textureData = MakeSPtr<TextureData>();

	u32 imageSize = width * height * pixelSize;

	VkBuffer stageBuffer;
	VkDeviceMemory stageMemory;
	createBuffer(deviceData, imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stageBuffer,
		stageMemory
	);

	void* tData;
	vkMapMemory(deviceData->device, stageMemory, 0, imageSize, 0, &tData);
	memcpy(tData, data.get(), imageSize);
	vkUnmapMemory(deviceData->device, stageMemory);

	VkImageCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.imageType = VK_IMAGE_TYPE_2D;
	createInfo.extent.width = width;
	createInfo.extent.height = height;
	createInfo.extent.depth = 1;
	createInfo.mipLevels = 1;
	createInfo.arrayLayers = 1;
	createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	createInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	createInfo.flags = 0;

	vkCreateImage(deviceData->device, &createInfo, nullptr, &textureData->image);

	VkMemoryRequirements memReqs{};
	vkGetImageMemoryRequirements(deviceData->device, textureData->image, &memReqs);
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memReqs.size;
	allocInfo.memoryTypeIndex = findMemoryType(deviceData, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vkAllocateMemory(deviceData->device, &allocInfo, nullptr, &textureData->memory);
	vkBindImageMemory(deviceData->device, textureData->image, textureData->memory, 0);

	transitionImageLayout(deviceData, textureData->image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(deviceData, stageBuffer, textureData->image, width, height);
	transitionImageLayout(deviceData, textureData->image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	textureData->device = deviceData->device;

	vkDestroyBuffer(deviceData->device, stageBuffer, nullptr);
	vkFreeMemory(deviceData->device, stageMemory, nullptr);

	textureData->view = createImageView(deviceData, textureData->image, VK_FORMAT_R8G8B8A8_SRGB);

	return Texture(HandleFromSPtr<TextureData>(textureData));
}

RenderTarget
Device::CreateRenderTarget() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto renderTargetData = MakeSPtr<RenderTargetData>();

	return RenderTarget(HandleFromSPtr<RenderTargetData>(renderTargetData));
}

Device&
Device::BeginFrame() {
	return *this;
}

void
Device::Present(RenderTarget& target) {

}

void
Device::Present(Context& context) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto contextData = SPtrFromHandle<ContextData>(context.Get());
	VkResult result{};

	uint32_t imageIndex;
	result = vkAcquireNextImageKHR(deviceData->device, deviceData->swapchain, UINT64_MAX, deviceData->imageAvailableSemaphores[deviceData->frameIndex], VK_NULL_HANDLE, &imageIndex);
	
	VkSemaphore waitSemaphores[] = { deviceData->imageAvailableSemaphores[deviceData->frameIndex] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { 
		deviceData->renderFinishedSemaphores[deviceData->frameIndex]
	};

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = _countof(waitSemaphores);
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &contextData->commandBuffers[imageIndex];
	submitInfo.signalSemaphoreCount = _countof(signalSemaphores);
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(deviceData->device, 1, &contextData->consumedFences[imageIndex]);
	result = vkQueueSubmit(deviceData->graphicsQueue, 1, &submitInfo, contextData->consumedFences[imageIndex]);

	VkSwapchainKHR swapchains[] = { deviceData->swapchain };

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.swapchainCount = _countof(swapchains);
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.waitSemaphoreCount = _countof(signalSemaphores);
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.pResults = nullptr;
	result = vkQueuePresentKHR(deviceData->presentQueue, &presentInfo);
}

u32
Device::FinishFrame() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	deviceData->frameIndex = (deviceData->frameIndex + 1) % 2;

//	deviceData->frameIndex = (deviceData->frameIndex + 1) % 2;

	return deviceData->frameIndex;
}

void
Device::Wait() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	vkDeviceWaitIdle(deviceData->device);
}