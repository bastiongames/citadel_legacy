#ifndef __DX12__DEVICE_H__
#define __DX12__DEVICE_H__

#include "watchtower/Device.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace Citadel::Watchtower::DX12 {
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
		ComPtr<IDXGIFactory2> factory;
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;
		
		ComPtr<ID3D12CommandAllocator> allocator;
		ComPtr<ID3D12GraphicsCommandList> commandList;

		Keep::u32 fenceValue;
		ComPtr<ID3D12Fence> fence;
		HANDLE fenceEvent;

		HWND hwnd;
	};
}

#endif