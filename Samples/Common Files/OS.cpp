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
// OS
#ifdef VK_USE_PLATFORM_GLFW_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <assert.h>
#include <unistd.h>
#endif

#include "CookbookSampleFramework.h"

#include <chrono>

/* graphics contexts */


namespace VulkanCookbook {

#ifdef VK_USE_PLATFORM_GLFW_KHR

VulkanCookbookSampleBase* g_sampler = nullptr;

static void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
 {
	if(g_sampler)
		g_sampler->MouseMove(int(x), int(y));
 }

static void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
{
	int index = 0;
	if(button == GLFW_MOUSE_BUTTON_LEFT) index = 0;
	else if(button == GLFW_MOUSE_BUTTON_RIGHT) index = 1;
	else if(button == GLFW_MOUSE_BUTTON_MIDDLE) index = 2;

    if(GLFW_MOUSE_BUTTON_LEFT == button)
    {
        if(GLFW_PRESS == action)
        {
			g_sampler->MouseClick(index, true);
        }
        else if(GLFW_RELEASE == action)
        {
			g_sampler->MouseClick(index, false);
        }
    }
}

void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
{
	g_sampler->MouseWheel(y);
}

WindowFramework::WindowFramework( const char               * window_title,
								  int                        x,
								  int                        y,
								  int                        width,
								  int                        height,
								  VulkanCookbookSampleBase & sample ) :
  WindowParams(),
  Sample( sample ),
  Created( false )
{

	if (!glfwInit())
        assert(false);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);

	if(!window) assert(false);

	glfwSetMouseButtonCallback(window, onGLFWMouseCallBack);
	glfwSetCursorPosCallback(window, onGLFWMouseMoveCallBack);
	glfwSetScrollCallback(window, onGLFWMouseScrollCallback);


	WindowParams.window = window;
	Created = true;
	g_sampler = &sample;

}

WindowFramework::~WindowFramework() {

}

void WindowFramework::Render() {
	if( Created && Sample.Initialize( WindowParams ) )
	{
		typedef std::chrono::system_clock::time_point tp;
		typedef std::chrono::microseconds mms;
		tp start;
		long spare;
		long dur;
		tp end;
		double frameRate = 60.;
		while (!glfwWindowShouldClose(WindowParams.window))
		{
			start = std::chrono::system_clock::now();
			glfwPollEvents();
			if( Sample.IsReady() ) {
				Sample.UpdateTime();
				Sample.Draw();
				Sample.MouseReset();
			}

			end = std::chrono::system_clock::now();
			dur =  std::chrono::duration_cast<mms>(end - start).count();

			spare = 1000000/90 - dur;
			if(spare > 0)
			{
				usleep(spare);
				dur += spare;
			}

			frameRate = 1000000/(double)dur;
			printf("fps = %0.3f\n", frameRate);
		}
	}
	Sample.Deinitialize();
	glfwDestroyWindow(WindowParams.window);
}

