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

#include <boost/optional.hpp>
#include <memory>
#include <string>

#include "mongo/config.h"

#ifdef MONGO_CONFIG_SSL

#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobj.h"
#include "mongo/db/service_context.h"
#include "mongo/platform/atomic_word.h"
#include "mongo/util/decorable.h"
#include "mongo/util/net/sock.h"
#include "mongo/util/net/ssl/apple.hpp"
#include "mongo/util/net/ssl_types.h"
#include "mongo/util/out_of_line_executor.h"
#include "mongo/util/time_support.h"

// SChannel implementation
#if MONGO_CONFIG_SSL_PROVIDER == MONGO_CONFIG_SSL_PROVIDER_OPENSSL
#include <openssl/err.h>
#include <openssl/ssl.h>
#endif
#endif  // #ifdef MONGO_CONFIG_SSL

namespace mongo {

/*
 * @return the SSL version std::string prefixed with prefix and suffixed with suffix
 */
const std::string getSSLVersion(const std::string& prefix, const std::string& suffix);

/**
 * Validation callback for setParameter 'opensslCipherConfig'.
 */
Status validateOpensslCipherConfig(const std::string&);

/**
 * Validation callback for setParameter 'disableNonTLSConnectionLogging'.
 */
Status validateDisableNonTLSConnectionLogging(const bool&);
}  // namespace mongo

#ifdef MONGO_CONFIG_SSL
namespace mongo {
struct SSLParams;

#if MONGO_CONFIG_SSL_PROVIDER == MONGO_CONFIG_SSL_PROVIDER_OPENSSL
typedef SSL_CTX* SSLContextType;
typedef SSL* SSLConnectionType;
#elif MONGO_CONFIG_SSL_PROVIDER == MONGO_CONFIG_SSL_PROVIDER_WINDOWS
typedef SCHANNEL_CRED* SSLContextType;
typedef PCtxtHandle SSLConnectionType;
#elif MONGO_CONFIG_SSL_PROVIDER == MONGO_CONFIG_SSL_PROVIDER_APPLE
typedef asio::ssl::apple::Context* SSLContextType;
typedef SSLContextRef SSLConnectionType;
#else
#error "Unknown SSL Provider"
#endif


#if MONGO_CONFIG_SSL_PROVIDER == MONGO_CONFIG_SSL_PROVIDER_OPENSSL
/*
 * There are a number of OpenSSL types that we want to be able to use with unique_ptr that have a
 * custom OpenSSL deleter function. This template implements a stateless deleter for types with
 * C free functions:
 * using UniqueX509 = std::unique_ptr<X509, OpenSSLDeleter<decltype(::X509_free), ::X509_free>>;
 */
template <typename Deleter, Deleter* impl>
struct OpenSSLDeleter {
    template <typename Obj>
    void operator()(Obj* ptr) const {
        if (ptr != nullptr) {
            impl(ptr);
        }
    }
};
#endif
/**
 * Maintain per connection SSL state for the Sock class. Used by SSLManagerInterface to perform SSL
 * operations.
 */
class SSLConnectionInterface {
public:
    virtual ~SSLConnectionInterface();
};

class SSLConfiguration {
public:
    bool isClusterMember(StringData subjectName) const;
    bool isClusterMember(SSLX509Name subjectName) const;
    void getServerStatusBSON(BSONObjBuilder*) const;
    Status setServerSubjectName(SSLX509Name name);

    const SSLX509Name& serverSubjectName() const {
        return _serverSubjectName;
    }

