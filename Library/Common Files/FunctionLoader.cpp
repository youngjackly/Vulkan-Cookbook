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
// Recipe:  05 Loading function exported from a Vulkan Loader library
#ifdef VK_USE_PLATFORM_GLFW_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#include "FunctionLoader.h"

namespace VulkanCookbook {

bool ConnectWithVulkanLoaderLibrary( LIBRARY_TYPE & vulkan_library ) {
#if defined _WIN32
  vulkan_library = LoadLibrary( "vulkan-1.dll" );
#elif defined __linux
#ifndef VK_USE_PLATFORM_GLFW_KHR
  vulkan_library = dlopen( "libvulkan.so.1", RTLD_NOW );
#endif
#endif

#ifdef VK_USE_PLATFORM_GLFW_KHR
  return true;
#else
  if( vulkan_library == nullptr ) {
	std::cout << "Could not connect with a Vulkan Runtime library." << std::endl;
	return false;
  }
#endif
  return true;
}

bool LoadFunctionExportedFromVulkanLoaderLibrary( LIBRARY_TYPE const & vulkan_library )
{
#if defined _WIN32
  #define LoadFunction GetProcAddress
#elif defined __linux
  #define LoadFunction dlsym
#endif

#ifndef VK_USE_PLATFORM_GLFW_KHR
#define EXPORTED_VULKAN_FUNCTION( name )                              \
    name = (PFN_##name)LoadFunction( vulkan_library, #name );         \
    if( name == nullptr ) {                                           \
      std::cout << "Could not load exported Vulkan function named: "  \
        #name << std::endl;                                           \
      return false;                                                   \
    }
#else

#define EXPORTED_VULKAN_FUNCTION( name )                              \
	name = (PFN_##name) glfwGetInstanceProcAddress(nullptr, #name); \
	if( name == nullptr ) {                                           \
      std::cout << "Could not load exported Vulkan function named: "  \
        #name << std::endl;                                           \
		return false;                                                   \
	}
#endif

#include "ListOfVulkanFunctions.inl"

	return true;
  }

bool LoadGlobalLevelFunctions()
{
#define GLOBAL_LEVEL_VULKAN_FUNCTION( name )                              \
name = (PFN_##name)vkGetInstanceProcAddr( nullptr, #name );           \
if( name == nullptr ) {                                               \
std::cout << "Could not load global level Vulkan function named: "  \
#name << std::endl;                                               \
return false;                                                       \
}

#include "ListOfVulkanFunctions.inl"

return true;
}

bool LoadInstanceLevelFunctions( VkInstance                        instance,
								 std::vector<char const *> const & enabled_extensions ) {
  // Load core Vulkan API instance-level functions
#define INSTANCE_LEVEL_VULKAN_FUNCTION( name )                                  \
  name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );                \
  if( name == nullptr ) {                                                     \
	std::cout << "Could not load instance-level Vulkan function named: "      \
	  #name << std::endl;                                                     \
	return false;                                                             \
  }

  // Load instance-level functions from enabled extensions
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )        \
  for( auto & enabled_extension : enabled_extensions ) {                      \
	if( std::string( enabled_extension ) == std::string( extension ) ) {      \
	  name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );            \
	  if( name == nullptr ) {                                                 \
		std::cout << "Could not load instance-level Vulkan function named: "  \
		  #name << std::endl;                                                 \
		return false;                                                         \
	  }                                                                       \
	}                                                                         \
  }

#include "ListOfVulkanFunctions.inl"

  return true;
}

bool LoadDeviceLevelFunctions( VkDevice                          logical_device,
							   std::vector<char const *> const & enabled_extensions ) {
  // Load core Vulkan API device-level functions
#define DEVICE_LEVEL_VULKAN_FUNCTION( name )                                    \
  name = (PFN_##name)vkGetDeviceProcAddr( logical_device, #name );            \
  if( name == nullptr ) {                                                     \
	std::cout << "Could not load device-level Vulkan function named: "        \
	  #name << std::endl;                                                     \
	return false;                                                             \
  }

  // Load device-level functions from enabled extensions
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )          \
  for( auto & enabled_extension : enabled_extensions ) {                      \
	if( std::string( enabled_extension ) == std::string( extension ) ) {      \
	  name = (PFN_##name)vkGetDeviceProcAddr( logical_device, #name );        \
	  if( name == nullptr ) {                                                 \
		std::cout << "Could not load device-level Vulkan function named: "    \
		  #name << std::endl;                                                 \
		return false;                                                         \
	  }                                                                       \
	}                                                                         \
  }

#include "ListOfVulkanFunctions.inl"

  return true;
}

void ReleaseVulkanLoaderLibrary( LIBRARY_TYPE & vulkan_library ) {
	if( nullptr != vulkan_library ) {
#if defined _WIN32
		FreeLibrary( vulkan_library );
#elif defined __linux
		dlclose( vulkan_library );
#endif
		vulkan_library = nullptr;
	}
}

} // namespace VulkanCookbook

