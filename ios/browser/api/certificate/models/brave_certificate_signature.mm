/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "brave/ios/browser/api/certificate/models/brave_certificate_signature.h"
#import "brave/ios/browser/api/certificate/utils/brave_certificate_utils.h"
#import "brave/ios/browser/api/certificate/utils/brave_certificate_x509_utils.h"

// base
#include "base/strings/sys_string_conversions.h"

// net
#include "net/cert/internal/parsed_certificate.h"
#include "net/der/input.h"

@implementation BraveCertificateSignature
- (instancetype)initWithCertificate:(const net::ParsedCertificate*)certificate {
  if ((self = [super init])) {
    _algorithm = [[NSString alloc] init];
    _digest = [[NSString alloc] init];
    _objectIdentifier = [[NSData alloc] init];
    _absoluteObjectIdentifier = [[NSString alloc] init];
    _signatureHexEncoded = [[NSString alloc] init];
    _parameters = [[NSString alloc] init];
    _bytesSize = 0;

    //    net::CertErrors errors;
    //    net::SignatureAlgorithm signature_algorithm =
    //    net::SignatureAlgorithm::Create(
    //        certificate->signature_algorithm(),
    //        &errors);
    //
    //    if (!signature_algorithm) {
    //      NSLog(@"Error: %s\n", errors.ToDebugString().c_str());
    //      return self;
    //    }

    _digest =
        base::SysUTF8ToNSString(x509_utils::signature_algorithm_digest_to_name(
            certificate->signature_algorithm()));
    _algorithm =
        base::SysUTF8ToNSString(x509_utils::signature_algorithm_id_to_name(
            certificate->signature_algorithm()));

    net::der::Input signature_oid;
    net::der::Input signature_params;
    if (x509_utils::ParseAlgorithmIdentifier(
            certificate->signature_algorithm_tlv(), &signature_oid,
            &signature_params)) {
      _objectIdentifier = brave::NSDataFromString(signature_oid.AsString());

      std::string absolute_oid = x509_utils::NIDToAbsoluteOID(signature_oid);
      if (!absolute_oid.empty()) {
        _absoluteObjectIdentifier = base::SysUTF8ToNSString(absolute_oid);
      }

      if (!x509_utils::IsNull(signature_params)) {
        _parameters = base::SysUTF8ToNSString(
            brave::hex_string_from_string(signature_params.AsString()));
      }
    }

    _signatureHexEncoded =
        base::SysUTF8ToNSString(brave::hex_string_from_string(
            certificate->signature_value().bytes().AsString()));
    _bytesSize = certificate->signature_value().bytes().Length();
  }
  return self;
}
@end