    SSLX509Name clientSubjectName;
    Date_t serverCertificateExpirationDate;
    bool hasCA = false;

private:
    SSLX509Name _serverSubjectName;
    std::vector<SSLX509Name::Entry> _canonicalServerSubjectName;
};

// These represent the ASN.1 type bytes for strings used in an X509 DirectoryString
constexpr int kASN1BMPString = 30;
constexpr int kASN1IA5String = 22;
constexpr int kASN1OctetString = 4;
constexpr int kASN1PrintableString = 19;
constexpr int kASN1TeletexString = 20;
constexpr int kASN1UTF8String = 12;
constexpr int kASN1UniversalString = 28;

/**
 * Stores information about a globally unique OID.
 */
class ASN1OID {
public:
    ASN1OID(std::string identifier, std::string shortDescription, std::string longDescription)
        : identifier(std::move(identifier)),
          shortDescription(std::move(shortDescription)),
          longDescription(std::move(longDescription)) {}

    std::string identifier;        // An OID
    std::string shortDescription;  // A brief description of the entity associated with the OID
    std::string longDescription;   // A long form description of the entity associated with the OID
};
const ASN1OID mongodbRolesOID("1.3.6.1.4.1.34601.2.1.1",
                              "MongoRoles",
                              "Sequence of MongoDB Database Roles");

/**
 * Counts of negogtiated version used by TLS connections.
 */
struct TLSVersionCounts {
    AtomicWord<long long> tlsUnknown;
    AtomicWord<long long> tls10;
    AtomicWord<long long> tls11;
    AtomicWord<long long> tls12;
    AtomicWord<long long> tls13;

    static TLSVersionCounts& get(ServiceContext* serviceContext);
};

class SSLManagerInterface : public Decorable<SSLManagerInterface> {
public:
    static std::unique_ptr<SSLManagerInterface> create(const SSLParams& params, bool isServer);

    virtual ~SSLManagerInterface();

    /**
     * Initiates a TLS connection.
     * Throws SocketException on failure.
     * @return a pointer to an SSLConnectionInterface. Resources are freed in
     * SSLConnectionInterface's destructor
     */
    virtual SSLConnectionInterface* connect(Socket* socket) = 0;

    /**
     * Waits for the other side to initiate a TLS connection.
     * Throws SocketException on failure.
     * @return a pointer to an SSLConnectionInterface. Resources are freed in
     * SSLConnectionInterface's destructor
     */
    virtual SSLConnectionInterface* accept(Socket* socket, const char* initialBytes, int len) = 0;

    /**
     * Fetches a peer certificate and validates it if it exists
     * Throws NetworkException on failure
     * @return a std::string containing the certificate's subject name.
     *
     * This version of parseAndValidatePeerCertificate is deprecated because it throws a
     * NetworkException upon failure. New code should prefer the version that returns
     * a StatusWith instead.
     */
    virtual SSLPeerInfo parseAndValidatePeerCertificateDeprecated(
        const SSLConnectionInterface* conn,
        const std::string& remoteHost,
        const HostAndPort& hostForLogging) = 0;

    /**
     * Gets the SSLConfiguration containing all information about the current SSL setup
     * @return the SSLConfiguration
     */
    virtual const SSLConfiguration& getSSLConfiguration() const = 0;

#if MONGO_CONFIG_SSL_PROVIDER == MONGO_CONFIG_SSL_PROVIDER_OPENSSL
    /**
     * Fetches the error text for an error code, in a thread-safe manner.
     */
    static std::string getSSLErrorMessage(int code) {
        // 120 from the SSL documentation for ERR_error_string
        static const size_t msglen = 120;

        char msg[msglen];
        ERR_error_string_n(code, msg, msglen);
        return msg;
    }
#endif

    /**
     * SSL wrappers
     */
    virtual int SSL_read(SSLConnectionInterface* conn, void* buf, int num) = 0;

    virtual int SSL_write(SSLConnectionInterface* conn, const void* buf, int num) = 0;

    virtual int SSL_shutdown(SSLConnectionInterface* conn) = 0;

    enum class ConnectionDirection { kIncoming, kOutgoing };

