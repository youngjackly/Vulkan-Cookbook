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
// Recipe:  18 Creating a compute pipeline

#ifndef CREATING_A_COMPUTE_PIPELINE
#define CREATING_A_COMPUTE_PIPELINE

#include "Common.h"

namespace VulkanCookbook {

  bool CreateComputePipeline( VkDevice                                logical_device,
                              VkPipelineCreateFlags                   additional_options,
                              VkPipelineShaderStageCreateInfo const & compute_shader_stage,
                              VkPipelineLayout                        pipeline_layout,
                              VkPipeline                              base_pipeline_handle,
                              VkPipelineCache                         pipeline_cache,
                              VkPipeline                            & compute_pipeline );

} // namespace VulkanCookbook

#endif // CREATING_A_COMPUTE_PIPELINE