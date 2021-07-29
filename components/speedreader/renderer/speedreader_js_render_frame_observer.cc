/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/speedreader/renderer/speedreader_js_render_frame_observer.h"

namespace speedreader {

SpeedreaderJsRenderFrameObserver::SpeedreaderJsRenderFrameObserver(
    content::RenderFrame* render_frame,
    const int32_t isolated_world_id)
    : RenderFrameObserver(render_frame) {}
      //isolated_world_id_(isolated_world_id) {}

void SpeedreaderJsRenderFrameObserver::OnDestruct() {
  delete this;
}

}  // namespace speedreader