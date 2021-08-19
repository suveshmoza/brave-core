/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_IOS_BROWSER_API_CERTIFICATE_UTILS_BRAVE_CERTIFICATE_UTILS_H_
#define BRAVE_IOS_BROWSER_API_CERTIFICATE_UTILS_BRAVE_CERTIFICATE_UTILS_H_

#import <Foundation/Foundation.h>
#include <string>
#include <vector>
#include "base/time/time.h"
#include "net/cert/internal/signature_algorithm.h"
#include "net/der/input.h"

#import "brave/ios/browser/api/certificate/common/brave_certificate_enums.h"

namespace brave {
NSData* NSDataFromString(const std::string& str);
std::string hex_string_from_bytes(const std::uint8_t* bytes, std::size_t size);
std::string hex_string_from_string(const std::string& string);

std::vector<std::string> split_string(const std::string& value, char separator);
std::vector<std::string> split_string_into_parts(const std::string& value,
                                                 std::uint32_t parts);
}  // namespace brave

#endif  //  BRAVE_IOS_BROWSER_API_CERTIFICATE_UTILS_BRAVE_CERTIFICATE_UTILS_H_
