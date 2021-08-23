/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ad_events/ad_event_util.h"

#include <string>

#include "base/guid.h"
#include "bat/ads/internal/bundle/creative_ad_notification_info.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

namespace {

CreativeAdNotificationInfo GetCreativeAdNotification() {
  CreativeAdNotificationInfo creative_ad_notification;

  creative_ad_notification.creative_instance_id = base::GenerateGUID();
  creative_ad_notification.advertiser_id = base::GenerateGUID();

  return creative_ad_notification;
}

CreativeAdNotificationInfo GetCreativeAdNotificationForAdvertiser(
    const std::string& advertiser_id) {
  CreativeAdNotificationInfo creative_ad_notification;

  creative_ad_notification.creative_instance_id = base::GenerateGUID();
  creative_ad_notification.advertiser_id = advertiser_id;

  return creative_ad_notification;
}

AdEventInfo GetAdEvent(
    const CreativeAdNotificationInfo& creative_ad_notification,
    const ConfirmationType confirmation_type,
    const base::Time& timestamp) {
  AdEventInfo ad_event;
  ad_event.uuid = base::GenerateGUID();
  ad_event.confirmation_type = confirmation_type;
  ad_event.creative_instance_id = creative_ad_notification.creative_instance_id;
  ad_event.advertiser_id = creative_ad_notification.advertiser_id;
  ad_event.timestamp = static_cast<int64_t>(timestamp.ToDoubleT());

  return ad_event;
}

}  // namespace

TEST(BatAdsAdEventUtilTest, GetLastSeenAdTimeForEmptyAdEvents) {
  // Arrange
  AdEventList ad_events;

  const CreativeAdNotificationInfo creative_ad_notification =
      GetCreativeAdNotification();

  // Act
  const base::Time ad_last_seen =
      GetLastSeenAdTime(ad_events, creative_ad_notification);

  // Assert
  const base::Time expected_ad_last_seen = base::Time::FromDoubleT(0.0);
  EXPECT_EQ(expected_ad_last_seen, ad_last_seen);
}

TEST(BatAdsAdEventUtilTest, GetLastSeenAdTimeForUnseenAd) {
  // Arrange
  AdEventList ad_events;

  const CreativeAdNotificationInfo creative_ad_notification_1 =
      GetCreativeAdNotification();

  const base::Time event_time =
      base::Time::Now() - base::TimeDelta::FromHours(12);
  const AdEventInfo ad_event = GetAdEvent(
      creative_ad_notification_1, ConfirmationType::kViewed, event_time);
  ad_events.push_back(ad_event);

  // Act
  const CreativeAdNotificationInfo creative_ad_notification_2 =
      GetCreativeAdNotification();
  const base::Time ad_last_seen =
      GetLastSeenAdTime(ad_events, creative_ad_notification_2);

  // Assert
  const base::Time expected_ad_last_seen = base::Time::FromDoubleT(0.0);
  EXPECT_EQ(expected_ad_last_seen, ad_last_seen);
}

TEST(BatAdsAdEventUtilTest, GetLastSeenAdTime) {
  // Arrange
  AdEventList ad_events;

  const base::Time now = base::Time::Now();

  // Ad events are ordered by date DESC in |AdEvents::GetAll()|
  const CreativeAdNotificationInfo creative_ad_notification_1 =
      GetCreativeAdNotification();

  const CreativeAdNotificationInfo creative_ad_notification_2 =
      GetCreativeAdNotification();

  const AdEventInfo ad_event_4 =
      GetAdEvent(creative_ad_notification_1, ConfirmationType::kConversion,
                 now - base::TimeDelta::FromHours(3));
  ad_events.push_back(ad_event_4);

  const AdEventInfo ad_event_3 =
      GetAdEvent(creative_ad_notification_1, ConfirmationType::kViewed,
                 now - base::TimeDelta::FromHours(6));
  ad_events.push_back(ad_event_3);

  const AdEventInfo ad_event_2 =
      GetAdEvent(creative_ad_notification_2, ConfirmationType::kViewed,
                 now - base::TimeDelta::FromHours(11));
  ad_events.push_back(ad_event_2);

  const AdEventInfo ad_event_1 =
      GetAdEvent(creative_ad_notification_1, ConfirmationType::kViewed,
                 now - base::TimeDelta::FromHours(12));
  ad_events.push_back(ad_event_1);

  // Act
  const base::Time ad_last_seen =
      GetLastSeenAdTime(ad_events, creative_ad_notification_1);

  // Assert
  const base::Time expected_timestamp = now - base::TimeDelta::FromHours(6);
  const base::Time expected_ad_last_seen = base::Time::FromDoubleT(
      static_cast<int64_t>(expected_timestamp.ToDoubleT()));
  EXPECT_EQ(expected_ad_last_seen, ad_last_seen);
}

