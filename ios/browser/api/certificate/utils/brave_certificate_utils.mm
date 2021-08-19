/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "brave/ios/browser/api/certificate/utils/brave_certificate_utils.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace brave {
NSData* NSDataFromString(const std::string& str) {
  return str.empty()
             ? [[NSData alloc] init]
             : [NSData dataWithBytes:reinterpret_cast<const std::uint8_t*>(
                                         str.c_str())
                              length:str.size()];
}

std::string hex_string_from_bytes(const std::uint8_t* bytes, std::size_t size) {
  static const char hex_characters[] = "0123456789ABCDEF";

  std::string result;
  for (std::size_t i = 0; i < size; ++i) {
    result += hex_characters[(bytes[i] & 0xF0) >> 4];
    result += hex_characters[(bytes[i] & 0x0F) >> 0];
  }
  return result;
}

std::string hex_string_from_string(const std::string& string) {
  return hex_string_from_bytes(
      reinterpret_cast<const std::uint8_t*>(&string[0]), string.size());
}

std::vector<std::string> split_string(const std::string& value,
                                      char separator) {
  std::vector<std::string> result;
  std::string::size_type pBeg = 0;
  std::string::size_type pEnd = value.find(separator);

  while (pEnd != std::string::npos) {
    result.emplace_back(value, pBeg, pEnd - pBeg);
    pBeg = pEnd + 1;
    pEnd = value.find(separator, pBeg);
  }
  result.emplace_back(value, pBeg);
  return result;
}

std::vector<std::string> split_string_into_parts(const std::string& value,
                                                 std::uint32_t parts) {
  std::vector<std::string> result;
  std::size_t size = value.size() / parts;

  for (std::size_t i = 0; i < size; ++i) {
    result.emplace_back(value, i * parts, parts);
  }

  if (value.size() % parts != 0) {
    result.emplace_back(value, parts * size);
  }
  return result;
}
}  // namespace brave
