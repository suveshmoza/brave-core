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
    const toggleHumanWeb = (enabled: boolean) => {
      APP.prefs.set("modules.web-discovery-project.enabled", enabled)
      APP.prefs.set("modules.hpnv2.enabled", enabled);
    }

    const HUMAN_WEB_PREF_KEY = 'brave.human_web_enabled'

    chrome.settingsPrivate.onPrefsChanged.addListener((prefs) => {
      const pref = prefs.find(p => p.key === HUMAN_WEB_PREF_KEY)
      if (pref && pref.type === chrome.settingsPrivate.PrefType.BOOLEAN) {
        toggleHumanWeb(pref.value)
      }
    })

    chrome.settingsPrivate.getPref(HUMAN_WEB_PREF_KEY, (pref) => {
      if (pref && pref.type === chrome.settingsPrivate.PrefType.BOOLEAN) {
        toggleHumanWeb(pref.value)
      }
    })
  })
}