TEST(BatAdsAdEventUtilTest, GetLastSeenAdvertiserTimeForEmptyAdEvents) {
  // Arrange
  AdEventList ad_events;

  const CreativeAdNotificationInfo creative_ad_notification =
      GetCreativeAdNotification();

  // Act
  const base::Time advertiser_last_seen =
      GetLastSeenAdvertiserTime(ad_events, creative_ad_notification);

  // Assert
  const base::Time expected_advertiser_last_seen = base::Time::FromDoubleT(0.0);
  EXPECT_EQ(expected_advertiser_last_seen, advertiser_last_seen);
}

TEST(BatAdsAdEventUtilTest, GetLastSeenAdvertiserTimeForUnseenAdvertiser) {
  // Arrange
  AdEventList ad_events;

  const CreativeAdNotificationInfo creative_ad_notification_1 =
      GetCreativeAdNotification();

  const base::Time event_time =
      base::Time::Now() - base::TimeDelta::FromHours(12);
  const AdEventInfo ad_event = GetAdEvent(
      creative_ad_notification_1, ConfirmationType::kViewed, event_time);
  ad_events.push_back(ad_event);

  // Act
  const CreativeAdNotificationInfo creative_ad_notification_2 =
      GetCreativeAdNotification();
  const base::Time advertiser_last_seen =
      GetLastSeenAdvertiserTime(ad_events, creative_ad_notification_2);

  // Assert
  const base::Time expected_advertiser_last_seen = base::Time::FromDoubleT(0.0);
  EXPECT_EQ(expected_advertiser_last_seen, advertiser_last_seen);
}

TEST(BatAdsAdEventUtilTest, GetLastSeenAdvertiserTime) {
  // Arrange
  AdEventList ad_events;

  const base::Time now = base::Time::Now();

  const std::string advertiser_id_1 = base::GenerateGUID();
  const CreativeAdNotificationInfo creative_ad_notification_1 =
      GetCreativeAdNotificationForAdvertiser(advertiser_id_1);

  const std::string advertiser_id_2 = base::GenerateGUID();
  const CreativeAdNotificationInfo creative_ad_notification_2 =
      GetCreativeAdNotificationForAdvertiser(advertiser_id_2);

  const CreativeAdNotificationInfo creative_ad_notification_3 =
      GetCreativeAdNotificationForAdvertiser(advertiser_id_1);

  // Ad events are ordered by date DESC in |AdEvents::GetAll()|
  const AdEventInfo ad_event_4 =
      GetAdEvent(creative_ad_notification_1, ConfirmationType::kViewed,
                 now - base::TimeDelta::FromHours(3));
  ad_events.push_back(ad_event_4);

  const AdEventInfo ad_event_3 =
      GetAdEvent(creative_ad_notification_3, ConfirmationType::kViewed,
                 now - base::TimeDelta::FromHours(6));
  ad_events.push_back(ad_event_3);

  const AdEventInfo ad_event_2 =
      GetAdEvent(creative_ad_notification_2, ConfirmationType::kViewed,
                 now - base::TimeDelta::FromHours(11));
  ad_events.push_back(ad_event_2);

  const AdEventInfo ad_event_1 =
      GetAdEvent(creative_ad_notification_1, ConfirmationType::kViewed,
                 now - base::TimeDelta::FromHours(12));
  ad_events.push_back(ad_event_1);

  // Act
  const base::Time advertiser_last_seen =
      GetLastSeenAdvertiserTime(ad_events, creative_ad_notification_3);

  // Assert
  const base::Time expected_timestamp = now - base::TimeDelta::FromHours(3);
  const base::Time expected_advertiser_last_seen = base::Time::FromDoubleT(
      static_cast<int64_t>(expected_timestamp.ToDoubleT()));
  EXPECT_EQ(expected_advertiser_last_seen, advertiser_last_seen);
}

}  // namespace ads
