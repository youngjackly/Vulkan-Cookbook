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
// Chapter: 08 Graphics and Compute Pipelines
// Recipe:  14 Creating a pipeline cache object

#include "08 Graphics and Compute Pipelines/14 Creating a pipeline cache object.h"

namespace VulkanCookbook {

  bool CreatePipelineCacheObject( VkDevice                           logical_device,
                                  std::vector<unsigned char> const & cache_data,
                                  VkPipelineCache                  & pipeline_cache ) {
    VkPipelineCacheCreateInfo pipeline_cache_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,     // VkStructureType                sType
      nullptr,                                          // const void                   * pNext
      0,                                                // VkPipelineCacheCreateFlags     flags
      static_cast<uint32_t>(cache_data.size()),         // size_t                         initialDataSize
      cache_data.data()                                 // const void                   * pInitialData
    };

    VkResult result = vkCreatePipelineCache( logical_device, &pipeline_cache_create_info, nullptr, &pipeline_cache );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create pipeline cache." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
