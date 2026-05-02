# Weno Fit OS SDK Philosophy

The Weno Fit OS SDK is designed to be a simple, efficient, and flexible platform for building apps for Weno Fit OS smart health trackers.

## Core Principles

*   **Simplicity:** The SDK should be easy to learn and use, even for developers new to embedded systems or JavaScript. We achieve this by providing a high-level API that abstracts away the complexities of the underlying hardware, exposed through JavaScript and the Elk embedded JavaScript engine.

*   **Efficiency:** Apps built with the SDK should be lightweight and optimized for low power consumption. We provide tools and guidance to help developers write efficient JavaScript code and minimize battery usage on embedded devices.

*   **Flexibility:** The SDK should provide enough flexibility for developers to create a wide range of apps, from simple watchfaces to complex fitness trackers, using the familiar and powerful JavaScript language. We expose a rich set of APIs for accessing device features and creating custom user interfaces.

*   **Safety & Security:** The OS is designed to be robust and protect itself from misbehaving apps. All third-party applications *must be digitally signed* by approved developers. The SDK includes tools for developers to sign their applications, and the OS verifies these signatures before execution to ensure authenticity and integrity.
