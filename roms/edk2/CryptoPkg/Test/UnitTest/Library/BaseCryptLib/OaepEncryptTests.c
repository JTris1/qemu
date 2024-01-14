/** @file
  This is a unit test for RSA OAEP encrypt.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "TestBaseCryptLib.h"

CONST  UINT8  RandSeed[] = "This is the random seed for PRNG verification.";

//
// Self signed X509 certificate
// CN = ca.self
// O = Intel
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  SelfTestCert[] = {
  0x30, 0x82, 0x03, 0x90, 0x30, 0x82, 0x02, 0x78, 0x02, 0x09, 0x00, 0xE4, 0xDF, 0x47, 0x80, 0xEF,
  0x4B, 0x3C, 0x6D, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B,
  0x05, 0x00, 0x30, 0x81, 0x89, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02,
  0x43, 0x4E, 0x31, 0x11, 0x30, 0x0F, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0C, 0x08, 0x53, 0x68, 0x61,
  0x6E, 0x67, 0x48, 0x61, 0x69, 0x31, 0x11, 0x30, 0x0F, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0C, 0x08,
  0x53, 0x68, 0x61, 0x6E, 0x67, 0x48, 0x61, 0x69, 0x31, 0x0E, 0x30, 0x0C, 0x06, 0x03, 0x55, 0x04,
  0x0A, 0x0C, 0x05, 0x49, 0x6E, 0x74, 0x65, 0x6C, 0x31, 0x0C, 0x30, 0x0A, 0x06, 0x03, 0x55, 0x04,
  0x0B, 0x0C, 0x03, 0x46, 0x49, 0x45, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C,
  0x07, 0x63, 0x61, 0x2E, 0x73, 0x65, 0x6C, 0x66, 0x31, 0x24, 0x30, 0x22, 0x06, 0x09, 0x2A, 0x86,
  0x48, 0x86, 0xF7, 0x0D, 0x01, 0x09, 0x01, 0x16, 0x15, 0x7A, 0x68, 0x69, 0x63, 0x68, 0x61, 0x6F,
  0x2E, 0x67, 0x61, 0x6F, 0x40, 0x69, 0x6E, 0x74, 0x65, 0x6C, 0x2E, 0x63, 0x6F, 0x6D, 0x30, 0x1E,
  0x17, 0x0D, 0x31, 0x39, 0x30, 0x33, 0x32, 0x37, 0x30, 0x35, 0x35, 0x37, 0x30, 0x39, 0x5A, 0x17,
  0x0D, 0x32, 0x30, 0x30, 0x33, 0x32, 0x36, 0x30, 0x35, 0x35, 0x37, 0x30, 0x39, 0x5A, 0x30, 0x81,
  0x89, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x43, 0x4E, 0x31, 0x11,
  0x30, 0x0F, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0C, 0x08, 0x53, 0x68, 0x61, 0x6E, 0x67, 0x48, 0x61,
  0x69, 0x31, 0x11, 0x30, 0x0F, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0C, 0x08, 0x53, 0x68, 0x61, 0x6E,
  0x67, 0x48, 0x61, 0x69, 0x31, 0x0E, 0x30, 0x0C, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C, 0x05, 0x49,
  0x6E, 0x74, 0x65, 0x6C, 0x31, 0x0C, 0x30, 0x0A, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x0C, 0x03, 0x46,
  0x49, 0x45, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x07, 0x63, 0x61, 0x2E,
  0x73, 0x65, 0x6C, 0x66, 0x31, 0x24, 0x30, 0x22, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D,
  0x01, 0x09, 0x01, 0x16, 0x15, 0x7A, 0x68, 0x69, 0x63, 0x68, 0x61, 0x6F, 0x2E, 0x67, 0x61, 0x6F,
  0x40, 0x69, 0x6E, 0x74, 0x65, 0x6C, 0x2E, 0x63, 0x6F, 0x6D, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0D,
  0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01,
  0x0F, 0x00, 0x30, 0x82, 0x01, 0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0xBC, 0xE4, 0x67, 0xDC, 0xC7,
  0xEA, 0x6F, 0x8A, 0xA7, 0xCC, 0xB2, 0x54, 0x47, 0x48, 0x6A, 0xE2, 0x39, 0xFF, 0xC2, 0x48, 0x58,
  0x34, 0x07, 0x03, 0x6D, 0x39, 0xB3, 0x67, 0x46, 0x4C, 0xBC, 0xA0, 0xFA, 0x4E, 0x64, 0x23, 0x56,
  0x47, 0x7B, 0xC9, 0x1A, 0x2A, 0x55, 0x42, 0x54, 0x10, 0x18, 0x30, 0x92, 0x60, 0x30, 0x5B, 0x9E,
  0xC0, 0x65, 0xD2, 0xD4, 0x05, 0x4A, 0xA6, 0x10, 0x66, 0x04, 0xA9, 0x54, 0x4E, 0xEE, 0x49, 0x39,
  0x43, 0x65, 0x1E, 0x2E, 0x28, 0xDE, 0x79, 0x24, 0xA9, 0x7E, 0xD8, 0x5B, 0xBC, 0x2F, 0x46, 0x6A,
  0xB7, 0xB6, 0x0D, 0x17, 0x88, 0x37, 0x52, 0x5C, 0xFE, 0x93, 0xC0, 0xE2, 0xFD, 0x6A, 0x08, 0x1B,
  0xFB, 0xD1, 0x87, 0xBD, 0xBD, 0x58, 0x57, 0x2C, 0x06, 0x5D, 0xD2, 0x7D, 0x52, 0xE2, 0x49, 0x8E,
  0xDC, 0xE5, 0x26, 0xBD, 0x92, 0x60, 0xB0, 0x3F, 0x58, 0x5E, 0x52, 0xD7, 0x91, 0xDA, 0x93, 0x62,
  0x8D, 0x71, 0x80, 0x53, 0xBA, 0x15, 0xC4, 0x1F, 0xF3, 0xBD, 0xE0, 0xC5, 0xA4, 0xB8, 0xD3, 0x64,
  0x12, 0x14, 0x1B, 0x11, 0x6B, 0x7B, 0xC2, 0x92, 0xC7, 0xE2, 0x94, 0x0B, 0xB8, 0x67, 0x38, 0x48,
  0x63, 0x11, 0x74, 0x25, 0x7C, 0x37, 0xC3, 0xB2, 0xAE, 0xD9, 0xA7, 0x17, 0x9C, 0x4B, 0x9D, 0x6C,
  0x27, 0xB0, 0x87, 0x16, 0x6B, 0xF2, 0x96, 0xE5, 0x1D, 0x37, 0x27, 0xDE, 0xF2, 0x98, 0xB7, 0x81,
  0x08, 0xD9, 0x7A, 0xBA, 0x84, 0x14, 0x61, 0x60, 0x48, 0xCE, 0xCE, 0x51, 0x73, 0xF4, 0xDB, 0xF1,
  0x5F, 0x7A, 0x17, 0x71, 0x4F, 0xC1, 0x0B, 0xCE, 0xC7, 0x31, 0xC1, 0x4E, 0xA3, 0xEE, 0x6F, 0x72,
  0x97, 0x90, 0xFB, 0x8B, 0x54, 0x9F, 0x82, 0x5B, 0x48, 0x5A, 0xF1, 0xAD, 0x8B, 0x3A, 0xCD, 0xCA,
  0xB2, 0x8B, 0x7A, 0x53, 0xD4, 0xF7, 0x71, 0x16, 0x75, 0xA7, 0x35, 0x02, 0x03, 0x01, 0x00, 0x01,
  0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x03,
  0x82, 0x01, 0x01, 0x00, 0x78, 0x64, 0xCD, 0x6F, 0xC0, 0xF9, 0xE7, 0xAB, 0x19, 0x0B, 0xA4, 0x92,
  0x88, 0xD7, 0xE3, 0xC4, 0x9C, 0xCF, 0xF2, 0x49, 0xFC, 0xCE, 0x8C, 0xEC, 0x33, 0x3F, 0x24, 0x6F,
  0xF7, 0xBC, 0x7C, 0x28, 0xEF, 0xAE, 0xAB, 0x7B, 0xAF, 0x84, 0xE3, 0x4E, 0x38, 0xAB, 0x66, 0x20,
  0x7C, 0xB9, 0xE0, 0x37, 0xFD, 0x84, 0x28, 0x3C, 0xEE, 0x35, 0xC4, 0x75, 0x04, 0x43, 0xCA, 0x4F,
  0x36, 0x60, 0xC4, 0x06, 0x16, 0x54, 0x3C, 0xF9, 0x47, 0x78, 0xBD, 0xE9, 0x7F, 0xF1, 0xE3, 0x0B,
  0xC7, 0x7E, 0x30, 0xA4, 0xCB, 0x3A, 0xC7, 0xCD, 0x50, 0x9A, 0x6C, 0xC1, 0xE3, 0xF8, 0xF8, 0x16,
  0xAA, 0x67, 0x7F, 0x92, 0xEE, 0x1D, 0x47, 0xF2, 0x96, 0xDF, 0xD7, 0x43, 0x64, 0x78, 0xA7, 0xD9,
  0xF6, 0xA1, 0xD9, 0xEB, 0x5B, 0x2E, 0xC6, 0x9E, 0xEA, 0xD5, 0x04, 0x52, 0xA4, 0x73, 0x97, 0xC8,
  0xB3, 0xFD, 0xE1, 0x7E, 0xB1, 0xA4, 0x3F, 0xF4, 0x56, 0x66, 0x8E, 0x6B, 0xBC, 0xD8, 0xF0, 0x7C,
  0xBF, 0x58, 0xA4, 0xE0, 0x45, 0xDC, 0x2B, 0x0B, 0x2C, 0x40, 0x52, 0x4D, 0x4B, 0xA9, 0x8A, 0x77,
  0x87, 0x31, 0xF8, 0x45, 0xD9, 0x60, 0xBB, 0xAD, 0x28, 0x29, 0x14, 0x0D, 0x0D, 0x8F, 0x0F, 0x34,
  0x76, 0x88, 0xF3, 0x5A, 0x65, 0x00, 0x54, 0x22, 0x35, 0xBF, 0x3A, 0x2B, 0xE9, 0xB0, 0x24, 0xE5,
  0xBE, 0xDF, 0x08, 0xA0, 0x02, 0xB2, 0x81, 0x01, 0xE8, 0x14, 0xBE, 0x6E, 0x7E, 0xBD, 0x60, 0x0C,
  0x4A, 0xD8, 0x57, 0xA4, 0x50, 0xD3, 0xD0, 0x59, 0xCE, 0x57, 0x27, 0xE9, 0xA6, 0x16, 0x15, 0x5F,
  0xF2, 0x6C, 0xCC, 0x57, 0x36, 0x20, 0x06, 0xE6, 0xA7, 0x24, 0x22, 0xB0, 0xB1, 0xC4, 0xBC, 0x2C,
  0x5C, 0xCE, 0x92, 0x11, 0xFF, 0x6F, 0x8C, 0x14, 0x7E, 0xC4, 0x10, 0x02, 0xF6, 0x69, 0x86, 0x6D,
  0x5B, 0x64, 0x81, 0x13,
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  PrivateKey[] = {
  0x30, 0x82, 0x04, 0xA4, 0x02, 0x01, 0x00, 0x02, 0x82, 0x01, 0x01, 0x00, 0xBC, 0xE4, 0x67, 0xDC,
  0xC7, 0xEA, 0x6F, 0x8A, 0xA7, 0xCC, 0xB2, 0x54, 0x47, 0x48, 0x6A, 0xE2, 0x39, 0xFF, 0xC2, 0x48,
  0x58, 0x34, 0x07, 0x03, 0x6D, 0x39, 0xB3, 0x67, 0x46, 0x4C, 0xBC, 0xA0, 0xFA, 0x4E, 0x64, 0x23,
  0x56, 0x47, 0x7B, 0xC9, 0x1A, 0x2A, 0x55, 0x42, 0x54, 0x10, 0x18, 0x30, 0x92, 0x60, 0x30, 0x5B,
  0x9E, 0xC0, 0x65, 0xD2, 0xD4, 0x05, 0x4A, 0xA6, 0x10, 0x66, 0x04, 0xA9, 0x54, 0x4E, 0xEE, 0x49,
  0x39, 0x43, 0x65, 0x1E, 0x2E, 0x28, 0xDE, 0x79, 0x24, 0xA9, 0x7E, 0xD8, 0x5B, 0xBC, 0x2F, 0x46,
  0x6A, 0xB7, 0xB6, 0x0D, 0x17, 0x88, 0x37, 0x52, 0x5C, 0xFE, 0x93, 0xC0, 0xE2, 0xFD, 0x6A, 0x08,
  0x1B, 0xFB, 0xD1, 0x87, 0xBD, 0xBD, 0x58, 0x57, 0x2C, 0x06, 0x5D, 0xD2, 0x7D, 0x52, 0xE2, 0x49,
  0x8E, 0xDC, 0xE5, 0x26, 0xBD, 0x92, 0x60, 0xB0, 0x3F, 0x58, 0x5E, 0x52, 0xD7, 0x91, 0xDA, 0x93,
  0x62, 0x8D, 0x71, 0x80, 0x53, 0xBA, 0x15, 0xC4, 0x1F, 0xF3, 0xBD, 0xE0, 0xC5, 0xA4, 0xB8, 0xD3,
  0x64, 0x12, 0x14, 0x1B, 0x11, 0x6B, 0x7B, 0xC2, 0x92, 0xC7, 0xE2, 0x94, 0x0B, 0xB8, 0x67, 0x38,
  0x48, 0x63, 0x11, 0x74, 0x25, 0x7C, 0x37, 0xC3, 0xB2, 0xAE, 0xD9, 0xA7, 0x17, 0x9C, 0x4B, 0x9D,
  0x6C, 0x27, 0xB0, 0x87, 0x16, 0x6B, 0xF2, 0x96, 0xE5, 0x1D, 0x37, 0x27, 0xDE, 0xF2, 0x98, 0xB7,
  0x81, 0x08, 0xD9, 0x7A, 0xBA, 0x84, 0x14, 0x61, 0x60, 0x48, 0xCE, 0xCE, 0x51, 0x73, 0xF4, 0xDB,
  0xF1, 0x5F, 0x7A, 0x17, 0x71, 0x4F, 0xC1, 0x0B, 0xCE, 0xC7, 0x31, 0xC1, 0x4E, 0xA3, 0xEE, 0x6F,
  0x72, 0x97, 0x90, 0xFB, 0x8B, 0x54, 0x9F, 0x82, 0x5B, 0x48, 0x5A, 0xF1, 0xAD, 0x8B, 0x3A, 0xCD,
  0xCA, 0xB2, 0x8B, 0x7A, 0x53, 0xD4, 0xF7, 0x71, 0x16, 0x75, 0xA7, 0x35, 0x02, 0x03, 0x01, 0x00,
  0x01, 0x02, 0x82, 0x01, 0x00, 0x13, 0xF7, 0xD1, 0x42, 0xF5, 0x9F, 0x42, 0xCB, 0x55, 0x91, 0xBE,
  0x08, 0x4A, 0xC0, 0xCD, 0x0B, 0xBD, 0x35, 0xDC, 0x43, 0xE9, 0x8F, 0x16, 0x6E, 0xB6, 0x4D, 0x33,
  0x39, 0xE7, 0xA4, 0x95, 0x0C, 0x2F, 0x69, 0xBA, 0x0C, 0x42, 0x42, 0xAC, 0x43, 0x46, 0x10, 0xD3,
  0x92, 0x7F, 0x70, 0x74, 0x1E, 0x2E, 0x5B, 0x1C, 0xC1, 0x92, 0xB6, 0xA4, 0x0C, 0xF5, 0x7C, 0xD9,
  0xB7, 0x54, 0x64, 0x74, 0x79, 0xB1, 0xFF, 0xE6, 0x10, 0xB7, 0x8C, 0xF8, 0x53, 0x88, 0x6D, 0xA9,
  0x97, 0x04, 0xD9, 0x26, 0x1F, 0x99, 0x12, 0xFB, 0xAC, 0x65, 0xFB, 0xA5, 0xB3, 0x1C, 0x99, 0xB9,
  0xBF, 0x6B, 0x35, 0x3E, 0x49, 0x55, 0xB5, 0x94, 0x4F, 0xE7, 0x25, 0x67, 0xB1, 0x01, 0xCD, 0xD2,
  0x58, 0xE4, 0xBE, 0x87, 0x8C, 0x88, 0xD3, 0x0A, 0x38, 0xDC, 0x71, 0x5D, 0x88, 0x0A, 0xE2, 0x3E,
  0x76, 0x63, 0x3B, 0xE4, 0x3C, 0x8F, 0x2F, 0x29, 0x1D, 0xD1, 0x66, 0x8D, 0xC0, 0x4A, 0x68, 0x15,
  0x90, 0x4C, 0x95, 0x61, 0xF4, 0xFD, 0xE8, 0xFA, 0x9C, 0x6C, 0x00, 0x22, 0x23, 0xD5, 0x17, 0x6E,
  0xEE, 0xA8, 0xD8, 0x70, 0xC5, 0x74, 0xEA, 0x09, 0x13, 0x7F, 0x0C, 0x37, 0x4D, 0x50, 0xCD, 0xE9,
  0x16, 0xC2, 0xD5, 0xDE, 0x5E, 0xC3, 0xFC, 0x46, 0x08, 0xF1, 0x99, 0xC0, 0xB4, 0x28, 0xFD, 0x2B,
  0x29, 0xEF, 0x76, 0xD7, 0x04, 0x4F, 0x02, 0x54, 0x16, 0x54, 0x55, 0x20, 0xEC, 0xBC, 0xBF, 0x85,
  0x5F, 0x12, 0xCC, 0xFC, 0x0D, 0xF2, 0xEF, 0xFC, 0x4D, 0x3E, 0xA2, 0x5E, 0x97, 0xFE, 0x35, 0x10,
  0x0F, 0x53, 0x1F, 0x80, 0xD5, 0xC0, 0xB4, 0xE9, 0xE9, 0x31, 0x4C, 0x89, 0x14, 0x72, 0x39, 0x65,
  0x89, 0xEF, 0x7A, 0x51, 0x4A, 0xB9, 0xA9, 0xCC, 0x1B, 0x52, 0xB0, 0x02, 0x52, 0x65, 0x2F, 0x0B,
  0x89, 0x41, 0x70, 0x1E, 0x01, 0x02, 0x81, 0x81, 0x00, 0xDB, 0xE8, 0x21, 0x9E, 0xDB, 0x58, 0x42,
  0x0B, 0x02, 0x35, 0x87, 0xDF, 0x35, 0xF4, 0xC1, 0xEE, 0xD5, 0x72, 0x2C, 0xDC, 0x4D, 0xC7, 0xDA,
  0x6D, 0x18, 0x0F, 0x68, 0x6F, 0xE7, 0xB0, 0x6C, 0xA0, 0xB2, 0x9E, 0xB8, 0x5E, 0x4A, 0x86, 0x81,
  0x97, 0xBD, 0x78, 0xFC, 0x06, 0xD5, 0xBE, 0x98, 0x42, 0x38, 0x78, 0x78, 0x16, 0xAE, 0x39, 0x56,
  0xFB, 0x67, 0x6B, 0xED, 0x7F, 0xB3, 0xC3, 0x7D, 0x95, 0x86, 0x05, 0x0C, 0xF1, 0x4C, 0xC2, 0x67,
  0x94, 0x93, 0x1B, 0xB7, 0x91, 0x0A, 0x80, 0xE2, 0x85, 0xA7, 0x75, 0x20, 0x61, 0xCD, 0x40, 0x5B,
  0x39, 0x13, 0x32, 0xF7, 0xB8, 0xD7, 0x0C, 0x39, 0x4C, 0x82, 0x4B, 0xA3, 0x9F, 0x5E, 0xB1, 0xB0,
  0x38, 0x81, 0x8B, 0x1E, 0xBD, 0x2B, 0xC2, 0x9F, 0x3A, 0x68, 0x72, 0xC2, 0x90, 0xCB, 0xA2, 0xE2,
  0xAC, 0x86, 0xFF, 0xC8, 0xD7, 0xB8, 0x97, 0x0E, 0x69, 0x02, 0x81, 0x81, 0x00, 0xDB, 0xE5, 0x21,
  0x1F, 0x4C, 0x83, 0xD9, 0x7A, 0x56, 0x87, 0x82, 0xF1, 0x40, 0x1A, 0xD3, 0x5B, 0xC5, 0xB4, 0x4D,
  0x18, 0x8A, 0xE3, 0x6B, 0xC9, 0x7E, 0x8F, 0x5D, 0x00, 0x33, 0x66, 0xEB, 0xAE, 0x8E, 0x54, 0x49,
  0xD7, 0xEA, 0xF3, 0xF9, 0xED, 0xD7, 0xEE, 0x2C, 0xC8, 0xD6, 0x8F, 0x40, 0x0A, 0x53, 0xCB, 0x5D,
  0x16, 0xB7, 0xEB, 0x4E, 0xFB, 0x11, 0x18, 0x1B, 0x4B, 0x6B, 0x43, 0x04, 0xD6, 0x1D, 0x8F, 0x6D,
  0x4D, 0x48, 0x8B, 0x6C, 0x82, 0x52, 0xB4, 0x8D, 0x58, 0x9B, 0x19, 0x8D, 0xE0, 0x0D, 0xFD, 0x15,
  0x9D, 0x35, 0xDF, 0x5E, 0xED, 0xDB, 0x8F, 0x8C, 0x44, 0x11, 0xC9, 0x9A, 0xD9, 0x3F, 0xD4, 0xA5,
  0xEE, 0x68, 0x3B, 0x38, 0xD1, 0x66, 0xB0, 0x62, 0x7E, 0xA5, 0x04, 0xD1, 0x67, 0xA4, 0x1D, 0x93,
  0xD6, 0x2A, 0xFB, 0xF6, 0xC1, 0xB2, 0xC4, 0x02, 0x8A, 0xB2, 0xA8, 0xD0, 0xED, 0x02, 0x81, 0x81,
  0x00, 0xD1, 0x1D, 0x98, 0x43, 0x09, 0x10, 0x8F, 0x95, 0x0A, 0x5F, 0xBF, 0x3A, 0x1A, 0x84, 0x0A,
  0x9B, 0x44, 0x6C, 0x53, 0x8E, 0x97, 0x84, 0x72, 0x08, 0x85, 0xF5, 0xB8, 0xF9, 0x99, 0x58, 0x22,
  0xA4, 0x59, 0xD7, 0x27, 0x39, 0xBB, 0x22, 0xD7, 0x3F, 0x65, 0x95, 0x14, 0x0C, 0xA7, 0xDB, 0x9B,
  0xCC, 0x75, 0x08, 0xD2, 0x21, 0xB7, 0x41, 0x9D, 0x9E, 0x6A, 0x85, 0xAE, 0x8E, 0x5E, 0x72, 0x8B,
  0x0C, 0xD2, 0x2A, 0x21, 0x59, 0xA1, 0x05, 0xC9, 0x1B, 0xF7, 0x1C, 0x47, 0x5E, 0x6C, 0x9F, 0x8D,
  0x49, 0x4D, 0xA9, 0xCA, 0xC6, 0x56, 0x39, 0x59, 0xC6, 0x17, 0xA4, 0xC2, 0xFF, 0x09, 0xB8, 0x7D,
  0x10, 0x6D, 0x62, 0xE2, 0x50, 0x9D, 0xEA, 0xD8, 0xC9, 0x93, 0x1E, 0xA9, 0x77, 0x78, 0x92, 0x95,
  0x7A, 0x24, 0x72, 0xE4, 0xE4, 0x6E, 0x51, 0x11, 0xBE, 0xC0, 0x8D, 0xA1, 0xFE, 0xC9, 0x71, 0x13,
  0xE9, 0x02, 0x81, 0x80, 0x38, 0xE5, 0x3F, 0x3D, 0xB6, 0xF1, 0x82, 0x8A, 0xD9, 0xEA, 0xB4, 0x21,
  0xB1, 0xE1, 0x7F, 0x00, 0x86, 0xD6, 0x11, 0x00, 0xA1, 0xA7, 0x18, 0x53, 0x89, 0x50, 0x94, 0xD2,
  0xA1, 0x7A, 0x26, 0xCF, 0x02, 0x7A, 0xA2, 0xA4, 0xA0, 0x61, 0x42, 0x30, 0x9D, 0x27, 0x3E, 0x25,
  0x35, 0x06, 0xD9, 0x9E, 0x12, 0x72, 0x53, 0x02, 0xB6, 0xD1, 0xC6, 0x9C, 0xC5, 0x9E, 0x84, 0x29,
  0x6E, 0x76, 0x67, 0xC0, 0xF2, 0xE3, 0x12, 0x77, 0xE5, 0xA6, 0x55, 0x8C, 0x1E, 0xD4, 0x22, 0xA2,
  0x63, 0x77, 0x28, 0x33, 0x69, 0xCB, 0x2B, 0x15, 0x83, 0x43, 0xE9, 0x6F, 0x07, 0xD9, 0x4B, 0x84,
  0xAD, 0x49, 0x89, 0xAC, 0x5E, 0x8C, 0xA9, 0xE6, 0x56, 0xC7, 0xC3, 0xFA, 0xC2, 0x64, 0x2F, 0x07,
  0xEF, 0xB0, 0xA0, 0x71, 0xFD, 0x8F, 0xBD, 0x99, 0xB3, 0x83, 0x80, 0x24, 0x3C, 0x19, 0xE5, 0x42,
  0x77, 0xD9, 0xA0, 0x3D, 0x02, 0x81, 0x81, 0x00, 0xB7, 0x35, 0xE9, 0x66, 0x4A, 0xCD, 0x5F, 0x5C,
  0x44, 0x9A, 0x7F, 0x23, 0xAE, 0xC2, 0x7F, 0xF0, 0x8D, 0xE4, 0xDB, 0xCD, 0x1A, 0x8A, 0x40, 0xD7,
  0x0A, 0xC4, 0x3E, 0xC5, 0x03, 0xFB, 0x47, 0xAB, 0x72, 0xDF, 0xD7, 0x68, 0x27, 0x6E, 0x86, 0x94,
  0x2A, 0x2A, 0xDA, 0x25, 0x9E, 0xF6, 0x5E, 0xE0, 0x6D, 0x7C, 0x7D, 0xFE, 0x72, 0xDD, 0xC7, 0xBC,
  0x74, 0xC7, 0xAD, 0xEA, 0x44, 0xBE, 0x5C, 0x8A, 0x03, 0xCD, 0x35, 0x74, 0x6F, 0x4E, 0x92, 0x31,
  0xBE, 0x57, 0x9A, 0x02, 0x0C, 0xFC, 0xD1, 0xD5, 0xE7, 0x3E, 0x53, 0x0C, 0x47, 0x12, 0x39, 0x45,
  0x6E, 0x35, 0x24, 0xA6, 0xDA, 0x42, 0x63, 0xC6, 0xBF, 0xCA, 0xE8, 0x32, 0xFD, 0x61, 0xC0, 0x80,
  0x39, 0xD2, 0xB7, 0x0D, 0xF3, 0xA2, 0xF3, 0xE4, 0x49, 0x26, 0x89, 0xD0, 0xA0, 0x24, 0xC8, 0x27,
  0x86, 0x10, 0x09, 0x88, 0x6C, 0x35, 0x60, 0xF2,
};

UNIT_TEST_STATUS
EFIAPI
TestVerifyOaepEncrypt (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Status;
  UINT8    File[4];
  UINT8    *OutBuffer;
  UINTN    OutBufferSize;
  UINT8    *OutBuffer2;
  UINTN    OutBuffer2Size;

  // Create a file and add content '123' in it
  File[0] = '1';
  File[1] = '2';
  File[2] = '3';
  File[3] = 0;

  OutBuffer      = NULL;
  OutBufferSize  = 0;
  OutBuffer2     = NULL;
  OutBuffer2Size = 0;

  Status = Pkcs1v2Encrypt (
             SelfTestCert,
             (UINTN)sizeof (SelfTestCert),
             File,
             (UINTN)sizeof (File),
             NULL,
             0,
             &OutBuffer,
             (UINTN *)&OutBufferSize
             );
  UT_ASSERT_TRUE (Status);

  Status = Pkcs1v2Encrypt (
             SelfTestCert,
             (UINTN)sizeof (SelfTestCert),
             File,
             (UINTN)4,
             NULL,
             0,
             &OutBuffer2,
             (UINTN *)&OutBuffer2Size
             );
  UT_ASSERT_TRUE (Status);

  // TRUE - the two OutBuffers are indentical. That means the Oaep encrypt result is incorrect.
  Status = (CompareMem (OutBuffer, OutBuffer2, OutBufferSize >= OutBuffer2Size ? OutBufferSize : OutBuffer2Size) == 0);
  UT_ASSERT_FALSE (Status);

  if (OutBuffer) {
    FreePool (OutBuffer);
    OutBuffer     = NULL;
    OutBufferSize = 0;
  }

  if (OutBuffer2) {
    FreePool (OutBuffer2);
    OutBuffer2     = NULL;
    OutBuffer2Size = 0;
  }

  Status = Pkcs1v2Encrypt (
             SelfTestCert,
             (UINTN)sizeof (SelfTestCert),
             File,
             (UINTN)4,
             RandSeed,
             (UINTN)sizeof (RandSeed),
             &OutBuffer,
             (UINTN *)&OutBufferSize
             );
  UT_ASSERT_TRUE (Status);

  Status = Pkcs1v2Encrypt (
             SelfTestCert,
             (UINTN)sizeof (SelfTestCert),
             File,
             (UINTN)4,
             RandSeed,
             (UINTN)sizeof (RandSeed),
             &OutBuffer2,
             (UINTN *)&OutBuffer2Size
             );
  UT_ASSERT_TRUE (Status);

  // TRUE - the two OutBuffers are indentical. That means the Oaep encrypt result is incorrect.
  Status = (CompareMem (OutBuffer, OutBuffer2, OutBufferSize >= OutBuffer2Size ? OutBufferSize : OutBuffer2Size) == 0);
  UT_ASSERT_FALSE (Status);

  if (OutBuffer) {
    FreePool (OutBuffer);
    OutBuffer     = NULL;
    OutBufferSize = 0;
  }

  if (OutBuffer2) {
    FreePool (OutBuffer2);
    OutBuffer2     = NULL;
    OutBuffer2Size = 0;
  }

  Status = Pkcs1v2Encrypt (
             NULL,
             (UINTN)sizeof (SelfTestCert),
             File,
             (UINTN)4,
             NULL,
             0,
             &OutBuffer,
             (UINTN *)&OutBufferSize
             );
  UT_ASSERT_FALSE (Status);

  Status = Pkcs1v2Encrypt (
             SelfTestCert,
             (UINTN)sizeof (SelfTestCert),
             File,
             (UINTN)4,
             NULL,
             0,
             (UINT8 **)NULL,
             (UINTN *)&OutBufferSize
             );
  UT_ASSERT_FALSE (Status);

  Status = Pkcs1v2Encrypt (
             SelfTestCert,
             (UINTN)sizeof (SelfTestCert),
             File,
             (UINTN)4,
             NULL,
             0,
             &OutBuffer,
             (UINTN *)NULL
             );
  UT_ASSERT_FALSE (Status);

  return UNIT_TEST_PASSED;
}

TEST_DESC  mOaepTest[] = {
  //
  // -----Description--------------------------------------Class----------------------Function-----------------Pre---Post--Context
  //
  { "TestVerifyOaepEncrypt()", "CryptoPkg.BaseCryptLib.Pkcs1v2Encrypt", TestVerifyOaepEncrypt, NULL, NULL, NULL },
};

UINTN  mOaepTestNum = ARRAY_SIZE (mOaepTest);
