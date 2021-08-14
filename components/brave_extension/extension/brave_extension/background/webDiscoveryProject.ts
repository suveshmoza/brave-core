// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import { App } from '../web-discovery-project/build'

if (App !== undefined) {
  const APP = new App({
    version: chrome.runtime.getManifest().version,
  });

  APP.start().then(() => {
    const toggleWebDiscoveryExtension = (enabled: boolean) => {
      APP.prefs.set('modules.web-discovery-project.enabled', enabled)
      APP.prefs.set('modules.hpnv2.enabled', enabled);
    }

    const WEB_DISCOVERY_PREF_KEY = 'brave.web_discovery_enabled'

    chrome.settingsPrivate.onPrefsChanged.addListener((prefs) => {
      const pref = prefs.find(p => p.key === WEB_DISCOVERY_PREF_KEY)
      if (pref && pref.type === chrome.settingsPrivate.PrefType.BOOLEAN) {
        toggleWebDiscoveryExtension(pref.value)
      }
    })

    chrome.settingsPrivate.getPref(WEB_DISCOVERY_PREF_KEY, (pref) => {
      if (pref && pref.type === chrome.settingsPrivate.PrefType.BOOLEAN) {
        toggleWebDiscoveryExtension(pref.value)
      }
    })
  })
}
