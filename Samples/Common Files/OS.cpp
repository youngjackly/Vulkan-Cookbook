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

#include "CookbookSampleFramework.h"


/* graphics contexts */
xcb_gcontext_t       foreground;
xcb_gcontext_t       fill;
xcb_drawable_t       pid;

namespace VulkanCookbook {

#ifdef VK_USE_PLATFORM_XCB_KHR

WindowFramework::WindowFramework( const char               * window_title,
								  int                        x,
								  int                        y,
								  int                        width,
								  int                        height,
								  VulkanCookbookSampleBase & sample ) :
  WindowParams(),
  Sample( sample ),
  Created( false ) {

	/* Open the connection to the X server */

	xcb_connection_t    *c = xcb_connect (nullptr, nullptr);

	/* Get the first screen */
	xcb_screen_t *screen = xcb_setup_roots_iterator (xcb_get_setup (c)).data;

	uint32_t             mask = 0;
	uint32_t             values[2];


	/* black foreground graphic context */
	foreground = xcb_generate_id (c);
	mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	values[0] = screen->black_pixel;
	values[1] = 0;
	xcb_create_gc (c, foreground, screen->root, mask, values);

	/* make the background pixmap for the window */
	pid = xcb_generate_id (c);
	xcb_create_pixmap(c,
			  screen->root_depth,
			  pid,
			  screen->root,
			  500, 500);

	/* context for filling with white */
	fill = xcb_generate_id(c);
	mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
	values[0] = screen->white_pixel;
	values[1] = screen->white_pixel;
	xcb_create_gc(c, fill, pid, mask,values);

	/* Create the window */
	xcb_window_t win = xcb_generate_id(c);
	mask = XCB_CW_BACK_PIXMAP  | XCB_CW_EVENT_MASK;
	values[0] = pid;
	values[1] = XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
				XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_MOTION |
				XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE;
	xcb_create_window (c,                             /* Connection          */
			   screen->root_depth,          /* depth               */
					   win,                           /* window Id           */
					   screen->root,                  /* parent window       */
					   x, y,                          /* x, y                */
					   width, height,                      /* width, height       */
					   10,                            /* border_width        */
					   XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
					   screen->root_visual,           /* visual              */
					   mask, values);                 /* masks */


	/* Map the window on the screen */
	xcb_map_window (c, win);

	/* fill the pixmap with white (it starts empty) */
	xcb_poly_fill_rectangle(c, pid, fill, 1, (xcb_rectangle_t[]){{ 0, 0, 500, 500}});

	xcb_flush(c);

	WindowParams.Connection = c;
	WindowParams.Window = win;
  Created = true;
}

WindowFramework::~WindowFramework() {

}

void WindowFramework::Render() {
	if( Created && Sample.Initialize( WindowParams ) )
	{

		bool loop = true;

		xcb_generic_event_t *e  = nullptr;

		while( (e = xcb_wait_for_event(WindowParams.Connection)) ) {

			switch( e->response_type & ~0x80 ) {
			case XCB_BUTTON_PRESS:
			{
				xcb_button_press_event_t* be = (xcb_button_press_event_t*)e;
				Sample.MouseClick( 0, be->state );
			}
				break;
				//	  case USER_MESSAGE_MOUSE_MOVE:
				//		Sample.MouseMove( static_cast<int>(message.wParam), static_cast<int>(message.lParam) );
				//		break;
				//	  case USER_MESSAGE_MOUSE_WHEEL:
				//		Sample.MouseWheel( static_cast<short>(message.wParam) * 0.002f );
				//		break;
				//	  case USER_MESSAGE_RESIZE:
				//		if( !Sample.Resize() ) {
				//		  loop = false;
				//		}
				//		break;
				//	  case USER_MESSAGE_QUIT:
				//		loop = false;
				//		break;
			default:
				break;
			}
			free(e);

			if( Sample.IsReady() ) {
				Sample.UpdateTime();
				Sample.Draw();
				Sample.MouseReset();
			}
		}
	}
	Sample.Deinitialize();
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
