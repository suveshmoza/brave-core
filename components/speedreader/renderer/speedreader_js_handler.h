/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_SPEEDREADER_RENDERER_SPEEDREADER_JS_HANDLER_H_
#define BRAVE_COMPONENTS_SPEEDREADER_RENDERER_SPEEDREADER_JS_HANDLER_H_

#include "brave/components/speedreader/common/speedreader_ui_prefs.mojom.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "v8/include/v8.h"

namespace speedreader {

class SpeedreaderJSHandler {
 public:
  explicit SpeedreaderJSHandler(content::RenderFrame* render_frame);
  ~SpeedreaderJSHandler();

  // Add the "speedreader" JavaScript object and its functions to the current
  // |RenderFrame|.
  void AddJavaScriptObjectToFrame(v8::Local<v8::Context> context);

 private:
  void CreateSpeedreaderObject(
      v8::Isolate* isolate,
      v8::Local<v8::Context> context);

  void BindFunctionsToObject(v8::Isolate* isolate,
                             v8::Local<v8::Object> javascript_object);

  // Add a function to the provided object.
  template <typename Sig>
  void BindFunctionToObject(v8::Isolate* isolate,
                            v8::Local<v8::Object> javascript_object,
                            const std::string& name,
                            const base::RepeatingCallback<Sig>& callback);

  void EnsureConnected();

  void SetFontScale(float scale);

  content::RenderFrame* render_frame_;
  mojo::Remote<mojom::SpeedreaderUIPrefs> speedreader_ui_prefs_;
};

}  // namespace speedreader

#endif  // BRAVE_COMPONENTS_SPEEDREADER_RENDERER_SPEEDREADER_JS_HANDLER_H_