    /**
     * Initializes an OpenSSL context according to the provided settings. Only settings which are
     * acceptable on non-blocking connections are set. "direction" specifies whether the SSL_CTX
     * will be used to make outgoing connections or accept incoming connections.
     */
    virtual Status initSSLContext(SSLContextType context,
                                  const SSLParams& params,
                                  ConnectionDirection direction) = 0;

    /**
     * Fetches a peer certificate and validates it if it exists. If validation fails, but weak
     * validation is enabled, the `subjectName` will be empty. If validation fails, and invalid
     * certificates are not allowed, a non-OK status will be returned. If validation is successful,
     * the `subjectName` will contain  the certificate's subject name, and any roles acquired by
     * X509 authorization will be returned in `roles`.
     * Further, the SNI Name will be captured into the `sni` value, when available.
     * The reactor is there to continue the execution of the chained statements to the Future
     * returned by OCSP validation. Can be a nullptr, but will make this function synchronous and
     * single threaded.
     */
    virtual Future<SSLPeerInfo> parseAndValidatePeerCertificate(SSLConnectionType ssl,
                                                                boost::optional<std::string> sni,
                                                                const std::string& remoteHost,
                                                                const HostAndPort& hostForLogging,
                                                                const ExecutorPtr& reactor) = 0;

    /**
     * No-op function for SChannel and SecureTransport. Attaches stapled OCSP response to the
     * SSL_CTX obect.
     */
    virtual Status stapleOCSPResponse(SSLContextType context) = 0;
};

// Access SSL functions through this instance.
SSLManagerInterface* getSSLManager();

extern bool isSSLServer;

/**
 * Returns true if the `nameToMatch` is a valid match against the `certHostName` requirement from an
 * x.509 certificate.  Matches a remote host name to an x.509 host name, including wildcards.
 */
bool hostNameMatchForX509Certificates(std::string nameToMatch, std::string certHostName);

/**
 * Parse a binary blob of DER encoded ASN.1 into a set of RoleNames.
 */
StatusWith<stdx::unordered_set<RoleName>> parsePeerRoles(ConstDataRange cdrExtension);

using DERInteger = std::vector<uint8_t>;

/**
 * Parse a binary blob of DER encoded ASN.1 into a list of features (integers).
 * ASN.1 Integers can be very large, so they are stored in a vector of bytes.
 */
StatusWith<std::vector<DERInteger>> parseTLSFeature(ConstDataRange cdrExtension);

/**
 * Strip the trailing '.' in FQDN.
 */
std::string removeFQDNRoot(std::string name);

/**
 * Escape a string per RFC 2253
 *
 * See "2.4 Converting an AttributeValue from ASN.1 to a String" in RFC 2243
 */
std::string escapeRfc2253(StringData str);

/**
 * Parse a DN from a string per RFC 4514
 */
StatusWith<SSLX509Name> parseDN(StringData str);

/**
 * These functions map short names for RDN components to numeric OID's and the other way around.
 *
 * The x509ShortNameToOid returns boost::none if no mapping exists for that oid.
 */
std::string x509OidToShortName(StringData name);
boost::optional<std::string> x509ShortNameToOid(StringData name);

/**
 * Platform neutral TLS version enum
 */
enum class TLSVersion {
    kUnknown,
    kTLS10,
    kTLS11,
    kTLS12,
    kTLS13,
};

/**
 * Map SSL version to platform-neutral enum.
 */
StatusWith<TLSVersion> mapTLSVersion(SSLConnectionType conn);

/**
 * Record information about TLS versions and optionally log the TLS version
 */
void recordTLSVersion(TLSVersion version, const HostAndPort& hostForLogging);

/**
 * Emit a warning() explaining that a client certificate is about to expire.
 */
void tlsEmitWarningExpiringClientCertificate(const SSLX509Name& peer);
void tlsEmitWarningExpiringClientCertificate(const SSLX509Name& peer, Days days);

}  // namespace mongo
#endif  // #ifdef MONGO_CONFIG_SSL
