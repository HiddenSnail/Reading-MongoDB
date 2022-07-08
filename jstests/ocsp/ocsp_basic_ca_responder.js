// Check that OCSP verification works
// @tags: [requires_http_client]

load("jstests/ocsp/lib/mock_ocsp.js");

(function() {
"use strict";

if (determineSSLProvider() === "apple") {
    return;
}
function test(serverCert, caCert, responderCertPair) {
    clearOCSPCache();

    const ocsp_options = {
        sslMode: "requireSSL",
        sslPEMKeyFile: serverCert,
        sslCAFile: caCert,
        sslAllowInvalidHostnames: "",
        setParameter: {
            "failpoint.disableStapling": "{'mode':'alwaysOn'}",
            "ocspEnabled": "true",
        },
    };

    // This is to test what happens when the responder is down,
    // making sure that we soft fail.
    let conn = null;

    let mock_ocsp = new MockOCSPServer("", 1, responderCertPair);
    mock_ocsp.start();

    assert.doesNotThrow(() => {
        conn = MongoRunner.runMongod(ocsp_options);
    });

    mock_ocsp.stop();
    mock_ocsp = new MockOCSPServer(FAULT_REVOKED, 1, responderCertPair);
    mock_ocsp.start();

    assert.throws(() => {
        new Mongo(conn.host);
    });

    MongoRunner.stopMongod(conn);

    // The mongoRunner spawns a new Mongo Object to validate the collections which races
    // with the shutdown logic of the mock_ocsp responder on some platforms. We need this
    // sleep to make sure that the threads don't interfere with each other.
    sleep(1000);
    mock_ocsp.stop();
}

test(OCSP_SERVER_CERT, OCSP_CA_PEM, OCSP_CA_RESPONDER);

// TODO: SERVER-47963 - remove this platform check.
if (determineSSLProvider() === "windows") {
    return;
}

test(OCSP_SERVER_INTERMEDIATE_CA_CERT, OCSP_INTERMEDIATE_CA_PEM, OCSP_INTERMEDIATE_RESPONDER);
}());