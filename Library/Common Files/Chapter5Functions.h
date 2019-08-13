// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and / or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The below copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// Vulkan Cookbook
// ISBN: 9781786468154
// © Packt Publishing Limited
//
// Author:   Pawel Lapinski
// LinkedIn: https://www.linkedin.com/in/pawel-lapinski-84522329
//
// Chapter: 05 Descriptor Sets
// Recipe:  01 Creating a sampler

#ifndef CREATING_A_SAMPLER
#define CREATING_A_SAMPLER

#include "Common.h"

namespace VulkanCookbook {

  bool CreateSampler( VkDevice               logical_device,
                      VkFilter               mag_filter,
                      VkFilter               min_filter,
                      VkSamplerMipmapMode    mipmap_mode,
                      VkSamplerAddressMode   u_address_mode,
                      VkSamplerAddressMode   v_address_mode,
                      VkSamplerAddressMode   w_address_mode,
                      float                  lod_bias,
                      bool                   anisotropy_enable,
                      float                  max_anisotropy,
                      bool                   compare_enable,
                      VkCompareOp            compare_operator,
                      float                  min_lod,
                      float                  max_lod,
                      VkBorderColor          border_color,
                      bool                   unnormalized_coords,
                      VkSampler            & sampler );

  bool CreateSampledImage( VkPhysicalDevice    physical_device,
                           VkDevice            logical_device,
                           VkImageType         type,
                           VkFormat            format,
                           VkExtent3D          size,
                           uint32_t            num_mipmaps,
                           uint32_t            num_layers,
                           VkImageUsageFlags   usage,
                           bool                cubemap,
                           VkImageViewType     view_type,
                           VkImageAspectFlags  aspect,
                           bool                linear_filtering,
                           VkImage           & sampled_image,
                           VkDeviceMemory    & memory_object,
                           VkImageView       & sampled_image_view );

  bool CreateCombinedImageSampler( VkPhysicalDevice       physical_device,
                                   VkDevice               logical_device,
                                   VkImageType            type,
                                   VkFormat               format,
                                   VkExtent3D             size,
                                   uint32_t               num_mipmaps,
                                   uint32_t               num_layers,
                                   VkImageUsageFlags      usage,
                                   bool                   cubemap,
                                   VkImageViewType        view_type,
                                   VkImageAspectFlags     aspect,
                                   VkFilter               mag_filter,
                                   VkFilter               min_filter,
                                   VkSamplerMipmapMode    mipmap_mode,
                                   VkSamplerAddressMode   u_address_mode,
                                   VkSamplerAddressMode   v_address_mode,
                                   VkSamplerAddressMode   w_address_mode,
                                   float                  lod_bias,
                                   bool                   anisotropy_enable,
                                   float                  max_anisotropy,
                                   bool                   compare_enable,
                                   VkCompareOp            compare_operator,
                                   float                  min_lod,
                                   float                  max_lod,
                                   VkBorderColor          border_color,
                                   bool                   unnormalized_coords,
                                   VkSampler            & sampler,
                                   VkImage              & sampled_image,
                                   VkDeviceMemory       & memory_object,
                                   VkImageView          & sampled_image_view );

  bool CreateStorageImage( VkPhysicalDevice    physical_device,
                           VkDevice            logical_device,
                           VkImageType         type,
                           VkFormat            format,
                           VkExtent3D          size,
                           uint32_t            num_mipmaps,
                           uint32_t            num_layers,
                           VkImageUsageFlags   usage,
                           VkImageViewType     view_type,
                           VkImageAspectFlags  aspect,
                           bool                atomic_operations,
                           VkImage           & storage_image,
                           VkDeviceMemory    & memory_object,
                           VkImageView       & storage_image_view );

  bool CreateUniformTexelBuffer( VkPhysicalDevice     physical_device,
                                 VkDevice             logical_device,
                                 VkFormat             format,
                                 VkDeviceSize         size,
                                 VkImageUsageFlags    usage,
                                 VkBuffer           & uniform_texel_buffer,
                                 VkDeviceMemory     & memory_object,
                                 VkBufferView       & uniform_texel_buffer_view );

  bool CreateStorageTexelBuffer( VkPhysicalDevice     physical_device,
                                 VkDevice             logical_device,
                                 VkFormat             format,
                                 VkDeviceSize         size,
                                 VkBufferUsageFlags   usage,
                                 bool                 atomic_operations,
                                 VkBuffer           & storage_texel_buffer,
                                 VkDeviceMemory     & memory_object,
                                 VkBufferView       & storage_texel_buffer_view );

  bool CreateUniformBuffer( VkPhysicalDevice     physical_device,
                            VkDevice             logical_device,
                            VkDeviceSize         size,
                            VkBufferUsageFlags   usage,
                            VkBuffer           & uniform_buffer,
                            VkDeviceMemory     & memory_object );

  bool CreateStorageBuffer( VkPhysicalDevice     physical_device,
                            VkDevice             logical_device,
                            VkDeviceSize         size,
                            VkBufferUsageFlags   usage,
                            VkBuffer           & storage_buffer,
                            VkDeviceMemory     & memory_object );

