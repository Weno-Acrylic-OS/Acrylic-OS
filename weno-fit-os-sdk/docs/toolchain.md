# Weno Fit OS SDK - Toolchain Setup

The Weno Fit OS SDK provides a comprehensive toolchain to enable developers to build, test, and deploy applications using JavaScript and the Elk embedded engine.

## Key Components:

1.  **Elk Embedded JavaScript Engine:**
    *   **Purpose:** The lightweight JavaScript runtime that executes Weno Fit OS applications.
    *   **Developer Access:** The SDK will include the Elk runtime library (to be linked with the OS) and potentially an Elk compiler/transpiler for preparing JavaScript code for embedded execution.

2.  **LVGL Library & Bindings:**
    *   **Purpose:** The UI framework for Weno Fit OS applications.
    *   **Developer Access:** The SDK will provide the necessary C headers and pre-compiled LVGL libraries. Crucially, a set of **JavaScript bindings for LVGL** will be exposed, allowing developers to create and manipulate UI elements directly from their Elk code.

3.  **Weno Fit OS SDK Libraries:**
    *   **Purpose:** Pre-compiled C libraries that implement the core System, Sensor, and Notification APIs (as defined in the API Reference).
    *   **Developer Access:** These libraries will be provided as part of the SDK, and their functionalities will be accessible via JavaScript bindings.

4.  **Weno Fit OS CLI Tool (`wfo-cli`):**
    *   **Purpose:** A command-line interface tool to streamline the development workflow.
    *   **Features:**
        *   **`wfo-cli init <app_name>`:** Initializes a new Weno Fit OS application project with a predefined structure, `app.json` manifest, and a basic `index.js` file.
        *   **`wfo-cli build`:** Compiles the application's JavaScript/Elk code, bundles assets, and packages them into a `.wfo` file, ready for signing.
        *   **`wfo-cli sign --package <file.wfo> --key <private_key.pem>`:** Digitally signs the application package (utilizing the `wfo-signer` utility discussed in the Security section).
        *   **`wfo-cli deploy --package <file.wfo>`:** Deploys the signed application package to a connected Weno Fit OS device or the web simulator.
        *   **`wfo-cli debug`:** Provides tools for debugging running JS/Elk applications (e.g., logging, remote console access).

5.  **Weno Fit OS Web Simulator:**
    *   **Purpose:** An in-browser environment for testing applications without needing physical hardware.
    *   **Access:** Developers can launch the simulator via the `wfo-cli` or directly open the generated `weno_fit_os.html` file. It will accurately emulate the device's display and input.

## Development Workflow Overview:

1.  **Project Creation:** `wfo-cli init MyWatchApp`
2.  **Code Development:** Write `app.js` and other JS/Elk files, define UI using LVGL bindings, interact with Weno Fit OS APIs.
3.  **Build:** `wfo-cli build` to create `MyWatchApp.wfo`.
4.  **Sign:** `wfo-cli sign --package MyWatchApp.wfo --key my_dev_key.pem`
5.  **Test:** `wfo-cli deploy --package MyWatchApp.wfo --simulator` or `wfo-cli deploy --package MyWatchApp.wfo --device /dev/ttyUSB0`
6.  **Debug:** `wfo-cli debug`
