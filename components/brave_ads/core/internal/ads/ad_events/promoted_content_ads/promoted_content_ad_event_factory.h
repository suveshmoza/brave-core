/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_ADS_AD_EVENTS_PROMOTED_CONTENT_ADS_PROMOTED_CONTENT_AD_EVENT_FACTORY_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_ADS_AD_EVENTS_PROMOTED_CONTENT_ADS_PROMOTED_CONTENT_AD_EVENT_FACTORY_H_

#include <memory>

#include "brave/components/brave_ads/common/interfaces/ads.mojom-shared.h"
#include "brave/components/brave_ads/core/internal/ads/ad_events/ad_event_interface.h"

namespace brave_ads {

struct PromotedContentAdInfo;

namespace promoted_content_ads {

class AdEventFactory final {
 public:
  static std::unique_ptr<AdEventInterface<PromotedContentAdInfo>> Build(
      mojom::PromotedContentAdEventType event_type);
};

}  // namespace promoted_content_ads
}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_ADS_AD_EVENTS_PROMOTED_CONTENT_ADS_PROMOTED_CONTENT_AD_EVENT_FACTORY_H_
