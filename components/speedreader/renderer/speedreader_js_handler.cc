/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/speedreader/renderer/speedreader_js_handler.h"

#include "brave/browser/speedreader/speedreader_tab_helper.h"
#include "brave/components/speedreader/common/speedreader_ui_prefs.mojom-shared.h"
#include "gin/arguments.h"
#include "gin/function_template.h"
#include "third_party/blink/public/common/browser_interface_broker_proxy.h"
#include "third_party/blink/public/web/blink.h"

namespace {

// TODO(karenliu): OK these values for accessibility
constexpr float kMinFontScale = 0.5f;
constexpr float kMaxFontScale = 2.5f;

}  // anonymous namespace

namespace speedreader {

SpeedreaderJSHandler::SpeedreaderJSHandler(
    content::RenderFrame* render_frame)
    : render_frame_(render_frame) {}

SpeedreaderJSHandler::~SpeedreaderJSHandler() = default;

void SpeedreaderJSHandler::AddJavaScriptObjectToFrame(
    v8::Local<v8::Context> context) {
  v8::Isolate* isolate = blink::MainThreadIsolate();
  if (context.IsEmpty())
    return;

  v8::Context::Scope context_scope(context);

  CreateSpeedreaderObject(isolate, context);
}

template <typename Sig>
void SpeedreaderJSHandler::BindFunctionToObject(
    v8::Isolate* isolate,
    v8::Local<v8::Object> javascript_object,
    const std::string& name,
    const base::RepeatingCallback<Sig>& callback) {
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  // Get the isolate associated with this object.
  javascript_object
      ->Set(context, gin::StringToSymbol(isolate, name),
            gin::CreateFunctionTemplate(isolate, callback)
                ->GetFunction(context)
                .ToLocalChecked())
      .Check();
}

void SpeedreaderJSHandler::SetFontScale(float scale) {
  if (!(scale >= kMinFontScale && scale <= kMaxFontScale)) {
    // invalid size outside of UI given
    return;
  }
  speedreader_ui_prefs_->SetFontScale(scale);
}

void SpeedreaderJSHandler::BindFunctionsToObject(
    v8::Isolate* isolate,
    v8::Local<v8::Object> javascript_object) {
  BindFunctionToObject(
      isolate, javascript_object, "setFontScale",
      base::BindRepeating(&SpeedreaderJSHandler::SetFontScale,
                          base::Unretained(this)));
}

void SpeedreaderJSHandler::EnsureConnected() {
  if (!speedreader_ui_prefs_) {
    render_frame_->GetBrowserInterfaceBroker()->GetInterface(
      speedreader_ui_prefs_.BindNewPipeAndPassReceiver());
  }
}

void SpeedreaderJSHandler::CreateSpeedreaderObject(
    v8::Isolate* isolate,
    v8::Local<v8::Context> context) {
  v8::Local<v8::Object> global = context->Global();
  v8::Local<v8::Object> speedreader_obj;
  v8::Local<v8::Value> speedreader_value;

  constexpr base::StringPiece obj_name = "speedreader";
  if (!global->Get(context, gin::StringToV8(isolate, obj_name))
           .ToLocal(&speedreader_value) ||
      !speedreader_value->IsObject()) {
    speedreader_obj = v8::Object::New(isolate);
    global
        ->Set(context, gin::StringToSymbol(isolate, obj_name),
              speedreader_obj)
        .Check();
    BindFunctionsToObject(isolate, speedreader_obj);
  }
}

}  // namespace speedreader