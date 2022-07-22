
/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

// List of possible SSL providers
#define MONGO_CONFIG_SSL_PROVIDER_OPENSSL 1
#define MONGO_CONFIG_SSL_PROVIDER_WINDOWS 2
#define MONGO_CONFIG_SSL_PROVIDER_APPLE 3

// Define altivec vec_vbpermq output index
// #undef MONGO_CONFIG_ALTIVEC_VEC_VBPERMQ_OUTPUT_INDEX

// Define if building a debug build
// #undef MONGO_CONFIG_DEBUG_BUILD

// Defined if OpenSSL has `SSL_CTX_set_ecdh_auto` and `SSL_set_ecdh_auto`
#define MONGO_CONFIG_HAVE_SSL_SET_ECDH_AUTO 1

// Defined if OpenSSL has `EC_KEY_new_by_curve_name` and the `NID_X9_62_prime256v1` curve defined
#define MONGO_CONFIG_HAVE_SSL_EC_KEY_NEW 1

// Defined if execinfo.h and backtrace are available
#define MONGO_CONFIG_HAVE_EXECINFO_BACKTRACE 1

// Defined if OpenSSL has the FIPS_mode_set function
#define MONGO_CONFIG_HAVE_FIPS_MODE_SET 1

// Defined if unitstd.h is available
#define MONGO_CONFIG_HAVE_HEADER_UNISTD_H 1

// Defined if memset_s is available
// #undef MONGO_CONFIG_HAVE_MEMSET_S

// Defined if a POSIX monotonic clock is available
#define MONGO_CONFIG_HAVE_POSIX_MONOTONIC_CLOCK 1

// Defined if pthread.h and pthread_setname_np are available
#define MONGO_CONFIG_HAVE_PTHREAD_SETNAME_NP 1

// Defined if strnlen is available
#define MONGO_CONFIG_HAVE_STRNLEN 1

// A number, if we have some extended alignment ability
#define MONGO_CONFIG_MAX_EXTENDED_ALIGNMENT 64

// Defined if building an optimized build
#define MONGO_CONFIG_OPTIMIZED_BUILD 1

// Defined if SSL support is enabled
#define MONGO_CONFIG_SSL 1

// Defined if OpenSSL has SEQUENCE_ANY
#define MONGO_CONFIG_HAVE_ASN1_ANY_DEFINITIONS 1

// Defined if SSL support is enabled with chosen ssl provider
#define MONGO_CONFIG_SSL_PROVIDER MONGO_CONFIG_SSL_PROVIDER_OPENSSL

// Defined if USDT probes are enabled
// #undef MONGO_CONFIG_USDT_ENABLED

// Set to which provider defines USDT
// #undef MONGO_CONFIG_USDT_PROVIDER

// Defined if using libunwind.
#define MONGO_CONFIG_USE_LIBUNWIND 1

// Defined if raw latches are enabled
// #undef MONGO_CONFIG_USE_RAW_LATCHES

// Defined if WiredTiger storage engine is enabled
#define MONGO_CONFIG_WIREDTIGER_ENABLED 1
