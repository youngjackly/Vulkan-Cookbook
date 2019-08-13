#pragma once

#include "Common.h"

namespace VulkanCookbook {

bool CheckAvailableInstanceExtensions( std::vector<VkExtensionProperties> & available_extensions );

bool CreateVulkanInstance( std::vector<char const *> const & desired_extensions,
char const * const  application_name, VkInstance & instance);

bool EnumerateAvailablePhysicalDevices( VkInstance                      instance,
										std::vector<VkPhysicalDevice> & available_devices );

bool CheckAvailableDeviceExtensions( VkPhysicalDevice                     physical_device,
									 std::vector<VkExtensionProperties> & available_extensions );

void DestroyVulkanInstance( VkInstance & instance );

void GetFeaturesAndPropertiesOfPhysicalDevice( VkPhysicalDevice             physical_device,
											   VkPhysicalDeviceFeatures   & device_features,
											   VkPhysicalDeviceProperties & device_properties );

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
// Chapter: 01 Instance and Devices
// Recipe:  13 Checking available queue families and their properties

#ifndef Checking_available_queue_families_and_their_properties
#define CHECKING_AVAILABLE_QUEUE_FAMILIES_AND_THEIR_PROPERTIES

#include "Common.h"

namespace VulkanCookbook {

  bool CheckAvailableQueueFamiliesAndTheirProperties( VkPhysicalDevice                       physical_device,
                                                      std::vector<VkQueueFamilyProperties> & queue_families );

} // namespace VulkanCookbook

#endif // CHECKING_AVAILABLE_QUEUE_FAMILIES_AND_THEIR_PROPERTIES// Permission is hereby granted, free of charge, to any person obtaining a
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
// Chapter: 01 Instance and Devices
// Recipe:  14 Selecting index of a queue family with desired capabilities

#ifndef SELECTING_INDEX_OF_A_QUEUE_FAMILY_WITH_DESIRED_CAPABILITIES
#define SELECTING_INDEX_OF_A_QUEUE_FAMILY_WITH_DESIRED_CAPABILITIES

#include "Common.h"

namespace VulkanCookbook {

  bool SelectIndexOfQueueFamilyWithDesiredCapabilities( VkPhysicalDevice   physical_device,
                                                        VkQueueFlags       desired_capabilities,
                                                        uint32_t         & queue_family_index );

} // namespace VulkanCookbook

#endif // SELECTING_INDEX_OF_A_QUEUE_FAMILY_WITH_DESIRED_CAPABILITIES// Permission is hereby granted, free of charge, to any person obtaining a
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
// Chapter: 01 Instance and Devices
// Recipe:  15 Creating a logical device

#ifndef CREATING_A_LOGICAL_DEVICE
#define CREATING_A_LOGICAL_DEVICE

#include "Common.h"

namespace VulkanCookbook {

  struct QueueInfo {
    uint32_t           FamilyIndex;
    std::vector<float> Priorities;
  };

  bool CreateLogicalDevice( VkPhysicalDevice                  physical_device,
                            std::vector< QueueInfo >          queue_infos,
                            std::vector<char const *> const & desired_extensions,
                            VkPhysicalDeviceFeatures        * desired_features,
                            VkDevice                        & logical_device );

} // namespace VulkanCookbook

#endif // CREATING_A_LOGICAL_DEVICE// Permission is hereby granted, free of charge, to any person obtaining a
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
// Chapter: 01 Instance and Devices
// Recipe:  17 Getting a device queue

#ifndef GETTING_A_DEVICE_QUEUE
#define GETTING_A_DEVICE_QUEUE

#include "Common.h"

namespace VulkanCookbook {

  void GetDeviceQueue( VkDevice logical_device, uint32_t queue_family_index, uint32_t queue_index, VkQueue & queue );

} // namespace VulkanCookbook

#endif // GETTING_A_DEVICE_QUEUE// Permission is hereby granted, free of charge, to any person obtaining a
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
// Chapter: 01 Instance and Devices
// Recipe:  18 Creating a logical device with geometry shaders and graphics and compute queues

#ifndef CREATING_A_LOGICAL_DEVICE_WITH_GEOMETRY_SHADERS_AND_GRAPHICS_AND_COMPUTE_QUEUES
#define CREATING_A_LOGICAL_DEVICE_WITH_GEOMETRY_SHADERS_AND_GRAPHICS_AND_COMPUTE_QUEUES

#include "Common.h"

namespace VulkanCookbook {

  bool CreateLogicalDeviceWithGeometryShadersAndGraphicsAndComputeQueues( VkInstance   instance,
                                                                          VkDevice   & logical_device,
                                                                          VkQueue    & graphics_queue,
                                                                          VkQueue    & compute_queue );

} // namespace VulkanCookbook

#endif // CREATING_A_LOGICAL_DEVICE_WITH_GEOMETRY_SHADERS_AND_GRAPHICS_AND_COMPUTE_QUEUES// Permission is hereby granted, free of charge, to any person obtaining a
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
// Chapter: 01 Instance and Devices
// Recipe:  19 Destroying a logical device

#ifndef DESTROYING_A_LOGICAL_DEVICE
#define DESTROYING_A_LOGICAL_DEVICE

#include "Common.h"

namespace VulkanCookbook {

  void DestroyLogicalDevice( VkDevice & logical_device );

} // namespace VulkanCookbook

#endif // DESTROYING_A_LOGICAL_DEVICE