#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR

  namespace {
    enum UserMessage {
      USER_MESSAGE_RESIZE = WM_USER + 1,
      USER_MESSAGE_QUIT,
      USER_MESSAGE_MOUSE_CLICK,
      USER_MESSAGE_MOUSE_MOVE,
      USER_MESSAGE_MOUSE_WHEEL
    };
  }

  LRESULT CALLBACK WindowProcedure( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {
    switch( message ) {
    case WM_LBUTTONDOWN:
      PostMessage( hWnd, USER_MESSAGE_MOUSE_CLICK, 0, 1 );
      break;
    case WM_LBUTTONUP:
      PostMessage( hWnd, USER_MESSAGE_MOUSE_CLICK, 0, 0 );
      break;
    case WM_RBUTTONDOWN:
      PostMessage( hWnd, USER_MESSAGE_MOUSE_CLICK, 1, 1 );
      break;
    case WM_RBUTTONUP:
      PostMessage( hWnd, USER_MESSAGE_MOUSE_CLICK, 1, 0 );
      break;
    case WM_MOUSEMOVE:
      PostMessage( hWnd, USER_MESSAGE_MOUSE_MOVE, LOWORD( lParam ), HIWORD( lParam ) );
      break;
    case WM_MOUSEWHEEL:
      PostMessage( hWnd, USER_MESSAGE_MOUSE_WHEEL, HIWORD( wParam ), 0 );
      break;
    case WM_SIZE:
    case WM_EXITSIZEMOVE:
      PostMessage( hWnd, USER_MESSAGE_RESIZE, wParam, lParam );
      break;
    case WM_KEYDOWN:
      if( VK_ESCAPE == wParam ) {
        PostMessage( hWnd, USER_MESSAGE_QUIT, wParam, lParam );
      }
      break;
    case WM_CLOSE:
      PostMessage( hWnd, USER_MESSAGE_QUIT, wParam, lParam );
      break;
    default:
      return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
  }

  WindowFramework::WindowFramework( const char               * window_title,
                                    int                        x,
                                    int                        y,
                                    int                        width,
                                    int                        height,
                                    VulkanCookbookSampleBase & sample ) :
    WindowParams(),
    Sample( sample ),
    Created( false ) {
    WindowParams.HInstance = GetModuleHandle( nullptr );

    WNDCLASSEX window_class = {
      sizeof( WNDCLASSEX ),             // UINT         cbSize
                                        /* Win 3.x */
      CS_HREDRAW | CS_VREDRAW,          // UINT         style
      WindowProcedure,                  // WNDPROC      lpfnWndProc
      0,                                // int          cbClsExtra
      0,                                // int          cbWndExtra
      WindowParams.HInstance,           // HINSTANCE    hInstance
      nullptr,                          // HICON        hIcon
      LoadCursor( nullptr, IDC_ARROW ), // HCURSOR      hCursor
      (HBRUSH)(COLOR_WINDOW + 1),       // HBRUSH       hbrBackground
      nullptr,                          // LPCSTR       lpszMenuName
      "VulkanCookbook",                 // LPCSTR       lpszClassName
                                        /* Win 4.0 */
      nullptr                           // HICON        hIconSm
    };

    if( !RegisterClassEx( &window_class ) ) {
      return;
    }

    WindowParams.HWnd = CreateWindow( "VulkanCookbook", window_title, WS_OVERLAPPEDWINDOW, x, y, width, height, nullptr, nullptr, WindowParams.HInstance, nullptr );
    if( !WindowParams.HWnd ) {
      return;
    }

    Created = true;
  }

  WindowFramework::~WindowFramework() {
    if( WindowParams.HWnd ) {
      DestroyWindow( WindowParams.HWnd );
    }

    if( WindowParams.HInstance ) {
      UnregisterClass( "VulkanCookbook", WindowParams.HInstance );
    }
  }

  void WindowFramework::Render() {
    if( Created &&
        Sample.Initialize( WindowParams ) ) {

      ShowWindow( WindowParams.HWnd, SW_SHOWNORMAL );
      UpdateWindow( WindowParams.HWnd );

      MSG message;
      bool loop = true;

      while( loop ) {
        if( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) ) {
          switch( message.message ) {
          case USER_MESSAGE_MOUSE_CLICK:
            Sample.MouseClick( static_cast<size_t>(message.wParam), message.lParam > 0 );
            break;
          case USER_MESSAGE_MOUSE_MOVE:
            Sample.MouseMove( static_cast<int>(message.wParam), static_cast<int>(message.lParam) );
            break;
          case USER_MESSAGE_MOUSE_WHEEL:
            Sample.MouseWheel( static_cast<short>(message.wParam) * 0.002f );
            break;
          case USER_MESSAGE_RESIZE:
            if( !Sample.Resize() ) {
              loop = false;
            }
            break;
          case USER_MESSAGE_QUIT:
            loop = false;
            break;
          }
          TranslateMessage( &message );
          DispatchMessage( &message );
        } else {
          if( Sample.IsReady() ) {
            Sample.UpdateTime();
            Sample.Draw();
            Sample.MouseReset();
          }
        }
      }
    }

    Sample.Deinitialize();
  }

#endif

} // namespace VulkanCookbook
