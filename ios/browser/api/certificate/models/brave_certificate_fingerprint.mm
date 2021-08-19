/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "brave/ios/browser/api/certificate/models/brave_certificate_fingerprint.h"
#import "brave/ios/browser/api/certificate/utils/brave_certificate_utils.h"
#import "brave/ios/browser/api/certificate/utils/brave_certificate_x509_utils.h"

// base
#include "base/strings/sys_string_conversions.h"

// third_party
#include "third_party/boringssl/src/include/openssl/evp.h"
#include "third_party/boringssl/src/include/openssl/pool.h"
#include "third_party/boringssl/src/include/openssl/sha.h"

@implementation BraveCertificateFingerprint
- (instancetype)initWithCertificate:(CFDataRef)cert_data
                           withType:(BraveFingerprintType)type {
  if ((self = [super init])) {
    // OpenSSL_add_all_digests()
    _type = type;

    switch (type) {
      case BraveFingerprintType_SHA1: {
        std::string data = std::string(SHA_DIGEST_LENGTH, '\0');

        SHA_CTX ctx;
        SHA1_Init(&ctx);
        SHA1_Update(
            &ctx,
            reinterpret_cast<const unsigned char*>(CFDataGetBytePtr(cert_data)),
            CFDataGetLength(cert_data));
        SHA1_Final(reinterpret_cast<unsigned char*>(&data[0]), &ctx);

        _fingerprintHexEncoded =
            base::SysUTF8ToNSString(brave::hex_string_from_string(data));
      } break;

      case BraveFingerprintType_SHA256: {
        std::string data = std::string(SHA256_DIGEST_LENGTH, '\0');

        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(
            &ctx,
            reinterpret_cast<const unsigned char*>(CFDataGetBytePtr(cert_data)),
            CFDataGetLength(cert_data));
        SHA256_Final(reinterpret_cast<unsigned char*>(&data[0]), &ctx);

        _fingerprintHexEncoded =
            base::SysUTF8ToNSString(brave::hex_string_from_string(data));
      } break;
    }
  }
  return self;
}
@end