  bool CreateInputAttachment( VkPhysicalDevice     physical_device,
                              VkDevice             logical_device,
                              VkImageType          type,
                              VkFormat             format,
                              VkExtent3D           size,
                              VkImageUsageFlags    usage,
                              VkImageViewType      view_type,
                              VkImageAspectFlags   aspect,
                              VkImage            & input_attachment,
                              VkDeviceMemory     & memory_object,
                              VkImageView        & input_attachment_image_view );

  bool CreateDescriptorSetLayout( VkDevice                                          logical_device,
                                  std::vector<VkDescriptorSetLayoutBinding> const & bindings,
                                  VkDescriptorSetLayout                           & descriptor_set_layout );

  bool CreateDescriptorPool( VkDevice                                  logical_device,
                             bool                                      free_individual_sets,
                             uint32_t                                  max_sets_count,
                             std::vector<VkDescriptorPoolSize> const & descriptor_types,
                             VkDescriptorPool                        & descriptor_pool );

  bool AllocateDescriptorSets( VkDevice                                   logical_device,
                               VkDescriptorPool                           descriptor_pool,
                               std::vector<VkDescriptorSetLayout> const & descriptor_set_layouts,
                               std::vector<VkDescriptorSet>             & descriptor_sets );

  struct ImageDescriptorInfo {
    VkDescriptorSet                     TargetDescriptorSet;
    uint32_t                            TargetDescriptorBinding;
    uint32_t                            TargetArrayElement;
    VkDescriptorType                    TargetDescriptorType;
    std::vector<VkDescriptorImageInfo>  ImageInfos;
  };

  struct BufferDescriptorInfo {
    VkDescriptorSet                     TargetDescriptorSet;
    uint32_t                            TargetDescriptorBinding;
    uint32_t                            TargetArrayElement;
    VkDescriptorType                    TargetDescriptorType;
    std::vector<VkDescriptorBufferInfo> BufferInfos;
  };

  struct TexelBufferDescriptorInfo {
    VkDescriptorSet                     TargetDescriptorSet;
    uint32_t                            TargetDescriptorBinding;
    uint32_t                            TargetArrayElement;
    VkDescriptorType                    TargetDescriptorType;
    std::vector<VkBufferView>           TexelBufferViews;
  };

  struct CopyDescriptorInfo {
    VkDescriptorSet     TargetDescriptorSet;
    uint32_t            TargetDescriptorBinding;
    uint32_t            TargetArrayElement;
    VkDescriptorSet     SourceDescriptorSet;
    uint32_t            SourceDescriptorBinding;
    uint32_t            SourceArrayElement;
    uint32_t            DescriptorCount;
  };

  void UpdateDescriptorSets( VkDevice                                       logical_device,
                             std::vector<ImageDescriptorInfo> const       & image_descriptor_infos,
                             std::vector<BufferDescriptorInfo> const      & buffer_descriptor_infos,
                             std::vector<TexelBufferDescriptorInfo> const & texel_buffer_descriptor_infos,
                             std::vector<CopyDescriptorInfo> const        & copy_descriptor_infos );

  void BindDescriptorSets( VkCommandBuffer                      command_buffer,
                           VkPipelineBindPoint                  pipeline_type,
                           VkPipelineLayout                     pipeline_layout,
                           uint32_t                             index_for_first_set,
                           std::vector<VkDescriptorSet> const & descriptor_sets,
                           std::vector<uint32_t> const        & dynamic_offsets );

  bool CreateDescriptorsWithTextureAndUniformBuffer( VkPhysicalDevice               physical_device,
                                                     VkDevice                       logical_device,
                                                     VkExtent3D                     sampled_image_size,
                                                     uint32_t                       uniform_buffer_size,
                                                     VkSampler                    & sampler,
                                                     VkImage                      & sampled_image,
                                                     VkDeviceMemory               & sampled_image_memory_object,
                                                     VkImageView                  & sampled_image_view,
                                                     VkBuffer                     & uniform_buffer,
                                                     VkDeviceMemory               & uniform_buffer_memory_object,
                                                     VkDescriptorSetLayout        & descriptor_set_layout,
                                                     VkDescriptorPool             & descriptor_pool,
                                                     std::vector<VkDescriptorSet> & descriptor_sets );

  bool FreeDescriptorSets( VkDevice                       logical_device,
                           VkDescriptorPool               descriptor_pool,
                           std::vector<VkDescriptorSet> & descriptor_sets );

  bool ResetDescriptorPool( VkDevice          logical_device,
                            VkDescriptorPool  descriptor_pool );

  void DestroyDescriptorPool( VkDevice           logical_device,
                              VkDescriptorPool & descriptor_pool );

  void DestroyDescriptorSetLayout( VkDevice                logical_device,
                                   VkDescriptorSetLayout & descriptor_set_layout );

  void DestroySampler( VkDevice    logical_device,
                       VkSampler & sampler );

} // namespace VulkanCookbook

#endif // DESTROYING_A_SAMPLER
