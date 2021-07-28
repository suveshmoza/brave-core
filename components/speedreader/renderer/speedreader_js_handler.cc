/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/speedreader/renderer/speedreader_js_handler.h"
#include "brave/components/speedreader/common/speedreader_ui_prefs.mojom-shared.h"

#include "gin/arguments.h"
#include "gin/function_template.h"

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

v8::Local<v8::Object> SpeedreaderJSHandler::CreateSpeedreaderObject(
    v8::Isolate* isolate,
    v8::Local<v8::Context> context) {
  v8::Local<v8::Object> global = context->Global();
  v8::Local<v8::Object> speedreader_obj;
  v8::Local<v8::Value> speedreader_value;
  if (!global->Get(context, gin::StringToV8(isolate, "cf_worker"))
           .ToLocal(&speedreader_value) ||
      !speedreader_value->IsObject()) {
    speedreader_obj = v8::Object::New(isolate);
    global
        ->Set(context, gin::StringToSymbol(isolate, "cf_worker"),
              speedreader_obj)
        .Check();
    BindFunctionsToObject(isolate, speedreader_obj);
  }

}

}  // namespace speedreader