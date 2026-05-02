# Weno Fit OS SDK - SDK Structure

The Weno Fit OS SDK is organized to provide a clear and intuitive development environment for third-party application developers. This document outlines the proposed directory structure of the SDK.

```
weno-fit-os-sdk/
├── bin/                      # Executable command-line tools
│   ├── wfo-cli               # The main Weno Fit OS CLI tool
│   └── wfo-signer            # Tool for digitally signing application packages
├── docs/                     # Comprehensive SDK documentation
│   ├── philosophy.md         # SDK's core principles and goals
│   ├── app_manifest.md       # Definition of the app.json manifest file
│   ├── api_reference.md      # Conceptual API reference for JS/Elk bindings
│   ├── security_and_signing.md # Details on app signing and OS verification
│   ├── toolchain.md          # Overview of the development toolchain
│   └── getting_started.md    # A guide to quickly set up and build the first app (to be populated)
├── examples/                 # Sample applications demonstrating SDK usage
│   ├── hello_world_app/      # A basic "Hello, World!" app
│   │   ├── app.json
│   │   └── index.js
│   └── clock_app/            # A simple clock application (conceptual example)
│       ├── app.json
│       └── index.js
├── include/                  # C header files for native development (if supported/necessary)
│   └── lvgl/                 # LVGL headers used by the OS
├── lib/                      # Pre-compiled C libraries for the Weno Fit OS platform
│   ├── lvgl/                 # LVGL library binaries
│   └── weno_fit_os_core/     # Core OS service libraries
├── bindings/                 # JavaScript/Elk bindings for native C libraries
│   ├── lvgl_bindings.js      # Bindings for LVGL UI functionalities
│   ├── weno_fit_os_bindings.js # Bindings for WenoFitOS system APIs
│   └── sensors_bindings.js   # Bindings for sensor access APIs
└── templates/                # Project templates for `wfo-cli init` command
    └── basic_app/            # A minimal application project template
        ├── app.json
        └── index.js
```

## Explanation of Directories:

*   **`bin/`**: Contains executable command-line tools that developers will use to interact with the SDK, such as the `wfo-cli` for project management and `wfo-signer` for security.
*   **`docs/`**: Houses all documentation related to the SDK, providing guides, reference materials, and conceptual overviews.
*   **`examples/`**: Offers ready-to-use sample applications that demonstrate various features of the SDK and serve as starting points for new projects.
*   **`include/`**: Stores C header files. While primary app development is in JS/Elk, these might be relevant for understanding the underlying native APIs or for advanced use cases.
*   **`lib/`**: Contains pre-compiled native libraries that form the core of the Weno Fit OS and its functionalities. The JS/Elk bindings will interface with these.
*   **`bindings/`**: Holds the JavaScript/Elk code that provides an interface between the developers' JS/Elk applications and the native C libraries. These files expose the C APIs in a JS-friendly manner.
*   **`templates/`**: Provides predefined project structures that can be used by the `wfo-cli init` command to quickly set up new application projects.
