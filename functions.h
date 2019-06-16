#ifndef NATIVE_EXTENSION_GRAB_H
#define NATIVE_EXTENSION_GRAB_H

#include <node_api.h>

napi_value MakePanel(napi_env, napi_callback_info);
napi_value MakeKeyWindow(napi_env, napi_callback_info);

#endif
