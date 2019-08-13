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

#include "Chapter5Functions.h"

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
                      VkSampler            & sampler ) {
    VkSamplerCreateInfo sampler_create_info = {
      VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,    // VkStructureType          sType
      nullptr,                                  // const void             * pNext
      0,                                        // VkSamplerCreateFlags     flags
      mag_filter,                               // VkFilter                 magFilter
      min_filter,                               // VkFilter                 minFilter
      mipmap_mode,                              // VkSamplerMipmapMode      mipmapMode
      u_address_mode,                           // VkSamplerAddressMode     addressModeU
      v_address_mode,                           // VkSamplerAddressMode     addressModeV
      w_address_mode,                           // VkSamplerAddressMode     addressModeW
      lod_bias,                                 // float                    mipLodBias
      anisotropy_enable,                        // VkBool32                 anisotropyEnable
      max_anisotropy,                           // float                    maxAnisotropy
      compare_enable,                           // VkBool32                 compareEnable
      compare_operator,                         // VkCompareOp              compareOp
      min_lod,                                  // float                    minLod
      max_lod,                                  // float                    maxLod
      border_color,                             // VkBorderColor            borderColor
      unnormalized_coords                       // VkBool32                 unnormalizedCoordinates
    };

    VkResult result = vkCreateSampler( logical_device, &sampler_create_info, nullptr, &sampler );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create sampler." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  02 Creating a sampled image

#include "Chapter4Functions.h"

namespace VulkanCookbook {

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
                           VkImageView       & sampled_image_view ) {
    VkFormatProperties format_properties;
    vkGetPhysicalDeviceFormatProperties( physical_device, format, &format_properties );
    if( !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) ) {
      std::cout << "Provided format is not supported for a sampled image." << std::endl;
      return false;
    }
    if( linear_filtering &&
        !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) ) {
      std::cout << "Provided format is not supported for a linear image filtering." << std::endl;
      return false;
    }

    if( !CreateImage( logical_device, type, format, size, num_mipmaps, num_layers, VK_SAMPLE_COUNT_1_BIT, usage | VK_IMAGE_USAGE_SAMPLED_BIT, cubemap, sampled_image ) ) {
      return false;
    }

    if( !AllocateAndBindMemoryObjectToImage( physical_device, logical_device, sampled_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory_object ) ) {
      return false;
    }

    if( !CreateImageView( logical_device, sampled_image, view_type, format, aspect, sampled_image_view ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  03 Creating a combined image sampler

namespace VulkanCookbook {

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
                                   VkImageView          & sampled_image_view ) {
    if( !CreateSampler( logical_device, mag_filter, min_filter, mipmap_mode, u_address_mode, v_address_mode, w_address_mode, lod_bias, anisotropy_enable, max_anisotropy, compare_enable, compare_operator, min_lod, max_lod, border_color, unnormalized_coords, sampler ) ) {
      return false;
    }

    bool linear_filtering = (mag_filter == VK_FILTER_LINEAR) || (min_filter == VK_FILTER_LINEAR) || (mipmap_mode == VK_SAMPLER_MIPMAP_MODE_LINEAR);
    if( !CreateSampledImage( physical_device, logical_device, type, format, size, num_mipmaps, num_layers, usage, cubemap, view_type, aspect, linear_filtering, sampled_image, memory_object, sampled_image_view ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  04 Creating a storage image


namespace VulkanCookbook {

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
                           VkImageView       & storage_image_view ) {
    VkFormatProperties format_properties;
    vkGetPhysicalDeviceFormatProperties( physical_device, format, &format_properties );
    if( !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT) ) {
      std::cout << "Provided format is not supported for a storage image." << std::endl;
      return false;
    }
    if( atomic_operations &&
        !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT) ) {
      std::cout << "Provided format is not supported for atomic operations on storage images." << std::endl;
      return false;
    }

    if( !CreateImage( logical_device, type, format, size, num_mipmaps, num_layers, VK_SAMPLE_COUNT_1_BIT, usage | VK_IMAGE_USAGE_STORAGE_BIT, false, storage_image ) ) {
      return false;
    }

    if( !AllocateAndBindMemoryObjectToImage( physical_device, logical_device, storage_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory_object ) ) {
      return false;
    }

    if( !CreateImageView( logical_device, storage_image, view_type, format, aspect, storage_image_view ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  05 Creating a uniform texel buffer


namespace VulkanCookbook {

  bool CreateUniformTexelBuffer( VkPhysicalDevice     physical_device,
                                 VkDevice             logical_device,
                                 VkFormat             format,
                                 VkDeviceSize         size,
                                 VkImageUsageFlags    usage,
                                 VkBuffer           & uniform_texel_buffer,
                                 VkDeviceMemory     & memory_object,
                                 VkBufferView       & uniform_texel_buffer_view ) {
    VkFormatProperties format_properties;
    vkGetPhysicalDeviceFormatProperties( physical_device, format, &format_properties );
    if( !(format_properties.bufferFeatures & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT) ) {
      std::cout << "Provided format is not supported for a uniform texel buffer." << std::endl;
      return false;
    }

    if( !CreateBuffer( logical_device, size, usage | VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT, uniform_texel_buffer ) ) {
      return false;
    }

    if( !AllocateAndBindMemoryObjectToBuffer( physical_device, logical_device, uniform_texel_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory_object ) ) {
      return false;
    }

    if( !CreateBufferView( logical_device, uniform_texel_buffer, format, 0, VK_WHOLE_SIZE, uniform_texel_buffer_view ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  06 Creating a storage texel buffer


namespace VulkanCookbook {

  bool CreateStorageTexelBuffer( VkPhysicalDevice     physical_device,
                                 VkDevice             logical_device,
                                 VkFormat             format,
                                 VkDeviceSize         size,
                                 VkBufferUsageFlags   usage,
                                 bool                 atomic_operations,
                                 VkBuffer           & storage_texel_buffer,
                                 VkDeviceMemory     & memory_object,
                                 VkBufferView       & storage_texel_buffer_view ) {
    VkFormatProperties format_properties;
    vkGetPhysicalDeviceFormatProperties( physical_device, format, &format_properties );
    if( !(format_properties.bufferFeatures & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT) ) {
      std::cout << "Provided format is not supported for a uniform texel buffer." << std::endl;
      return false;
    }

    if( atomic_operations &&
        !(format_properties.bufferFeatures & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT) ) {
      std::cout << "Provided format is not supported for atomic operations on storage texel buffers." << std::endl;
      return false;
    }

    if( !CreateBuffer( logical_device, size, usage | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, storage_texel_buffer ) ) {
      return false;
    }

    if( !AllocateAndBindMemoryObjectToBuffer( physical_device, logical_device, storage_texel_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory_object ) ) {
      return false;
    }

    if( !CreateBufferView( logical_device, storage_texel_buffer, format, 0, VK_WHOLE_SIZE, storage_texel_buffer_view ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  07 Creating a uniform buffer


namespace VulkanCookbook {

  bool CreateUniformBuffer( VkPhysicalDevice     physical_device,
                            VkDevice             logical_device,
                            VkDeviceSize         size,
                            VkBufferUsageFlags   usage,
                            VkBuffer           & uniform_buffer,
                            VkDeviceMemory     & memory_object ) {
    if( !CreateBuffer( logical_device, size, usage | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, uniform_buffer ) ) {
      return false;
    }

    if( !AllocateAndBindMemoryObjectToBuffer( physical_device, logical_device, uniform_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory_object ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  08 Creating a storage buffer


namespace VulkanCookbook {

  bool CreateStorageBuffer( VkPhysicalDevice     physical_device,
                            VkDevice             logical_device,
                            VkDeviceSize         size,
                            VkBufferUsageFlags   usage,
                            VkBuffer           & storage_buffer,
                            VkDeviceMemory     & memory_object ) {
    if( !CreateBuffer( logical_device, size, usage | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, storage_buffer ) ) {
      return false;
    }

    if( !AllocateAndBindMemoryObjectToBuffer( physical_device, logical_device, storage_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory_object ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  09 Creating an input attachment


namespace VulkanCookbook {

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
                              VkImageView        & input_attachment_image_view ) {
    VkFormatProperties format_properties;
    vkGetPhysicalDeviceFormatProperties( physical_device, format, &format_properties );
    if( (aspect & VK_IMAGE_ASPECT_COLOR_BIT) &&
        !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT) ) {
      std::cout << "Provided format is not supported for an input attachment." << std::endl;
      return false;
    }
    if( (aspect & (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT)) &&
      !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) ) {
      std::cout << "Provided format is not supported for an input attachment." << std::endl;
      return false;
    }

    if( !CreateImage( logical_device, type, format, size, 1, 1, VK_SAMPLE_COUNT_1_BIT, usage | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, false, input_attachment ) ) {
      return false;
    }

    if( !AllocateAndBindMemoryObjectToImage( physical_device, logical_device, input_attachment, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory_object ) ) {
      return false;
    }

    if( !CreateImageView( logical_device, input_attachment, view_type, format, aspect, input_attachment_image_view ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  10 Creating a descriptor set layout


namespace VulkanCookbook {

  bool CreateDescriptorSetLayout( VkDevice                                          logical_device,
                                  std::vector<VkDescriptorSetLayoutBinding> const & bindings,
                                  VkDescriptorSetLayout                           & descriptor_set_layout ) {
    VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,  // VkStructureType                      sType
      nullptr,                                              // const void                         * pNext
      0,                                                    // VkDescriptorSetLayoutCreateFlags     flags
      static_cast<uint32_t>(bindings.size()),               // uint32_t                             bindingCount
      bindings.data()                                       // const VkDescriptorSetLayoutBinding * pBindings
    };

    VkResult result = vkCreateDescriptorSetLayout( logical_device, &descriptor_set_layout_create_info, nullptr, &descriptor_set_layout );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create a layout for descriptor sets." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  11 Creating a descriptor pool


namespace VulkanCookbook {

  bool CreateDescriptorPool( VkDevice                                  logical_device,
                             bool                                      free_individual_sets,
                             uint32_t                                  max_sets_count,
                             std::vector<VkDescriptorPoolSize> const & descriptor_types,
                             VkDescriptorPool                        & descriptor_pool ) {
    VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,                // VkStructureType                sType
      nullptr,                                                      // const void                   * pNext
      free_individual_sets ?                                        // VkDescriptorPoolCreateFlags    flags
        VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT : 0u,
      max_sets_count,                                               // uint32_t                       maxSets
      static_cast<uint32_t>(descriptor_types.size()),               // uint32_t                       poolSizeCount
      descriptor_types.data()                                       // const VkDescriptorPoolSize   * pPoolSizes
    };

    VkResult result = vkCreateDescriptorPool( logical_device, &descriptor_pool_create_info, nullptr, &descriptor_pool );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create a descriptor pool." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  12 Allocating descriptor sets


namespace VulkanCookbook {

  bool AllocateDescriptorSets( VkDevice                                   logical_device,
                               VkDescriptorPool                           descriptor_pool,
                               std::vector<VkDescriptorSetLayout> const & descriptor_set_layouts,
                               std::vector<VkDescriptorSet>             & descriptor_sets ) {
    if( descriptor_set_layouts.size() > 0 ) {
      VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,         // VkStructureType                  sType
        nullptr,                                                // const void                     * pNext
        descriptor_pool,                                        // VkDescriptorPool                 descriptorPool
        static_cast<uint32_t>(descriptor_set_layouts.size()),   // uint32_t                         descriptorSetCount
        descriptor_set_layouts.data()                           // const VkDescriptorSetLayout    * pSetLayouts
      };

      descriptor_sets.resize( descriptor_set_layouts.size() );

      VkResult result = vkAllocateDescriptorSets( logical_device, &descriptor_set_allocate_info, descriptor_sets.data() );
      if( VK_SUCCESS != result ) {
        std::cout << "Could not allocate descriptor sets." << std::endl;
        return false;
      }
      return true;
    }
    return false;
  }

} // namespace VulkanCookbook
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
// Recipe:  13 Updating descriptor sets


namespace VulkanCookbook {

  void UpdateDescriptorSets( VkDevice                                       logical_device,
                             std::vector<ImageDescriptorInfo> const       & image_descriptor_infos,
                             std::vector<BufferDescriptorInfo> const      & buffer_descriptor_infos,
                             std::vector<TexelBufferDescriptorInfo> const & texel_buffer_descriptor_infos,
                             std::vector<CopyDescriptorInfo> const        & copy_descriptor_infos ) {
    std::vector<VkWriteDescriptorSet> write_descriptors;
    std::vector<VkCopyDescriptorSet> copy_descriptors;

    // image descriptors
    for( auto & image_descriptor : image_descriptor_infos ) {
      write_descriptors.push_back( {
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                 // VkStructureType                  sType
        nullptr,                                                                // const void                     * pNext
        image_descriptor.TargetDescriptorSet,                                   // VkDescriptorSet                  dstSet
        image_descriptor.TargetDescriptorBinding,                               // uint32_t                         dstBinding
        image_descriptor.TargetArrayElement,                                    // uint32_t                         dstArrayElement
        static_cast<uint32_t>(image_descriptor.ImageInfos.size()),              // uint32_t                         descriptorCount
        image_descriptor.TargetDescriptorType,                                  // VkDescriptorType                 descriptorType
        image_descriptor.ImageInfos.data(),                                     // const VkDescriptorImageInfo    * pImageInfo
        nullptr,                                                                // const VkDescriptorBufferInfo   * pBufferInfo
        nullptr                                                                 // const VkBufferView             * pTexelBufferView
      } );
    }

    // buffer descriptors
    for( auto & buffer_descriptor : buffer_descriptor_infos ) {
      write_descriptors.push_back( {
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                 // VkStructureType                  sType
        nullptr,                                                                // const void                     * pNext
        buffer_descriptor.TargetDescriptorSet,                                  // VkDescriptorSet                  dstSet
        buffer_descriptor.TargetDescriptorBinding,                              // uint32_t                         dstBinding
        buffer_descriptor.TargetArrayElement,                                   // uint32_t                         dstArrayElement
        static_cast<uint32_t>(buffer_descriptor.BufferInfos.size()),            // uint32_t                         descriptorCount
        buffer_descriptor.TargetDescriptorType,                                 // VkDescriptorType                 descriptorType
        nullptr,                                                                // const VkDescriptorImageInfo    * pImageInfo
        buffer_descriptor.BufferInfos.data(),                                   // const VkDescriptorBufferInfo   * pBufferInfo
        nullptr                                                                 // const VkBufferView             * pTexelBufferView
      } );
    }

    // texel buffer descriptors
    for( auto & texel_buffer_descriptor : texel_buffer_descriptor_infos ) {
      write_descriptors.push_back( {
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                 // VkStructureType                  sType
        nullptr,                                                                // const void                     * pNext
        texel_buffer_descriptor.TargetDescriptorSet,                            // VkDescriptorSet                  dstSet
        texel_buffer_descriptor.TargetDescriptorBinding,                        // uint32_t                         dstBinding
        texel_buffer_descriptor.TargetArrayElement,                             // uint32_t                         dstArrayElement
        static_cast<uint32_t>(texel_buffer_descriptor.TexelBufferViews.size()), // uint32_t                         descriptorCount
        texel_buffer_descriptor.TargetDescriptorType,                           // VkDescriptorType                 descriptorType
        nullptr,                                                                // const VkDescriptorImageInfo    * pImageInfo
        nullptr,                                                                // const VkDescriptorBufferInfo   * pBufferInfo
        texel_buffer_descriptor.TexelBufferViews.data()                         // const VkBufferView             * pTexelBufferView
      } );
    }

    // copy descriptors
    for( auto & copy_descriptor : copy_descriptor_infos ) {
      copy_descriptors.push_back( {
        VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET,                                  // VkStructureType    sType
        nullptr,                                                                // const void       * pNext
        copy_descriptor.SourceDescriptorSet,                                    // VkDescriptorSet    srcSet
        copy_descriptor.SourceDescriptorBinding,                                // uint32_t           srcBinding
        copy_descriptor.SourceArrayElement,                                     // uint32_t           srcArrayElement
        copy_descriptor.TargetDescriptorSet,                                    // VkDescriptorSet    dstSet
        copy_descriptor.TargetDescriptorBinding,                                // uint32_t           dstBinding
        copy_descriptor.TargetArrayElement,                                     // uint32_t           dstArrayElement
        copy_descriptor.DescriptorCount                                         // uint32_t           descriptorCount
      } );
    }

    vkUpdateDescriptorSets( logical_device, static_cast<uint32_t>(write_descriptors.size()), write_descriptors.data(), static_cast<uint32_t>(copy_descriptors.size()), copy_descriptors.data() );
  }

} // namespace VulkanCookbook
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
// Recipe:  14 Binding descriptor sets


namespace VulkanCookbook {

  void BindDescriptorSets( VkCommandBuffer                      command_buffer,
                           VkPipelineBindPoint                  pipeline_type,
                           VkPipelineLayout                     pipeline_layout,
                           uint32_t                             index_for_first_set,
                           std::vector<VkDescriptorSet> const & descriptor_sets,
                           std::vector<uint32_t> const        & dynamic_offsets ) {
    vkCmdBindDescriptorSets( command_buffer, pipeline_type, pipeline_layout, index_for_first_set,
      static_cast<uint32_t>(descriptor_sets.size()), descriptor_sets.data(),
      static_cast<uint32_t>(dynamic_offsets.size()), dynamic_offsets.data() );
  }

} // namespace VulkanCookbook
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
// Recipe:  15 Creating descriptors with a texture and a uniform buffer


namespace VulkanCookbook {

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
                                                     std::vector<VkDescriptorSet> & descriptor_sets ) {

    if( !CreateCombinedImageSampler( physical_device, logical_device, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM, sampled_image_size, 1, 1, VK_IMAGE_USAGE_TRANSFER_DST_BIT,
      false, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT,
      VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0.0f, false, 1.0f, false, VK_COMPARE_OP_ALWAYS, 0.0f, 0.0f, VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK, false,
      sampler, sampled_image, sampled_image_memory_object, sampled_image_view ) ) {
      return false;
    }

    if( !CreateUniformBuffer( physical_device, logical_device, uniform_buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, uniform_buffer, uniform_buffer_memory_object ) ) {
      return false;
    }

    std::vector<VkDescriptorSetLayoutBinding> bindings = {
      {
        0,                                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,                  // VkDescriptorType     descriptorType
        1,                                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_FRAGMENT_BIT,                               // VkShaderStageFlags   stageFlags
        nullptr                                                     // const VkSampler    * pImmutableSamplers
      },
      {
        1,                                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                          // VkDescriptorType     descriptorType
        1,                                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,  // VkShaderStageFlags   stageFlags
        nullptr                                                     // const VkSampler    * pImmutableSamplers
      }
    };
    if( !CreateDescriptorSetLayout( logical_device, bindings, descriptor_set_layout ) ) {
      return false;
    }

    std::vector<VkDescriptorPoolSize> descriptor_types = {
      {
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,                  // VkDescriptorType     type
        1                                                           // uint32_t             descriptorCount
      },
      {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                          // VkDescriptorType     type
        1                                                           // uint32_t             descriptorCount
      }
    };
    if( !CreateDescriptorPool( logical_device, false, 1, descriptor_types, descriptor_pool ) ) {
      return false;
    }

    if( !AllocateDescriptorSets( logical_device, descriptor_pool, { descriptor_set_layout }, descriptor_sets ) ) {
      return false;
    }

    std::vector<ImageDescriptorInfo> image_descriptor_infos = {
      {
        descriptor_sets[0],                         // VkDescriptorSet                      TargetDescriptorSet
        0,                                          // uint32_t                             TargetDescriptorBinding
        0,                                          // uint32_t                             TargetArrayElement
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // VkDescriptorType                     TargetDescriptorType
        {                                           // std::vector<VkDescriptorImageInfo>   ImageInfos
          {
            sampler,                                  // VkSampler                            sampler
            sampled_image_view,                       // VkImageView                          imageView
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL  // VkImageLayout                        imageLayout
          }
        }
      }
    };

    std::vector<BufferDescriptorInfo> buffer_descriptor_infos = {
      {
        descriptor_sets[0],                         // VkDescriptorSet                      TargetDescriptorSet
        1,                                          // uint32_t                             TargetDescriptorBinding
        0,                                          // uint32_t                             TargetArrayElement
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType                     TargetDescriptorType
        {                                           // std::vector<VkDescriptorBufferInfo>  BufferInfos
          {
            uniform_buffer,                           // VkBuffer                             buffer
            0,                                        // VkDeviceSize                         offset
            VK_WHOLE_SIZE                             // VkDeviceSize                         range
          }
        }
      }
    };

    UpdateDescriptorSets( logical_device, image_descriptor_infos, buffer_descriptor_infos, {}, {} );
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  16 Freeing descriptor sets


namespace VulkanCookbook {

  bool FreeDescriptorSets( VkDevice                       logical_device,
                           VkDescriptorPool               descriptor_pool,
                           std::vector<VkDescriptorSet> & descriptor_sets ) {
    if( descriptor_sets.size() > 0 ) {
      VkResult result = vkFreeDescriptorSets( logical_device, descriptor_pool, static_cast<uint32_t>(descriptor_sets.size()), descriptor_sets.data() );
      descriptor_sets.clear();
      if( VK_SUCCESS != result ) {
        std::cout << "Error occurred during freeing descriptor sets." << std::endl;
        return false;
      }
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  17 Resetting a descriptor pool

namespace VulkanCookbook {

  bool ResetDescriptorPool( VkDevice          logical_device,
                            VkDescriptorPool  descriptor_pool ) {
    VkResult result = vkResetDescriptorPool( logical_device, descriptor_pool, 0 );
    if( VK_SUCCESS != result ) {
      std::cout << "Error occurred during descriptor pool reset." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
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
// Recipe:  18 Destroying a descriptor pool


namespace VulkanCookbook {

  void DestroyDescriptorPool( VkDevice           logical_device,
                              VkDescriptorPool & descriptor_pool ) {
    if( VK_NULL_HANDLE != descriptor_pool ) {
      vkDestroyDescriptorPool( logical_device, descriptor_pool, nullptr );
      descriptor_pool = VK_NULL_HANDLE;
    }
  }

} // namespace VulkanCookbook
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
// Recipe:  19 Destroying a descriptor set layout


namespace VulkanCookbook {

  void DestroyDescriptorSetLayout( VkDevice                logical_device,
                                   VkDescriptorSetLayout & descriptor_set_layout ) {
    if( VK_NULL_HANDLE != descriptor_set_layout ) {
      vkDestroyDescriptorSetLayout( logical_device, descriptor_set_layout, nullptr );
      descriptor_set_layout = VK_NULL_HANDLE;
    }
  }

} // namespace VulkanCookbook
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
// Recipe:  20 Destroying a sampler


namespace VulkanCookbook {

  void DestroySampler( VkDevice    logical_device,
                       VkSampler & sampler ) {
    if( VK_NULL_HANDLE != sampler ) {
      vkDestroySampler( logical_device, sampler, nullptr );
      sampler = VK_NULL_HANDLE;
    }
  }

} // namespace VulkanCookbook
