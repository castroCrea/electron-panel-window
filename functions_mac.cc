#import <CoreServices/CoreServices.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <objc/objc-runtime.h>

#include "functions.h"

NSMutableDictionary *windowToOriginalClass = [NSMutableDictionary dictionary];

@interface MyDelegate : NSObject
{
}
- (BOOL)windowShouldClose:(id)sender;
@end

@implementation MyDelegate
- (BOOL)windowShouldClose:(NSWindow *)window
{
  NSAlert *alert = [[NSAlert alloc] init];
  [alert setAlertStyle:NSInformationalAlertStyle];
  [alert setMessageText:@"Are you sure you want to quit?"];
  [alert addButtonWithTitle:@"Yes"];
  [alert addButtonWithTitle:@"No"];
  NSInteger result = [alert runModal];
  if (result == NSAlertFirstButtonReturn)
  {
    [alert release];
    return YES;
  }
  [alert release];
  return NO;
}
@end

@interface Panel : NSPanel
@end

@implementation Panel
- (BOOL)canBecomeKeyWindow
{
  return YES;
}
- (BOOL)canBecomeMainWindow
{
  return YES;
}
- (BOOL)needsPanelToBecomeKey
{
  return YES;
}
- (BOOL)acceptsFirstResponder
{
  return YES;
}
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath context:(nullable void *)context
{
  // macOS Big Sur attempts to remove an observer for the NSTitlebarView that doesn't exist.
  // This is due to us changing the class from NSWindow -> NSPanel at runtime, it's possible
  // there is assumed setup that doesn't happen. Details of the exception this is avoiding are
  // here: https://github.com/goabstract/electron-panel-window/issues/6
  if ([keyPath isEqualToString:@"_titlebarBackdropGroupName"])
  {
    NSLog(@"removeObserver ignored");
    return;
  }

  if (context)
  {
    [super removeObserver:observer forKeyPath:keyPath context:context];
  }
  else
  {
    [super removeObserver:observer forKeyPath:keyPath];
  }
}
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath
{
  [self removeObserver:observer forKeyPath:keyPath context:NULL];
}
@end

napi_value MakePanel(napi_env env, napi_callback_info info)
{
  napi_status status;
  size_t argc = 1;
  napi_value handleBuffer[1];

  status = napi_get_cb_info(env, info, &argc, handleBuffer, 0, 0);
  if (status != napi_ok || argc < 1)
  {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return 0;
  }

  napi_handle_scope scope;
  status = napi_open_handle_scope(env, &scope);
  if (status != napi_ok)
  {
    return 0;
  }

  void *buffer;
  size_t bufferLength;
  status = napi_get_buffer_info(env, handleBuffer[0], &buffer, &bufferLength);
  if (status != napi_ok)
  {
    return handleBuffer[0];
  }
  NSView *mainContentView = *reinterpret_cast<NSView **>(buffer);
  if (!mainContentView)
    return handleBuffer[0];

  NSWindow *window = mainContentView.window;

  windowToOriginalClass[@((intptr_t)window)] = object_getClass(window);

  // Convert the NSWindow class to NSPanel
  object_setClass(window, [Panel class]);

  // Ensure that the window is a "non activating panel" which means it won't activate the application
  // when it becomes key.
  window.styleMask = NSWindowStyleMaskNonactivatingPanel;
  if (@available(macOS 10.14, *))
  {
    window.styleMask |= NSWindowStyleMaskFullSizeContentView |
                        NSWindowStyleMaskTitled |
                        NSWindowStyleMaskClosable |
                        NSWindowStyleMaskMiniaturizable |
                        NSWindowStyleMaskResizable;
  }
  else
  {
    window.styleMask |= NSWindowStyleMaskBorderless | NSWindowStyleMaskResizable;
  }

  // // Hide the traffic light controls
  [[window standardWindowButton:NSWindowCloseButton] setHidden:YES];
  [[window standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
  [[window standardWindowButton:NSWindowZoomButton] setHidden:YES];

  // // Hide the titlebar
  // [window setTitlebarAppearsTransparent:YES];
  // [window setTitleVisibility:NSWindowTitleHidden];

  // // Ensure that the window can display over the top of fullscreen apps
  [window setCollectionBehavior:NSWindowCollectionBehaviorTransient | NSWindowCollectionBehaviorCanJoinAllSpaces | NSWindowCollectionBehaviorFullScreenAuxiliary];
  [window setLevel:NSFloatingWindowLevel];
  [window setFloatingPanel:YES];

  status = napi_close_handle_scope(env, scope);
  if (status != napi_ok)
  {
    return 0;
  }

  return handleBuffer[0];
}

napi_value MakeKeyWindow(napi_env env, napi_callback_info info)
{
  napi_status status;
  size_t argc = 1;
  napi_value handleBuffer[1];

  status = napi_get_cb_info(env, info, &argc, handleBuffer, 0, 0);
  if (status != napi_ok || argc < 1)
  {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return 0;
  }

  napi_handle_scope scope;
  status = napi_open_handle_scope(env, &scope);
  if (status != napi_ok)
  {
    return 0;
  }

  void *buffer;
  size_t bufferLength;
  status = napi_get_buffer_info(env, handleBuffer[0], &buffer, &bufferLength);
  if (status != napi_ok)
  {
    return handleBuffer[0];
  }
  NSView *mainContentView = *reinterpret_cast<NSView **>(buffer);

  if (!mainContentView)
    return handleBuffer[0];

  [mainContentView.window makeKeyWindow];

  status = napi_close_handle_scope(env, scope);
  if (status != napi_ok)
  {
    return 0;
  }

  return handleBuffer[0];
}
