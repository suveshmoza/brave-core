/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_USER_ATTENTION_USER_REACTIONS_USER_REACTIONS_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_USER_ATTENTION_USER_REACTIONS_USER_REACTIONS_H_

#include "base/memory/raw_ptr.h"
#include "brave/components/brave_ads/core/internal/history/history_manager_observer.h"

namespace brave_ads {

class Account;
struct AdContentInfo;

class UserReactions final : public HistoryManagerObserver {
 public:
  explicit UserReactions(Account* account);

  UserReactions(const UserReactions& other) = delete;
  UserReactions& operator=(const UserReactions& other) = delete;

  UserReactions(UserReactions&& other) noexcept = delete;
  UserReactions& operator=(UserReactions&& other) noexcept = delete;

  ~UserReactions() override;

 private:
  // HistoryManagerObserver:
  void OnDidLikeAd(const AdContentInfo& ad_content) override;
  void OnDidDislikeAd(const AdContentInfo& ad_content) override;
  void OnDidMarkAdAsInappropriate(const AdContentInfo& ad_content) override;
  void OnDidSaveAd(const AdContentInfo& ad_content) override;

  const raw_ptr<Account> account_ = nullptr;  // NOT OWNED
};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_USER_ATTENTION_USER_REACTIONS_USER_REACTIONS_H_
