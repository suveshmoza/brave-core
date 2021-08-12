/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/toolbar/brave_vpn_button.h"

#include <memory>
#include <utility>

#include "base/notreached.h"
#include "brave/app/vector_icons/vector_icons.h"
#include "brave/browser/brave_vpn/brave_vpn_service_factory.h"
#include "brave/browser/themes/theme_properties.h"
#include "brave/components/brave_vpn/brave_vpn_service.h"
#include "brave/grit/brave_generated_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_properties.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/views/toolbar/toolbar_ink_drop_util.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/gfx/rrect_f.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/highlight_path_generator.h"

VpnLoginStatusDelegate::VpnLoginStatusDelegate() = default;
VpnLoginStatusDelegate::~VpnLoginStatusDelegate() = default;

void VpnLoginStatusDelegate::UpdateTargetURL(content::WebContents* source,
    const GURL& url) {
  LOG(ERROR) << "BSC]] UpdateTargetURL\nurl=" << url;
}

constexpr int kVpnLoginStatusIsolatedWorldId =
    content::ISOLATED_WORLD_ID_CONTENT_END + 1;

void VpnLoginStatusDelegate::LoadingStateChanged(content::WebContents* source,
                                                 bool to_different_document) {
  LOG(ERROR) << "BSC]] LoadingStateChanged\nto_different_document="
             << to_different_document << "\nIsLoading=" << source->IsLoading();

  if (!source->IsLoading()) {
    auto* main_frame = source->GetMainFrame();

    // https://github.com/brave/account-brave-com/blob/dev/static/js/skus.js
    const char16_t kRegisterSkuListener[] = uR"(
window.addEventListener("message", (event) => {
  if (event.origin !== "https://account.brave.software"){
    console.log('BSC]] bad origin; peacing out', event.origin);
    return;
  }
  if (event.type === 'credentials_presentation') {
    console.log('BSC]]', "credentials presented!");
  } else if (event.type == 'credential_summary') {
    console.log('BSC]]', "credentials summarized!");
  } else {
    console.log('BSC]] got message:', JSON.stringify(event));
  }
}, false);)";

    const char16_t kSendPostMsgScript[] =
        u"window.postMessage({type: 'get_credential_summary'}, '*')";

    std::u16string script1(kRegisterSkuListener);
    std::u16string script2(kSendPostMsgScript);
    main_frame->ExecuteJavaScriptInIsolatedWorld(
        script1, {}, kVpnLoginStatusIsolatedWorldId);
    LOG(ERROR) << "BSC]] main_frame->ExecuteJavaScriptInIsolatedWorld(script1)";

    // TODO: chain these if needed (ex: wait for first to finish)
    // I have no idea if these calls are blocking
    main_frame->ExecuteJavaScriptInIsolatedWorld(
        script2, {}, kVpnLoginStatusIsolatedWorldId);
    LOG(ERROR) << "BSC]] main_frame->ExecuteJavaScriptInIsolatedWorld(script2)";
  }
}

bool VpnLoginStatusDelegate::DidAddMessageToConsole(
      content::WebContents* source,
      blink::mojom::ConsoleMessageLevel log_level,
      const std::u16string& message,
      int32_t line_no,
      const std::u16string& source_id) {
  LOG(ERROR) << "BSC]] DidAddMessageToConsole\nmessage=" << message;
  return true;
}

namespace {

constexpr int kButtonRadius = 47;

class BraveVPNButtonHighlightPathGenerator
    : public views::HighlightPathGenerator {
 public:
  explicit BraveVPNButtonHighlightPathGenerator(const gfx::Insets& insets)
      : HighlightPathGenerator(insets) {}

  BraveVPNButtonHighlightPathGenerator(
      const BraveVPNButtonHighlightPathGenerator&) = delete;
  BraveVPNButtonHighlightPathGenerator& operator=(
      const BraveVPNButtonHighlightPathGenerator&) = delete;

  // views::HighlightPathGenerator overrides:
  absl::optional<gfx::RRectF> GetRoundRect(const gfx::RectF& rect) override {
    return gfx::RRectF(rect, kButtonRadius);
  }
};

}  // namespace

