// MIT License
//
// Copyright( c ) 2017 Packt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Vulkan Cookbook
// ISBN: 9781786468154
// � Packt Publishing Limited
//
// Author:   Pawel Lapinski
// LinkedIn: https://www.linkedin.com/in/pawel-lapinski-84522329
//
// 05-Using_Combined_Image_Samplers

#include "CookbookSampleFramework.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSample {
  VkDestroyer(VkBuffer)               VertexBuffer;
  VkDestroyer(VkDeviceMemory)         BufferMemory;

  VkDestroyer(VkRenderPass)           RenderPass;
  VkDestroyer(VkPipeline)             GraphicsPipeline;

  virtual bool Initialize( WindowParameters window_parameters ) override {
    if( !InitializeVulkan( window_parameters, nullptr, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, false ) ) {
      return false;
    }

    // Combined image sampler
    int width = 1;
    int height = 1;
    std::vector<unsigned char> image_data;
    if( !LoadTextureDataFromFile( "Data/Textures/sunset.jpg", 4, image_data, &width, &height ) ) {
      return false;
    }

    // Render pass
    std::vector<VkAttachmentDescription> attachment_descriptions = {
      {
        0,                                // VkAttachmentDescriptionFlags     flags
        Swapchain.Format,                 // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,      // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_STORE,     // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE, // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR   // VkImageLayout                    finalLayout
      }
    };

    std::vector<SubpassParameters> subpass_parameters = {
      {
        VK_PIPELINE_BIND_POINT_GRAPHICS,            // VkPipelineBindPoint                  PipelineType
        {},                                         // std::vector<VkAttachmentReference>   InputAttachments
        {
          {                                         // std::vector<VkAttachmentReference>   ColorAttachments
            0,                                        // uint32_t                             attachment
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout                        layout
          }
        },
        {},                                         // std::vector<VkAttachmentReference>   ResolveAttachments
        nullptr,                                    // VkAttachmentReference const        * DepthStencilAttachment
        {}                                          // std::vector<uint32_t>                PreserveAttachments
      }
    };

    std::vector<VkSubpassDependency> subpass_dependencies = {
      {
        VK_SUBPASS_EXTERNAL,                            // uint32_t                   srcSubpass
        0,                                              // uint32_t                   dstSubpass
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,              // VkPipelineStageFlags       srcStageMask
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // VkPipelineStageFlags       dstStageMask
        VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags              srcAccessMask
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           // VkAccessFlags              dstAccessMask
        VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
      },
      {
        0,                                              // uint32_t                   srcSubpass
        VK_SUBPASS_EXTERNAL,                            // uint32_t                   dstSubpass
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // VkPipelineStageFlags       srcStageMask
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,              // VkPipelineStageFlags       dstStageMask
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           // VkAccessFlags              srcAccessMask
        VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags              dstAccessMask
        VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
      }
    };

    InitVkDestroyer( LogicalDevice, RenderPass );
    if( !CreateRenderPass( *LogicalDevice, attachment_descriptions, subpass_parameters, subpass_dependencies, *RenderPass ) ) {
      return false;
    }

    if( !CreatePipelineObject( true ) ) {
      return false;
    }

    // Vertex data
    std::vector<float> vertices = {
      // positions          // color
      -0.20f, -0.55f, 0.0f, 1.0f, 0.3f, 0.3f, 0.0f,
      -0.55f,  0.55f, 0.0f, 1.0f, 0.3f, 0.3f, 1.0f,
       0.15f,  0.55f, 0.0f, 1.0f, 0.3f, 0.3f, 0.5f,
      // positions          // color
       0.00f, -0.40f, 0.0f, 0.3f, 1.0f, 0.3f, 0.0f,
      -0.35f,  0.70f, 0.0f, 0.3f, 1.0f, 0.3f, 1.0f,
       0.35f,  0.70f, 0.0f, 0.3f, 1.0f, 0.3f, 0.5f,
      // positions          // color
       0.20f, -0.70f, 0.0f, 0.3f, 0.3f, 1.0f, 0.0f,
      -0.15f,  0.40f, 0.0f, 0.3f, 0.3f, 1.0f, 1.0f,
       0.55f,  0.40f, 0.0f, 0.3f, 0.3f, 1.0f, 0.5f
    };

    InitVkDestroyer( LogicalDevice, VertexBuffer );
    if( !CreateBuffer( *LogicalDevice, sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, *VertexBuffer ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, BufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *VertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *BufferMemory ) ) {
      return false;
    }

    if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( vertices[0] ) * vertices.size(), &vertices[0], *VertexBuffer, 0, 0,
      VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, GraphicsQueue.Handle, FramesResources.front().CommandBuffer, {} ) ) {
      return false;
    }

    return true;
  }

  virtual bool Draw() override {
    auto prepare_frame = [&]( VkCommandBuffer command_buffer, uint32_t swapchain_image_index, VkFramebuffer framebuffer ) {
      if( !BeginCommandBufferRecordingOperation( command_buffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
        return false;
      }

      if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
        ImageTransition image_transition_before_drawing = {
          Swapchain.Images[swapchain_image_index],  // VkImage              Image
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
          VK_IMAGE_LAYOUT_UNDEFINED,                // VkImageLayout        CurrentLayout
          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout        NewLayout
          PresentQueue.FamilyIndex,                 // uint32_t             CurrentQueueFamily
          GraphicsQueue.FamilyIndex,                // uint32_t             NewQueueFamily
          VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
        };
        SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { image_transition_before_drawing } );
      }

      // Drawing
      BeginRenderPass( command_buffer, *RenderPass, framebuffer, { { 0, 0 }, Swapchain.Size }, { { 0.1f, 0.2f, 0.3f, 1.0f } }, VK_SUBPASS_CONTENTS_INLINE );

      BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *GraphicsPipeline );
      VkViewport viewport = {
        0.0f,                                       // float    x
        0.0f,                                       // float    y
        static_cast<float>(Swapchain.Size.width),   // float    width
        static_cast<float>(Swapchain.Size.height),  // float    height
        0.0f,                                       // float    minDepth
        1.0f,                                       // float    maxDepth
      };
      SetViewportStateDynamically( command_buffer, 0, { viewport } );

      VkRect2D scissor = {
        {                                           // VkOffset2D     offset
          0,                                          // int32_t        x
          0                                           // int32_t        y
        },
        {                                           // VkExtent2D     extent
          Swapchain.Size.width,                       // uint32_t       width
          Swapchain.Size.height                       // uint32_t       height
        }
      };
      SetScissorStateDynamically( command_buffer, 0, { scissor } );

      BindVertexBuffers( command_buffer, 0, { { *VertexBuffer, 0 } } );

      DrawGeometry( command_buffer, 9, 1, 0, 0 );

      EndRenderPass( command_buffer );

      if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
        ImageTransition image_transition_before_present = {
          Swapchain.Images[swapchain_image_index],  // VkImage              Image
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        CurrentLayout
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        NewLayout
          GraphicsQueue.FamilyIndex,                // uint32_t             CurrentQueueFamily
          PresentQueue.FamilyIndex,                 // uint32_t             NewQueueFamily
          VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
        };
        SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, { image_transition_before_present } );
      }

      if( !EndCommandBufferRecordingOperation( command_buffer ) ) {
        return false;
      }
      return true;
    };

    return IncreasePerformanceThroughIncreasingTheNumberOfSeparatelyRenderedFrames( *LogicalDevice, GraphicsQueue.Handle, PresentQueue.Handle,
      *Swapchain.Handle, Swapchain.Size, Swapchain.ImageViewsRaw, *RenderPass, {}, prepare_frame, FramesResources );
  }

  void OnMouseEvent() override{
    if( MouseState.Buttons[0].WasClicked ) {
      static bool enable_blending = false;
      CreatePipelineObject( enable_blending );
      enable_blending = !enable_blending;
    }
  }

  bool CreatePipelineObject( bool enable_blending ) {
    WaitForAllSubmittedCommandsToBeFinished( *LogicalDevice );

    std::vector<unsigned char> vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/13 Enabling Alpha Blending/shader.vert.spv", vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer(VkShaderModule) vertex_shader_module;
    InitVkDestroyer( LogicalDevice, vertex_shader_module );
    if( !CreateShaderModule( *LogicalDevice, vertex_shader_spirv, *vertex_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> fragment_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/13 Enabling Alpha Blending/shader.frag.spv", fragment_shader_spirv ) ) {
      return false;
    }
    VkDestroyer(VkShaderModule) fragment_shader_module;
    InitVkDestroyer( LogicalDevice, fragment_shader_module );
    if( !CreateShaderModule( *LogicalDevice, fragment_shader_spirv, *fragment_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,   // VkShaderStageFlagBits        ShaderStage
        *vertex_shader_module,        // VkShaderModule               ShaderModule
        "main",                       // char const                 * EntryPointName;
        nullptr                       // VkSpecializationInfo const * SpecializationInfo;
      },
      {
        VK_SHADER_STAGE_FRAGMENT_BIT, // VkShaderStageFlagBits        ShaderStage
        *fragment_shader_module,      // VkShaderModule               ShaderModule
        "main",                       // char const                 * EntryPointName
        nullptr                       // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos;
    SpecifyPipelineShaderStages( shader_stage_params, shader_stage_create_infos );

    std::vector<VkVertexInputBindingDescription> vertex_input_binding_descriptions = {
      {
        0,                            // uint32_t                     binding
        7 * sizeof( float ),          // uint32_t                     stride
        VK_VERTEX_INPUT_RATE_VERTEX   // VkVertexInputRate            inputRate
      }
    };

    std::vector<VkVertexInputAttributeDescription> vertex_attribute_descriptions = {
      {
        0,                              // uint32_t                     location
        0,                              // uint32_t                     binding
        VK_FORMAT_R32G32B32_SFLOAT,     // VkFormat                     format
        0                               // uint32_t                     offset
      },
      {
        1,                              // uint32_t                     location
        0,                              // uint32_t                     binding
        VK_FORMAT_R32G32B32A32_SFLOAT,  // VkFormat                     format
        3 * sizeof( float )             // uint32_t                     offset
      }
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( vertex_input_binding_descriptions, vertex_attribute_descriptions, vertex_input_state_create_info );

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info;
    SpecifyPipelineInputAssemblyState( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false, input_assembly_state_create_info );

    ViewportInfo viewport_infos = {
      {                     // std::vector<VkViewport>   Viewports
        {
          0.0f,               // float          x
          0.0f,               // float          y
          500.0f,             // float          width
          500.0f,             // float          height
          0.0f,               // float          minDepth
          1.0f                // float          maxDepth
        }
      },
      {                     // std::vector<VkRect2D>     Scissors
        {
          {                   // VkOffset2D     offset
            0,                  // int32_t        x
            0                   // int32_t        y
          },
          {                   // VkExtent2D     extent
            500,                // uint32_t       width
            500                 // uint32_t       height
          }
        }
      }
    };
    VkPipelineViewportStateCreateInfo viewport_state_create_info;
    SpecifyPipelineViewportAndScissorTestState( viewport_infos, viewport_state_create_info );

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info;
    SpecifyPipelineRasterizationState( false, false, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, false, 0.0f, 0.0f, 0.0f, 1.0f, rasterization_state_create_info );

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info;
    SpecifyPipelineMultisampleState( VK_SAMPLE_COUNT_1_BIT, false, 0.0f, nullptr, false, false, multisample_state_create_info );

    std::vector<VkPipelineColorBlendAttachmentState> attachment_blend_states = {
      {
        enable_blending,                      // VkBool32                 blendEnable
        VK_BLEND_FACTOR_SRC_ALPHA,            // VkBlendFactor            srcColorBlendFactor
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,  // VkBlendFactor            dstColorBlendFactor
        VK_BLEND_OP_ADD,                      // VkBlendOp                colorBlendOp
        VK_BLEND_FACTOR_SRC_ALPHA,            // VkBlendFactor            srcAlphaBlendFactor
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,  // VkBlendFactor            dstAlphaBlendFactor
        VK_BLEND_OP_ADD,                      // VkBlendOp                alphaBlendOp
        VK_COLOR_COMPONENT_R_BIT |            // VkColorComponentFlags    colorWriteMask
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT
      }
    };
    VkPipelineColorBlendStateCreateInfo blend_state_create_info;
    SpecifyPipelineBlendState( false, VK_LOGIC_OP_COPY, attachment_blend_states, { 1.0f, 1.0f, 1.0f, 1.0f }, blend_state_create_info );

    std::vector<VkDynamicState> dynamic_states = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info;
    SpecifyPipelineDynamicStates( dynamic_states, dynamic_state_create_info );

    VkDestroyer(VkPipelineLayout) pipeline_layout;
    InitVkDestroyer( LogicalDevice, pipeline_layout );
    if( !CreatePipelineLayout( *LogicalDevice, {}, {}, *pipeline_layout ) ) {
      return false;
    }

    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, shader_stage_create_infos, vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, rasterization_state_create_info, &multisample_state_create_info, nullptr, &blend_state_create_info,
      &dynamic_state_create_info, *pipeline_layout, *RenderPass, 0, VK_NULL_HANDLE, -1, graphics_pipeline_create_info );

    std::vector<VkPipeline> graphics_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { graphics_pipeline_create_info }, VK_NULL_HANDLE, graphics_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, GraphicsPipeline );
    *GraphicsPipeline = graphics_pipeline[0];

    return true;
  }

  virtual bool Resize() override {
    if( !CreateSwapchain( VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, false ) ) {
      return false;
    }
    return true;
  }

};

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "13 - Enabling Alpha Blending", 50, 25, 1280, 800, Sample )
