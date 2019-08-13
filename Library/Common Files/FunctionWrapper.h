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

  bool CheckAvailableQueueFamiliesAndTheirProperties( VkPhysicalDevice                       physical_device,
                                                      std::vector<VkQueueFamilyProperties> & queue_families );

  bool SelectIndexOfQueueFamilyWithDesiredCapabilities( VkPhysicalDevice   physical_device,
                                                        VkQueueFlags       desired_capabilities,
                                                        uint32_t         & queue_family_index );

  struct QueueInfo {
    uint32_t           FamilyIndex;
    std::vector<float> Priorities;
  };

  bool CreateLogicalDevice( VkPhysicalDevice                  physical_device,
                            std::vector< QueueInfo >          queue_infos,
                            std::vector<char const *> const & desired_extensions,
                            VkPhysicalDeviceFeatures        * desired_features,
                            VkDevice                        & logical_device );

  void GetDeviceQueue( VkDevice logical_device, uint32_t queue_family_index, uint32_t queue_index, VkQueue & queue );


  bool CreateLogicalDeviceWithGeometryShadersAndGraphicsAndComputeQueues( VkInstance   instance,
                                                                          VkDevice   & logical_device,
                                                                          VkQueue    & graphics_queue,
                                                                          VkQueue    & compute_queue );

  void DestroyLogicalDevice( VkDevice & logical_device );

} // namespace VulkanCookbook
