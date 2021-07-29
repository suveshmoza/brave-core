/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_SPEEDREADER_RENDERER_SPEEDREADER_FRAME_OBSERVER_
#define BRAVE_COMPONENTS_SPEEDREADER_RENDERER_SPEEDREADER_FRAME_OBSERVER_

#include <memory>

#include "brave/components/speedreader/renderer/speedreader_js_handler.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"

namespace speedreader {

class SpeedreaderJsRenderFrameObserver : public content::RenderFrameObserver {
 public:
  SpeedreaderJsRenderFrameObserver(content::RenderFrame* render_frame,
                                   const int32_t isolated_world_id);
  ~SpeedreaderJsRenderFrameObserver() override = default;

 private:
  // content::RenderFrameObserver:
  void OnDestruct() override;

  // The isolated world that speedreader should be written to
  //int32_t isolated_world_id_;
};

}  // namespace speedreader

#endif  // BRAVE_COMPONENTS_SPEEDREADER_RENDERER_SPEEDREADER_FRAME_OBSERVER_