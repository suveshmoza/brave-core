// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import { AppObjectType } from '../../constants/types'

export type InitializedPayloadType = {
  isWalletCreated: boolean
  isWalletLocked: boolean
  favoriteApps: AppObjectType[]
  accounts: string[]
  isWalletBackedUp: boolean
  walletAccountNames: string[]
  visibleTokens: string[]
}

export type UnlockWalletPayloadType = {
  password: string
}

export type SetInitialAccountNamesPayloadType = {
  accountNames: string[]
}

export type SetInitialVisibleTokensPayloadType = {
  visibleAssets: string[]
}

export type AddNewAccountNamePayloadType = {
  accountName: string
}

export type ChainChangedEventPayloadType = {
  chainId: string
}
