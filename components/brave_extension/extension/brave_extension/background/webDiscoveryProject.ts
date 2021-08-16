// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import { App } from '../web-discovery-project/build'

if (App !== undefined) {
  const APP = new App({
    version: chrome.runtime.getManifest().version
  })

  const WEB_DISCOVERY_PREF_KEY = 'brave.web_discovery_enabled'

  const toggleWebDiscovery = (pref: chrome.settingsPrivate.PrefObject) => {
    if (pref && pref.type === chrome.settingsPrivate.PrefType.BOOLEAN) {
      const enable = pref.value
      if (enable) {
        // enable
        APP.start()
          .then(
            () => {
              APP.prefs.set('modules.web-discovery-project.enabled', true)
              APP.prefs.set('modules.hpnv2.enabled', true)
            },
            (err) => {
              console.error('[web-discovery]', err)
            }
          )
      } else {
        // disable
        if (APP.isRunning) {
          // If app is not running, prefs may not be initialized, so we guard
          // against that error.
          APP.prefs.set('modules.web-discovery-project.enabled', false)
          APP.prefs.set('modules.hpnv2.enabled', false)
          APP.stop()
        }
      }
    }
  }

  chrome.settingsPrivate.onPrefsChanged.addListener((prefs: chrome.settingsPrivate.PrefObject[]) => {
    const pref = prefs.find(p => p.key === WEB_DISCOVERY_PREF_KEY)
    if (pref) {
      toggleWebDiscovery(pref)
    }
  })

  chrome.settingsPrivate.getPref(WEB_DISCOVERY_PREF_KEY, (pref: chrome.settingsPrivate.PrefObject) => {
    toggleWebDiscovery(pref)
  })
}
