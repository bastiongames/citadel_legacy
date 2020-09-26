#include "GraphicsPipeline.h"
#include "../Shaders/ShaderProgram.h"
#include "PipelineLayout.h"

#include "../RenderPass.h"

namespace Citadel::Watchtower::Pipelines::VLKN {
	GraphicsPipeline::GraphicsPipeline(VkDevice device, const GraphicsPipelineDescriptor& descriptor) {
		VkPipelineShaderStageCreateInfo vshaderStageCreateInfo{};
		{
			vshaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vshaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vshaderStageCreateInfo.module = *(VkShaderModule*)descriptor.shaderProgram->GetBytecode(Shaders::ShaderType::Vertex);
			vshaderStageCreateInfo.pName = "main";
		}

		VkPipelineShaderStageCreateInfo pshaderStageCreateInfo{};
		{
			pshaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pshaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			pshaderStageCreateInfo.module = *(VkShaderModule*)descriptor.shaderProgram->GetBytecode(Shaders::ShaderType::Pixel);
			pshaderStageCreateInfo.pName = "main";
		}
		VkPipelineShaderStageCreateInfo stages[] = { vshaderStageCreateInfo, pshaderStageCreateInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{};
		{
			auto shaderProgram = Keep::CastSPtr<Shaders::ShaderProgram, Shaders::VLKN::ShaderProgram>(descriptor.shaderProgram);
			shaderProgram->PopulateVertexInputCreateInfo(vertexInputCreateInfo);
		}

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
		{
			inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyStateCreateInfo.pNext = nullptr;
			inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssemblyStateCreateInfo.primitiveRestartEnable = false;
		}

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
		{
			rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
			rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
			rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizationStateCreateInfo.lineWidth = 1.0f;
			rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
		}
		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
		{
			multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampleStateCreateInfo.pNext = nullptr;
			multisampleStateCreateInfo.sampleShadingEnable = false;
			multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampleStateCreateInfo.minSampleShading = 1.0f;
			multisampleStateCreateInfo.pSampleMask = nullptr;
			multisampleStateCreateInfo.alphaToCoverageEnable = false;
			multisampleStateCreateInfo.alphaToOneEnable = false;
		}

		VkPipelineColorBlendStateCreateInfo colorBlendingCreateInfo{};
		{
			VkPipelineColorBlendAttachmentState colorblendAttachmentState{};
			colorblendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorblendAttachmentState.blendEnable = false;
			colorblendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			colorblendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorblendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
			colorblendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorblendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorblendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

			colorBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendingCreateInfo.pNext = nullptr;
			colorBlendingCreateInfo.logicOpEnable = false;
			colorBlendingCreateInfo.logicOp = VK_LOGIC_OP_COPY;
			colorBlendingCreateInfo.attachmentCount = 1;
			colorBlendingCreateInfo.pAttachments = &colorblendAttachmentState;
			colorBlendingCreateInfo.blendConstants[0] = 0.0f;
			colorBlendingCreateInfo.blendConstants[1] = 0.0f;
			colorBlendingCreateInfo.blendConstants[2] = 0.0f;
			colorBlendingCreateInfo.blendConstants[3] = 0.0f;

		}

		VkDynamicState dynamicStates[]{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_LINE_WIDTH
		};

		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
		{
			dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCreateInfo.pNext = nullptr;
			dynamicStateCreateInfo.flags = 0;
			dynamicStateCreateInfo.dynamicStateCount = 2;
			dynamicStateCreateInfo.pDynamicStates = dynamicStates;
		}

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
		{
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 720.0f;
			viewport.width = 1280.0f;
			viewport.height = -720.0f;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor{};
			scissor.offset = { 0,0 };
			scissor.extent = { 1280,720 };

			viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportStateCreateInfo.pNext = nullptr;
			viewportStateCreateInfo.viewportCount = 1;
			viewportStateCreateInfo.pViewports = &viewport;
			viewportStateCreateInfo.scissorCount = 1;
			viewportStateCreateInfo.pScissors = &scissor;
			viewportStateCreateInfo.flags = 0;
		}


		auto pipelineLayout = Keep::CastSPtr<Pipelines::PipelineLayout, VLKN::PipelineLayout>(descriptor.pipelineLayout);

		VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
		{
			auto vkRenderPass = Keep::CastSPtr<RenderPass, Watchtower::VLKN::RenderPass>(descriptor.renderPass);

			pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineCreateInfo.pNext = nullptr;
			pipelineCreateInfo.flags = 0;
			pipelineCreateInfo.stageCount = 2;
			pipelineCreateInfo.pStages = stages;
			pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
			pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
			pipelineCreateInfo.pTessellationState = nullptr;
			pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
			pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
			pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
			pipelineCreateInfo.pDepthStencilState = nullptr;
			pipelineCreateInfo.pColorBlendState = &colorBlendingCreateInfo;
			pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
			pipelineCreateInfo.layout = pipelineLayout->GetPipelineLayout();
			pipelineCreateInfo.renderPass = vkRenderPass->GetRenderPass();
			pipelineCreateInfo.subpass = 0;
			pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

			vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);
		}
	}

	void
	GraphicsPipeline::Record(const VkCommandBuffer& commandBuffer) {
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	}
}