BraveVPNButton::BraveVPNButton(Profile* profile)
    : ToolbarButton(base::BindRepeating(&BraveVPNButton::OnButtonPressed,
                                        base::Unretained(this))),
      service_(BraveVpnServiceFactory::GetForProfile(profile)) {
  DCHECK(service_);
  observation_.Observe(service_);

  // Replace ToolbarButton's highlight path generator.
  views::HighlightPathGenerator::Install(
      this, std::make_unique<BraveVPNButtonHighlightPathGenerator>(
                GetToolbarInkDropInsets(this)));

  label()->SetText(
      l10n_util::GetStringUTF16(IDS_BRAVE_VPN_TOOLBAR_BUTTON_TEXT));
  gfx::FontList font_list = views::Label::GetDefaultFontList();
  constexpr int kFontSize = 12;
  label()->SetFontList(
      font_list.DeriveWithSizeDelta(kFontSize - font_list.GetFontSize()));

  // Set image positions first. then label.
  SetHorizontalAlignment(gfx::ALIGN_LEFT);

  UpdateButtonState();

  // USED TO CHECK IF THEY ARE LOGGED IN LOL
  content::WebContents::CreateParams params(profile);
  contents_ = content::WebContents::Create(params);
  contents_delegate_.reset(new VpnLoginStatusDelegate);
  contents_->SetDelegate(contents_delegate_.get());
  if (contents_) {
    LOG(ERROR) << "BSC]] CREATED WEB CONTENT";
  }
}
// TODO(bsclifton): clean up contents
BraveVPNButton::~BraveVPNButton() = default;

void BraveVPNButton::OnConnectionStateChanged(bool connected) {
  UpdateButtonState();
}

void BraveVPNButton::UpdateColorsAndInsets() {
  if (const auto* tp = GetThemeProvider()) {
    const gfx::Insets paint_insets =
        gfx::Insets((height() - GetLayoutConstant(LOCATION_BAR_HEIGHT)) / 2);
    SetBackground(views::CreateBackgroundFromPainter(
        views::Painter::CreateSolidRoundRectPainter(
            tp->GetColor(ThemeProperties::COLOR_TOOLBAR), kButtonRadius,
            paint_insets)));

    SetEnabledTextColors(tp->GetColor(
        IsConnected()
            ? BraveThemeProperties::COLOR_BRAVE_VPN_BUTTON_TEXT_CONNECTED
            : BraveThemeProperties::COLOR_BRAVE_VPN_BUTTON_TEXT_DISCONNECTED));

    std::unique_ptr<views::Border> border = views::CreateRoundedRectBorder(
        1, kButtonRadius, gfx::Insets(),
        tp->GetColor(BraveThemeProperties::COLOR_BRAVE_VPN_BUTTON_BORDER));
    constexpr gfx::Insets kTargetInsets{4, 6};
    const gfx::Insets extra_insets = kTargetInsets - border->GetInsets();
    SetBorder(views::CreatePaddedBorder(std::move(border), extra_insets));
  }

  constexpr int kBraveAvatarImageLabelSpacing = 4;
  SetImageLabelSpacing(kBraveAvatarImageLabelSpacing);
}

void BraveVPNButton::UpdateButtonState() {
  constexpr SkColor kColorConnected = SkColorSetRGB(0x51, 0xCF, 0x66);
  constexpr SkColor kColorDisconnected = SkColorSetRGB(0xAE, 0xB1, 0xC2);
  SetImage(views::Button::STATE_NORMAL,
           gfx::CreateVectorIcon(kVpnIndicatorIcon, IsConnected()
                                                        ? kColorConnected
                                                        : kColorDisconnected));
}

bool BraveVPNButton::IsConnected() {
  return service_->IsConnected();
}

void BraveVPNButton::OnButtonPressed(const ui::Event& event) {
  ShowBraveVPNPanel();

  if (contents_) {
    contents_->GetController().LoadURL(
      GURL("https://basicuser:basicpass@account.brave.software/skus/"),
      content::Referrer(),
      ui::PAGE_TRANSITION_AUTO_TOPLEVEL,
      std::string());
  }
}

void BraveVPNButton::ShowBraveVPNPanel() {
  NOTIMPLEMENTED();
}

BEGIN_METADATA(BraveVPNButton, LabelButton)
END_METADATA
