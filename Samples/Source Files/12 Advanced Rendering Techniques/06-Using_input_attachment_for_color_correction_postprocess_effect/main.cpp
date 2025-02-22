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
// © Packt Publishing Limited
//
// Author:   Pawel Lapinski
// LinkedIn: https://www.linkedin.com/in/pawel-lapinski-84522329
//
// Chapter: 12 Advanced Rendering Techniques
// Recipe:  06 Using input attachment for color correction postprocess effect

#include "CookbookSampleFramework.h"
#include "OrbitingCamera.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSample {
  Mesh                                Skybox;
  VkDestroyer(VkBuffer)               SkyboxVertexBuffer;
  VkDestroyer(VkDeviceMemory)         SkyboxVertexBufferMemory;

  Mesh                                Model;
  VkDestroyer(VkBuffer)               ModelVertexBuffer;
  VkDestroyer(VkDeviceMemory)         ModelVertexBufferMemory;

  VkDestroyer(VkBuffer)               PostprocessVertexBuffer;
  VkDestroyer(VkDeviceMemory)         PostprocessVertexBufferMemory;

  VkDestroyer(VkImage)                CubemapImage;
  VkDestroyer(VkDeviceMemory)         CubemapImageMemory;
  VkDestroyer(VkSampler)              CubemapSampler;
  VkDestroyer(VkImageView)            CubemapImageView;

  VkDestroyer(VkDescriptorSetLayout)  DescriptorSetLayout;
  VkDestroyer(VkDescriptorPool)       DescriptorPool;
  std::vector<VkDescriptorSet>        DescriptorSets;

  VkDestroyer(VkImage)                SceneImage;
  VkDestroyer(VkDeviceMemory)         SceneImageMemory;
  VkDestroyer(VkImageView)            SceneImageView;
  VkDestroyer(VkFence)                SceneFence;

  VkDestroyer(VkDescriptorSetLayout)  PostprocessDescriptorSetLayout;
  VkDestroyer(VkDescriptorPool)       PostprocessDescriptorPool;
  std::vector<VkDescriptorSet>        PostprocessDescriptorSets;

  VkDestroyer(VkRenderPass)           RenderPass;
  VkDestroyer(VkPipelineLayout)       PipelineLayout;
  VkDestroyer(VkPipeline)             SkyboxPipeline;
  VkDestroyer(VkPipeline)             ModelPipeline;
  VkDestroyer(VkPipelineLayout)       PostprocessPipelineLayout;
  VkDestroyer(VkPipeline)             PostprocessPipeline;

  VkDestroyer(VkBuffer)               StagingBuffer;
  VkDestroyer(VkDeviceMemory)         StagingBufferMemory;
  bool                                UpdateUniformBuffer;
  VkDestroyer(VkBuffer)               UniformBuffer;
  VkDestroyer(VkDeviceMemory)         UniformBufferMemory;

  OrbitingCamera                      Camera;

  virtual bool Initialize( WindowParameters window_parameters ) override {
    if( !InitializeVulkan( window_parameters ) ) {
      return false;
    }

    Camera = OrbitingCamera( Vector3{ 0.0f, 0.0f, 0.0f }, 4.0f );

    InitVkDestroyer( LogicalDevice, SceneFence );
    if( !CreateFence( *LogicalDevice, true, *SceneFence ) ) {
      return false;
    }

    // Vertex data - model

    if( !Load3DModelFromObjFile( "Data/Models/sphere.obj", true, false, false, true, Model ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, ModelVertexBuffer );
    if( !CreateBuffer( *LogicalDevice, sizeof( Model.Data[0] ) * Model.Data.size(),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, *ModelVertexBuffer ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, ModelVertexBufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *ModelVertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *ModelVertexBufferMemory ) ) {
      return false;
    }

    if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( Model.Data[0] ) * Model.Data.size(),
      &Model.Data[0], *ModelVertexBuffer, 0, 0, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
      GraphicsQueue.Handle, FramesResources.front().CommandBuffer, {} ) ) {
      return false;
    }

    // Vertex data - skybox

    if( !Load3DModelFromObjFile( "Data/Models/cube.obj", false, false, false, false, Skybox ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, SkyboxVertexBuffer );
    if( !CreateBuffer( *LogicalDevice, sizeof( Skybox.Data[0] ) * Skybox.Data.size(),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, *SkyboxVertexBuffer ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, SkyboxVertexBufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *SkyboxVertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *SkyboxVertexBufferMemory ) ) {
      return false;
    }

    if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( Skybox.Data[0] ) * Skybox.Data.size(),
      &Skybox.Data[0], *SkyboxVertexBuffer, 0, 0, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
      GraphicsQueue.Handle, FramesResources.front().CommandBuffer, {} ) ) {
      return false;
    }

    // Fullscreen quad for postprocess

    std::vector<float> vertices = {
      // positions
      -1.0f, -1.0f, 0.0f,
      -1.0f,  1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
      -1.0f,  1.0f, 0.0f,
       1.0f,  1.0f, 0.0f,
    };

    InitVkDestroyer( LogicalDevice, PostprocessVertexBuffer );
    if( !CreateBuffer( *LogicalDevice, sizeof( vertices[0] ) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, *PostprocessVertexBuffer ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, PostprocessVertexBufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *PostprocessVertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *PostprocessVertexBufferMemory ) ) {
      return false;
    }

    if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( vertices[0] ) * vertices.size(), &vertices[0], *PostprocessVertexBuffer, 0, 0,
      VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, GraphicsQueue.Handle, FramesResources.front().CommandBuffer, {} ) ) {
      return false;
    }

    // Staging buffer

    InitVkDestroyer( LogicalDevice, StagingBuffer );
    if( !CreateBuffer( *LogicalDevice, 2 * 16 * sizeof( float ), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, *StagingBuffer ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, StagingBufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *StagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, *StagingBufferMemory ) ) {
      return false;
    }

    // Uniform buffer

    InitVkDestroyer( LogicalDevice, UniformBuffer );
    InitVkDestroyer( LogicalDevice, UniformBufferMemory );
    if( !CreateUniformBuffer( PhysicalDevice, *LogicalDevice, 2 * 16 * sizeof( float ), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      *UniformBuffer, *UniformBufferMemory ) ) {
      return false;
    }

    if( !UpdateStagingBuffer( true ) ) {
      return false;
    }

    // Cubemap

    InitVkDestroyer( LogicalDevice, CubemapImage );
    InitVkDestroyer( LogicalDevice, CubemapImageMemory );
    InitVkDestroyer( LogicalDevice, CubemapImageView );
    InitVkDestroyer( LogicalDevice, CubemapSampler );
    if( !CreateCombinedImageSampler( PhysicalDevice, *LogicalDevice, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM, { 1024, 1024, 1 }, 1, 6,
      VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, true, VK_IMAGE_VIEW_TYPE_CUBE, VK_IMAGE_ASPECT_COLOR_BIT, VK_FILTER_LINEAR,
      VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
      VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 0.0f, false, 1.0f, false, VK_COMPARE_OP_ALWAYS, 0.0f, 1.0f, VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
      false, *CubemapSampler, *CubemapImage, *CubemapImageMemory, *CubemapImageView ) ) {
      return false;
    }

    std::vector<std::string> cubemap_images = {
      "Data/Textures/Skansen/posx.jpg",
      "Data/Textures/Skansen/negx.jpg",
      "Data/Textures/Skansen/posy.jpg",
      "Data/Textures/Skansen/negy.jpg",
      "Data/Textures/Skansen/posz.jpg",
      "Data/Textures/Skansen/negz.jpg"
    };

    for( size_t i = 0; i < cubemap_images.size(); ++i ) {
      std::vector<unsigned char> cubemap_image_data;
      int image_data_size;
      if( !LoadTextureDataFromFile( cubemap_images[i].c_str(), 4, cubemap_image_data, nullptr, nullptr, nullptr, &image_data_size ) ) {
        return false;
      }
      VkImageSubresourceLayers image_subresource = {
        VK_IMAGE_ASPECT_COLOR_BIT,    // VkImageAspectFlags     aspectMask
        0,                            // uint32_t               mipLevel
        static_cast<uint32_t>(i),     // uint32_t               baseArrayLayer
        1                             // uint32_t               layerCount
      };
      UseStagingBufferToUpdateImageWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, image_data_size, &cubemap_image_data[0],
        *CubemapImage, image_subresource, { 0, 0, 0 }, { 1024, 1024, 1 }, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        0, VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        GraphicsQueue.Handle, FramesResources.front().CommandBuffer, {} );
    }

    // Descriptor set

    std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings = {
      {
        0,                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType     descriptorType
        1,                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_VERTEX_BIT,                 // VkShaderStageFlags   stageFlags
        nullptr                                     // const VkSampler    * pImmutableSamplers
      },
      {
        1,                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // VkDescriptorType     descriptorType
        1,                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_FRAGMENT_BIT,               // VkShaderStageFlags   stageFlags
        nullptr                                     // const VkSampler    * pImmutableSamplers
      }
    };
    InitVkDestroyer( LogicalDevice, DescriptorSetLayout );
    if( !CreateDescriptorSetLayout( *LogicalDevice, descriptor_set_layout_bindings, *DescriptorSetLayout ) ) {
      return false;
    }

    std::vector<VkDescriptorPoolSize> descriptor_pool_sizes = {
      {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType     type
        1                                           // uint32_t             descriptorCount
      },
      {
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // VkDescriptorType     type
        1                                           // uint32_t             descriptorCount
      }
    };
    InitVkDestroyer( LogicalDevice, DescriptorPool );
    if( !CreateDescriptorPool( *LogicalDevice, false, 1, descriptor_pool_sizes, *DescriptorPool ) ) {
      return false;
    }

    if( !AllocateDescriptorSets( *LogicalDevice, *DescriptorPool, { *DescriptorSetLayout }, DescriptorSets ) ) {
      return false;
    }

    BufferDescriptorInfo buffer_descriptor_update = {
      DescriptorSets[0],                          // VkDescriptorSet                      TargetDescriptorSet
      0,                                          // uint32_t                             TargetDescriptorBinding
      0,                                          // uint32_t                             TargetArrayElement
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType                     TargetDescriptorType
      {                                           // std::vector<VkDescriptorBufferInfo>  BufferInfos
        {
          *UniformBuffer,                           // VkBuffer                             buffer
          0,                                        // VkDeviceSize                         offset
          VK_WHOLE_SIZE                             // VkDeviceSize                         range
        }
      }
    };

    ImageDescriptorInfo image_descriptor_update = {
      DescriptorSets[0],                          // VkDescriptorSet                      TargetDescriptorSet
      1,                                          // uint32_t                             TargetDescriptorBinding
      0,                                          // uint32_t                             TargetArrayElement
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // VkDescriptorType                     TargetDescriptorType
      {                                           // std::vector<VkDescriptorImageInfo>   ImageInfos
        {
          *CubemapSampler,                          // VkSampler                            sampler
          *CubemapImageView,                        // VkImageView                          imageView
          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL  // VkImageLayout                        imageLayout
        }
      }
    };

    UpdateDescriptorSets( *LogicalDevice, { image_descriptor_update }, { buffer_descriptor_update }, {}, {} );

    // Postprocess descriptor set - with input attachment

    std::vector<VkDescriptorSetLayoutBinding> scene_descriptor_set_layout_bindings = {
      {
        0,                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,        // VkDescriptorType     descriptorType
        1,                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_FRAGMENT_BIT,               // VkShaderStageFlags   stageFlags
        nullptr                                     // const VkSampler    * pImmutableSamplers
      }
    };
    InitVkDestroyer( LogicalDevice, PostprocessDescriptorSetLayout );
    if( !CreateDescriptorSetLayout( *LogicalDevice, scene_descriptor_set_layout_bindings, *PostprocessDescriptorSetLayout ) ) {
      return false;
    }

    std::vector<VkDescriptorPoolSize> scene_descriptor_pool_sizes = {
      {
        VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,        // VkDescriptorType     type
        1                                           // uint32_t             descriptorCount
      }
    };
    InitVkDestroyer( LogicalDevice, PostprocessDescriptorPool );
    if( !CreateDescriptorPool( *LogicalDevice, false, 1, scene_descriptor_pool_sizes, *PostprocessDescriptorPool ) ) {
      return false;
    }

    if( !AllocateDescriptorSets( *LogicalDevice, *PostprocessDescriptorPool, { *PostprocessDescriptorSetLayout }, PostprocessDescriptorSets ) ) {
      return false;
    }

    // Render pass

    std::vector<VkAttachmentDescription> attachment_descriptions = {
      {
        0,                                                // VkAttachmentDescriptionFlags     flags
        Swapchain.Format,                                 // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                      // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL          // VkImageLayout                    finalLayout
      },
      {
        0,                                                // VkAttachmentDescriptionFlags     flags
        DepthFormat,                                      // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                      // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                    finalLayout
      },
      {
        0,                                                // VkAttachmentDescriptionFlags     flags
        Swapchain.Format,                                 // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_STORE,                     // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR                   // VkImageLayout                    finalLayout
      }
    };

    VkAttachmentReference depth_attachment = {
      1,                                                // uint32_t                             attachment
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                        layout
    };

    std::vector<SubpassParameters> subpass_parameters = {
      {
        VK_PIPELINE_BIND_POINT_GRAPHICS,              // VkPipelineBindPoint                  PipelineType
        {},                                           // std::vector<VkAttachmentReference>   InputAttachments
        {                                             // std::vector<VkAttachmentReference>   ColorAttachments
          {
            0,                                          // uint32_t                             attachment
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,   // VkImageLayout                        layout
          }
        },
        {},                                           // std::vector<VkAttachmentReference>   ResolveAttachments
        &depth_attachment,                            // VkAttachmentReference const        * DepthStencilAttachment
        {}                                            // std::vector<uint32_t>                PreserveAttachments
      },
      {
        VK_PIPELINE_BIND_POINT_GRAPHICS,              // VkPipelineBindPoint                  PipelineType
        {                                             // std::vector<VkAttachmentReference>   InputAttachments
          {
            0,                                          // uint32_t                             attachment
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,   // VkImageLayout                        layout
          }
        },
        {                                             // std::vector<VkAttachmentReference>   ColorAttachments
          {
            2,                                          // uint32_t                             attachment
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,   // VkImageLayout                        layout
          }
        },
        {},                                           // std::vector<VkAttachmentReference>   ResolveAttachments
        nullptr,                                      // VkAttachmentReference const        * DepthStencilAttachment
        {}                                            // std::vector<uint32_t>                PreserveAttachments
      }
    };

