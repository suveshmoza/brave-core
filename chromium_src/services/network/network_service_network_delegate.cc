/* Copyright 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "services/network/network_service_network_delegate.h"

#include "services/network/cookie_settings.h"

#define IsCookieAccessible IsEphemeralCookieAccessible
#define IsPrivacyModeEnabled IsEphemeralPrivacyModeEnabled
#define AnnotateAndMoveUserBlockedCookies \
  AnnotateAndMoveUserBlockedEphemeralCookies
#define OnAnnotateAndMoveUserBlockedCookies \
  OnAnnotateAndMoveUserBlockedCookies_ChromiumImpl
#define OnCanSetCookie OnCanSetCookie_ChromiumImpl
#define OnForcePrivacyMode OnForcePrivacyMode_ChromiumImpl

#include "../../../../services/network/network_service_network_delegate.cc"

#undef OnForcePrivacyMode
#undef OnCanSetCookie
#undef OnAnnotateAndMoveUserBlockedCookies
#undef AnnotateAndMoveUserBlockedCookies
#undef IsPrivacyModeEnabled
#undef IsCookieAccessible

using content_settings::EphemeralStorageAwareType;

namespace network {

bool NetworkServiceNetworkDelegate::OnAnnotateAndMoveUserBlockedCookies(
    const net::URLRequest& request,
    net::CookieAccessResultList& maybe_included_cookies,
    net::CookieAccessResultList& excluded_cookies,
    bool allowed_from_caller) {
  // Enable ephemeral storage support for the call.
  auto scoped_ephemeral_storage_awareness =
      network_context_->cookie_manager()
          ->cookie_settings()
          .CreateScopedEphemeralStorageAwareness(
              EphemeralStorageAwareType::kAware);
  return OnAnnotateAndMoveUserBlockedCookies_ChromiumImpl(
      request, maybe_included_cookies, excluded_cookies, allowed_from_caller);
}

bool NetworkServiceNetworkDelegate::OnCanSetCookie(
    const net::URLRequest& request,
    const net::CanonicalCookie& cookie,
    net::CookieOptions* options,
    bool allowed_from_caller) {
  // Enable ephemeral storage support for the call.
  auto scoped_ephemeral_storage_awareness =
      network_context_->cookie_manager()
          ->cookie_settings()
          .CreateScopedEphemeralStorageAwareness(
              EphemeralStorageAwareType::kAware);
  return OnCanSetCookie_ChromiumImpl(request, cookie, options,
                                     allowed_from_caller);
}

bool NetworkServiceNetworkDelegate::OnForcePrivacyMode(
    const GURL& url,
    const net::SiteForCookies& site_for_cookies,
    const absl::optional<url::Origin>& top_frame_origin,
    net::SamePartyContext::Type same_party_context_type) const {
  // Enable ephemeral storage support for the call.
  auto scoped_ephemeral_storage_awareness =
      network_context_->cookie_manager()
          ->cookie_settings()
          .CreateScopedEphemeralStorageAwareness(
              EphemeralStorageAwareType::kAware);
  return OnForcePrivacyMode_ChromiumImpl(
      url, site_for_cookies, top_frame_origin, same_party_context_type);
}

}  // namespace network
