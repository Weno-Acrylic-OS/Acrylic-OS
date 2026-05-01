# Weno Fit OS SDK - Security and App Signing

To ensure the integrity, authenticity, and security of applications running on Weno Fit OS devices, all third-party applications must be digitally signed by approved developers. The Weno Fit OS enforces strict signature verification before launching any application.

## 1. App Signing for Developers

Developers will use a provided SDK tool to sign their application packages.

### Process:

1.  **Generate Keys:** Developers generate a public/private key pair (e.g., using standard cryptographic tools like OpenSSL or a custom SDK tool). The private key is kept secret, and the public key (often embedded in a certificate) is provided to Weno Fit OS for approval.
2.  **Package Application:** The application's JavaScript/Elk code, `app.json` manifest, and any assets are bundled into a single package (e.g., a `.wfo` file, a custom archive format).
3.  **Sign Package:** Using the SDK's signing tool and their private key, developers generate a digital signature for the application package. This signature is typically appended to the package or included as a separate file within the package.

### SDK Signing Tool (Conceptual):

A command-line tool, e.g., `wfo-signer`, would facilitate this:

```bash
wfo-signer --package my_app.zip --private-key developer_private.pem --output my_app.wfo
```

## 2. OS-Level Signature Verification

The Weno Fit OS includes a robust mechanism to verify application signatures before execution.

### Process:

1.  **Trusted Key Store:** The Weno Fit OS device maintains a secure, immutable store of public keys (or certificates) belonging to approved developers. This store is part of the OS firmware and cannot be tampered with by unauthorized parties.
2.  **App Installation/Launch Request:** When a user attempts to install or launch an application, the OS intercepts the request.
3.  **Signature Extraction:** The OS extracts the digital signature and the application's content from the app package.
4.  **Verification:** The OS uses the corresponding public key from its trusted store to verify the extracted signature against the application's content.
5.  **Execution Policy:**
    *   If the signature is **valid** and matches a **trusted public key**, the application is allowed to launch.
    *   If the signature is **invalid**, **missing**, or corresponds to an **untrusted public key**, the application launch is denied, and an error is reported to the user.

### Key Considerations:

*   **Key Management:** A secure process for developers to submit their public keys for inclusion in the OS's trusted store will be crucial.
*   **Revocation:** Mechanisms for revoking compromised developer keys or specific app signatures will be necessary for long-term security.
*   **Performance:** Signature verification should be efficient to minimize app launch times.