std::vector<VkSubpassDependency> subpass_dependencies = {
  {
    0,                                              // uint32_t                   srcSubpass
    1,                                              // uint32_t                   dstSubpass
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // VkPipelineStageFlags       srcStageMask
    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,          // VkPipelineStageFlags       dstStageMask
    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           // VkAccessFlags              srcAccessMask
    VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,            // VkAccessFlags              dstAccessMask
    VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
  },
  {
    VK_SUBPASS_EXTERNAL,                            // uint32_t                   srcSubpass
    1,                                              // uint32_t                   dstSubpass
    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,              // VkPipelineStageFlags       srcStageMask
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // VkPipelineStageFlags       dstStageMask
    VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags              srcAccessMask
    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           // VkAccessFlags              dstAccessMask
    VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
  },
  {
    1,                                              // uint32_t                   srcSubpass
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

    // Graphics pipeline

    // Common

    std::vector<VkPushConstantRange> push_constant_ranges = {
      {
        VK_SHADER_STAGE_FRAGMENT_BIT,   // VkShaderStageFlags     stageFlags
        0,                              // uint32_t               offset
        sizeof( float ) * 4             // uint32_t               size
      }
    };
    InitVkDestroyer( LogicalDevice, PipelineLayout );
    if( !CreatePipelineLayout( *LogicalDevice, { *DescriptorSetLayout }, push_constant_ranges, *PipelineLayout ) ) {
      return false;
    }

    // Postprocess

    InitVkDestroyer( LogicalDevice, PostprocessPipelineLayout );
    if( !CreatePipelineLayout( *LogicalDevice, { *PostprocessDescriptorSetLayout }, push_constant_ranges, *PostprocessPipelineLayout ) ) {
      return false;
    }

    // Model

    std::vector<unsigned char> model_vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/06 Using input attachment for color correction postprocess effect/model.vert.spv", model_vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer(VkShaderModule) model_vertex_shader_module;
    InitVkDestroyer( LogicalDevice, model_vertex_shader_module );
    if( !CreateShaderModule( *LogicalDevice, model_vertex_shader_spirv, *model_vertex_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> model_fragment_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/06 Using input attachment for color correction postprocess effect/model.frag.spv", model_fragment_shader_spirv ) ) {
      return false;
    }
    VkDestroyer(VkShaderModule) model_fragment_shader_module;
    InitVkDestroyer( LogicalDevice, model_fragment_shader_module );
    if( !CreateShaderModule( *LogicalDevice, model_fragment_shader_spirv, *model_fragment_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> model_shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,       // VkShaderStageFlagBits        ShaderStage
        *model_vertex_shader_module,      // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName;
        nullptr                           // VkSpecializationInfo const * SpecializationInfo;
      },
      {
        VK_SHADER_STAGE_FRAGMENT_BIT,     // VkShaderStageFlagBits        ShaderStage
        *model_fragment_shader_module,    // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName
        nullptr                           // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> model_shader_stage_create_infos;
    SpecifyPipelineShaderStages( model_shader_stage_params, model_shader_stage_create_infos );

    std::vector<VkVertexInputBindingDescription> model_vertex_input_binding_descriptions = {
      {
        0,                                // uint32_t                     binding
        6 * sizeof( float ),              // uint32_t                     stride
        VK_VERTEX_INPUT_RATE_VERTEX       // VkVertexInputRate            inputRate
      }
    };

    std::vector<VkVertexInputAttributeDescription> model_vertex_attribute_descriptions = {
      {
        0,                                // uint32_t   location
        0,                                // uint32_t   binding
        VK_FORMAT_R32G32B32_SFLOAT,       // VkFormat   format
        0                                 // uint32_t   offset
      },
      {
        1,                                // uint32_t   location
        0,                                // uint32_t   binding
        VK_FORMAT_R32G32B32_SFLOAT,       // VkFormat   format
        3 * sizeof( float )               // uint32_t   offset
      }
    };

    VkPipelineVertexInputStateCreateInfo model_vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( model_vertex_input_binding_descriptions, model_vertex_attribute_descriptions, model_vertex_input_state_create_info );

    // Skybox

    std::vector<unsigned char> skybox_vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/06 Using input attachment for color correction postprocess effect/skybox.vert.spv", skybox_vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer(VkShaderModule) skybox_vertex_shader_module;
    InitVkDestroyer( LogicalDevice, skybox_vertex_shader_module );
    if( !CreateShaderModule( *LogicalDevice, skybox_vertex_shader_spirv, *skybox_vertex_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> skybox_fragment_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/06 Using input attachment for color correction postprocess effect/skybox.frag.spv", skybox_fragment_shader_spirv ) ) {
      return false;
    }
    VkDestroyer(VkShaderModule) skybox_fragment_shader_module;
    InitVkDestroyer( LogicalDevice, skybox_fragment_shader_module );
    if( !CreateShaderModule( *LogicalDevice, skybox_fragment_shader_spirv, *skybox_fragment_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> skybox_shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,       // VkShaderStageFlagBits        ShaderStage
        *skybox_vertex_shader_module,     // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName;
        nullptr                           // VkSpecializationInfo const * SpecializationInfo;
      },
      {
        VK_SHADER_STAGE_FRAGMENT_BIT,     // VkShaderStageFlagBits        ShaderStage
        *skybox_fragment_shader_module,   // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName
        nullptr                           // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> skybox_shader_stage_create_infos;
    SpecifyPipelineShaderStages( skybox_shader_stage_params, skybox_shader_stage_create_infos );

    std::vector<VkVertexInputBindingDescription> skybox_vertex_input_binding_descriptions = {
      {
        0,                            // uint32_t                     binding
        3 * sizeof( float ),          // uint32_t                     stride
        VK_VERTEX_INPUT_RATE_VERTEX   // VkVertexInputRate            inputRate
      }
    };

    std::vector<VkVertexInputAttributeDescription> skybox_vertex_attribute_descriptions = {
      {
        0,                                                                        // uint32_t   location
        0,                                                                        // uint32_t   binding
        VK_FORMAT_R32G32B32_SFLOAT,                                               // VkFormat   format
        0                                                                         // uint32_t   offset
      }
    };

    VkPipelineVertexInputStateCreateInfo skybox_vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( skybox_vertex_input_binding_descriptions, skybox_vertex_attribute_descriptions, skybox_vertex_input_state_create_info );

    // Postprocess

    std::vector<unsigned char> postprocess_vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/06 Using input attachment for color correction postprocess effect/postprocess.vert.spv", postprocess_vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer(VkShaderModule) postprocess_vertex_shader_module;
    InitVkDestroyer( LogicalDevice, postprocess_vertex_shader_module );
    if( !CreateShaderModule( *LogicalDevice, postprocess_vertex_shader_spirv, *postprocess_vertex_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> postprocess_fragment_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/06 Using input attachment for color correction postprocess effect/postprocess.frag.spv", postprocess_fragment_shader_spirv ) ) {
      return false;
    }
    VkDestroyer(VkShaderModule) postprocess_fragment_shader_module;
    InitVkDestroyer( LogicalDevice, postprocess_fragment_shader_module );
    if( !CreateShaderModule( *LogicalDevice, postprocess_fragment_shader_spirv, *postprocess_fragment_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> postprocess_shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,           // VkShaderStageFlagBits        ShaderStage
        *postprocess_vertex_shader_module,    // VkShaderModule               ShaderModule
        "main",                               // char const                 * EntryPointName;
        nullptr                               // VkSpecializationInfo const * SpecializationInfo;
      },
      {
        VK_SHADER_STAGE_FRAGMENT_BIT,         // VkShaderStageFlagBits        ShaderStage
        *postprocess_fragment_shader_module,  // VkShaderModule               ShaderModule
        "main",                               // char const                 * EntryPointName
        nullptr                               // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> postprocess_shader_stage_create_infos;
    SpecifyPipelineShaderStages( postprocess_shader_stage_params, postprocess_shader_stage_create_infos );

    std::vector<VkVertexInputBindingDescription> postprocess_vertex_input_binding_descriptions = {
      {
        0,                            // uint32_t                     binding
        3 * sizeof( float ),          // uint32_t                     stride
        VK_VERTEX_INPUT_RATE_VERTEX   // VkVertexInputRate            inputRate
      }
    };

    std::vector<VkVertexInputAttributeDescription> postprocess_vertex_attribute_descriptions = {
      {
        0,                                                                        // uint32_t   location
        0,                                                                        // uint32_t   binding
        VK_FORMAT_R32G32B32_SFLOAT,                                               // VkFormat   format
        0                                                                         // uint32_t   offset
      }
    };

    VkPipelineVertexInputStateCreateInfo postprocess_vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( postprocess_vertex_input_binding_descriptions, postprocess_vertex_attribute_descriptions, postprocess_vertex_input_state_create_info );

    // Common

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

    // Model

    VkPipelineRasterizationStateCreateInfo model_rasterization_state_create_info;
    SpecifyPipelineRasterizationState( false, false, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, false, 0.0f, 0.0f, 0.0f, 1.0f, model_rasterization_state_create_info );

    // Skybox

    VkPipelineRasterizationStateCreateInfo skybox_rasterization_state_create_info;
    SpecifyPipelineRasterizationState( false, false, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, false, 0.0f, 0.0f, 0.0f, 1.0f, skybox_rasterization_state_create_info );

    // Postprocess

    VkPipelineRasterizationStateCreateInfo postprocess_rasterization_state_create_info;
    SpecifyPipelineRasterizationState( false, false, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, false, 0.0f, 0.0f, 0.0f, 1.0f, postprocess_rasterization_state_create_info );

    // Common

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info;
    SpecifyPipelineMultisampleState( VK_SAMPLE_COUNT_1_BIT, false, 0.0f, nullptr, false, false, multisample_state_create_info );

    VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info;
    SpecifyPipelineDepthAndStencilState( true, true, VK_COMPARE_OP_LESS_OR_EQUAL, false, 0.0f, 1.0f, false, {}, {}, depth_stencil_state_create_info );

    std::vector<VkPipelineColorBlendAttachmentState> attachment_blend_states = {
      {
        false,                          // VkBool32                 blendEnable
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            srcColorBlendFactor
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            dstColorBlendFactor
        VK_BLEND_OP_ADD,                // VkBlendOp                colorBlendOp
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            srcAlphaBlendFactor
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            dstAlphaBlendFactor
        VK_BLEND_OP_ADD,                // VkBlendOp                alphaBlendOp
        VK_COLOR_COMPONENT_R_BIT |      // VkColorComponentFlags    colorWriteMask
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

    // Model

    VkGraphicsPipelineCreateInfo model_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, model_shader_stage_create_infos, model_vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, model_rasterization_state_create_info, &multisample_state_create_info, &depth_stencil_state_create_info, &blend_state_create_info,
      &dynamic_state_create_info, *PipelineLayout, *RenderPass, 0, VK_NULL_HANDLE, -1, model_pipeline_create_info );

    std::vector<VkPipeline> model_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { model_pipeline_create_info }, VK_NULL_HANDLE, model_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, ModelPipeline );
    *ModelPipeline = model_pipeline[0];

    // Skybox

    VkGraphicsPipelineCreateInfo skybox_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, skybox_shader_stage_create_infos, skybox_vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, skybox_rasterization_state_create_info, &multisample_state_create_info, &depth_stencil_state_create_info, &blend_state_create_info,
      &dynamic_state_create_info, *PipelineLayout, *RenderPass, 0, VK_NULL_HANDLE, -1, skybox_pipeline_create_info );

    std::vector<VkPipeline> skybox_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { skybox_pipeline_create_info }, VK_NULL_HANDLE, skybox_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, SkyboxPipeline );
    *SkyboxPipeline = skybox_pipeline[0];

    // Postprocess

    VkGraphicsPipelineCreateInfo postprocess_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, postprocess_shader_stage_create_infos, postprocess_vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, postprocess_rasterization_state_create_info, &multisample_state_create_info, nullptr, &blend_state_create_info,
      &dynamic_state_create_info, *PostprocessPipelineLayout, *RenderPass, 1, VK_NULL_HANDLE, -1, postprocess_pipeline_create_info );

    std::vector<VkPipeline> postprocess_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { postprocess_pipeline_create_info }, VK_NULL_HANDLE, postprocess_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, PostprocessPipeline );
    *PostprocessPipeline = postprocess_pipeline[0];

	if(!createColorAttatchmentAndview()) return false;

    return true;
  }

  virtual bool Draw() override {
    auto prepare_frame = [&]( VkCommandBuffer command_buffer, uint32_t swapchain_image_index, VkFramebuffer framebuffer ) {
      if( !BeginCommandBufferRecordingOperation( command_buffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
        return false;
      }

      if( UpdateUniformBuffer ) {
        UpdateUniformBuffer = false;

        BufferTransition pre_transfer_transition = {
          *UniformBuffer,               // VkBuffer         Buffer
          VK_ACCESS_UNIFORM_READ_BIT,   // VkAccessFlags    CurrentAccess
          VK_ACCESS_TRANSFER_WRITE_BIT, // VkAccessFlags    NewAccess
          VK_QUEUE_FAMILY_IGNORED,      // uint32_t         CurrentQueueFamily
          VK_QUEUE_FAMILY_IGNORED       // uint32_t         NewQueueFamily
        };
        SetBufferMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, { pre_transfer_transition } );

        std::vector<VkBufferCopy> regions = {
          {
            0,                        // VkDeviceSize     srcOffset
            0,                        // VkDeviceSize     dstOffset
            2 * 16 * sizeof( float )  // VkDeviceSize     size
          }
        };
        CopyDataBetweenBuffers( command_buffer, *StagingBuffer, *UniformBuffer, regions );

        BufferTransition post_transfer_transition = {
          *UniformBuffer,               // VkBuffer         Buffer
          VK_ACCESS_TRANSFER_WRITE_BIT, // VkAccessFlags    CurrentAccess
          VK_ACCESS_UNIFORM_READ_BIT,   // VkAccessFlags    NewAccess
          VK_QUEUE_FAMILY_IGNORED,      // uint32_t         CurrentQueueFamily
          VK_QUEUE_FAMILY_IGNORED       // uint32_t         NewQueueFamily
        };
        SetBufferMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, { post_transfer_transition } );
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

      BeginRenderPass( command_buffer, *RenderPass, framebuffer, { { 0, 0 }, Swapchain.Size }, { { 0.1f, 0.2f, 0.3f, 1.0f },{ 1.0f, 0 } }, VK_SUBPASS_CONTENTS_INLINE );

      // #0 subpass

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

      BindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *PipelineLayout, 0, DescriptorSets, {} );

      // Draw model

      BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *ModelPipeline );

      BindVertexBuffers( command_buffer, 0, { { *ModelVertexBuffer, 0 } } );

      ProvideDataToShadersThroughPushConstants( command_buffer, *PipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof( float ) * 4, &Camera.GetPosition()[0] );

      for( size_t i = 0; i < Model.Parts.size(); ++i ) {
        DrawGeometry( command_buffer, Model.Parts[i].VertexCount, 1, Model.Parts[i].VertexOffset, 0 );
      }

      // Draw skybox

      BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *SkyboxPipeline );

      BindVertexBuffers( command_buffer, 0, { { *SkyboxVertexBuffer, 0 } } );

      for( size_t i = 0; i < Skybox.Parts.size(); ++i ) {
        DrawGeometry( command_buffer, Skybox.Parts[i].VertexCount, 1, Skybox.Parts[i].VertexOffset, 0 );
      }

      // #1 subpass

      ProgressToTheNextSubpass( command_buffer, VK_SUBPASS_CONTENTS_INLINE );

      BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *PostprocessPipeline );

      BindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *PostprocessPipelineLayout, 0, PostprocessDescriptorSets, {} );

      BindVertexBuffers( command_buffer, 0, { { *PostprocessVertexBuffer, 0 } } );

      float time = TimerState.GetTime();
      ProvideDataToShadersThroughPushConstants( command_buffer, *PostprocessPipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof( float ), &time );

      DrawGeometry( command_buffer, 6, 1, 0, 0 );

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

    static uint32_t frame_index = 0;
    auto & current_frame = FramesResources[frame_index];

    if( !WaitForFences( *LogicalDevice, { *SceneFence }, false, 2000000000 ) ) {
      return false;
    }
    if( !ResetFences( *LogicalDevice, { *SceneFence } ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, current_frame.Framebuffer );

    uint32_t image_index;
    if( !AcquireSwapchainImage( *LogicalDevice, *Swapchain.Handle, *current_frame.ImageAcquiredSemaphore, VK_NULL_HANDLE, image_index ) ) {
      return false;
    }

    std::vector<VkImageView> attachments = { *SceneImageView, *current_frame.DepthAttachment, Swapchain.ImageViewsRaw[image_index] };
    if( !CreateFramebuffer( *LogicalDevice, *RenderPass, attachments, Swapchain.Size.width, Swapchain.Size.height, 1, *current_frame.Framebuffer ) ) {
      return false;
    }

    if( !prepare_frame( current_frame.CommandBuffer, image_index, *current_frame.Framebuffer ) ) {
      return false;
    }

    std::vector<WaitSemaphoreInfo> wait_semaphore_infos = {
      {
        *current_frame.ImageAcquiredSemaphore,          // VkSemaphore            Semaphore
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT   // VkPipelineStageFlags   WaitingStage
      }
    };
    if( !SubmitCommandBuffersToQueue( GraphicsQueue.Handle, wait_semaphore_infos, { current_frame.CommandBuffer },
    { *current_frame.ReadyToPresentSemaphore }, *SceneFence ) ) {
      return false;
    }

    PresentInfo present_info = {
      *Swapchain.Handle,                            // VkSwapchainKHR         Swapchain
      image_index                                   // uint32_t               ImageIndex
    };
    if( !PresentImage( PresentQueue.Handle, { *current_frame.ReadyToPresentSemaphore }, { present_info } ) ) {
      return false;
    }

    frame_index = (frame_index + 1) % FramesResources.size();
    return true;
  }

  void OnMouseEvent() override{
    UpdateStagingBuffer( false );
  }

  bool UpdateStagingBuffer( bool force ) {
    if( MouseState.Buttons[0].IsPressed ||
      force ) {
      UpdateUniformBuffer = true;

      Camera.RotateHorizontally( 0.5f * MouseState.Position.Delta.X );
      Camera.RotateVertically( -0.5f * MouseState.Position.Delta.Y );

      Matrix4x4 view_matrix = Camera.GetMatrix();

      if( !MapUpdateAndUnmapHostVisibleMemory( *LogicalDevice, *StagingBufferMemory, 0, sizeof( view_matrix[0] ) * view_matrix.size(), &view_matrix[0], true, nullptr ) ) {
        return false;
      }

      Matrix4x4 perspective_matrix = PreparePerspectiveProjectionMatrix( static_cast<float>(Swapchain.Size.width) / static_cast<float>(Swapchain.Size.height),
        50.0f, 0.5f, 10.0f );

      if( !MapUpdateAndUnmapHostVisibleMemory( *LogicalDevice, *StagingBufferMemory, sizeof( view_matrix[0] ) * view_matrix.size(),
        sizeof( perspective_matrix[0] ) * perspective_matrix.size(), &perspective_matrix[0], true, nullptr ) ) {
        return false;
      }
    }
    return true;
  }


  bool createColorAttatchmentAndview()
  {
	  InitVkDestroyer( LogicalDevice, SceneImage );
      InitVkDestroyer( LogicalDevice, SceneImageMemory );
      InitVkDestroyer( LogicalDevice, SceneImageView );
      if( !CreateInputAttachment( PhysicalDevice, *LogicalDevice, VK_IMAGE_TYPE_2D, Swapchain.Format, { Swapchain.Size.width,
        Swapchain.Size.height, 1 }, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_ASPECT_COLOR_BIT, *SceneImage, *SceneImageMemory, *SceneImageView ) ) {
        return false;
      }

      // Postprocess descriptor set - with input attachment

      ImageDescriptorInfo scene_image_descriptor_update = {
        PostprocessDescriptorSets[0],               // VkDescriptorSet                      TargetDescriptorSet
        0,                                          // uint32_t                             TargetDescriptorBinding
        0,                                          // uint32_t                             TargetArrayElement
        VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,        // VkDescriptorType                     TargetDescriptorType
        {                                           // std::vector<VkDescriptorImageInfo>   ImageInfos
          {
            VK_NULL_HANDLE,                           // VkSampler                            sampler
            *SceneImageView,                          // VkImageView                          imageView
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL  // VkImageLayout                        imageLayout
          }
        }
      };

      UpdateDescriptorSets( *LogicalDevice, { scene_image_descriptor_update }, {}, {}, {} );

      if( !UpdateStagingBuffer( true ) ) {
        return false;
      }
	  return true;
  }

  virtual bool Resize() override {
    if( !CreateSwapchain() ) {
      return false;
    }

    if( IsReady() ) {
      // Scene image (color attachment in 1st subpass, input attachment in 2nd subpass
		if(!createColorAttatchmentAndview())
			return false;
    }
    return true;
  }

};

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "12/06 - Using input attachment for color correction postprocess effect", 50, 25, 1280, 800, Sample )
