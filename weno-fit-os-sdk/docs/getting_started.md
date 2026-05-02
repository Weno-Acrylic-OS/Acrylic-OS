# Weno Fit OS SDK - Getting Started Guide

This guide will walk you through setting up your development environment and creating your first application for Weno Fit OS using the SDK.

## 1. Prerequisites

Before you begin, ensure you have the following installed on your development machine:

*   **Node.js (LTS Version):** Required for the `wfo-cli` tool.
*   **Git:** For cloning sample projects and managing your code.
*   **Text Editor or IDE:** (e.g., VS Code, Sublime Text)
*   **Weno Fit OS Device or Web Browser:** For testing your applications.

## 2. Installing the Weno Fit OS SDK

The Weno Fit OS SDK is distributed as a command-line interface (CLI) tool and a collection of libraries and example projects.

To install the `wfo-cli`:

```bash
npm install -g wfo-cli
```

This will install the main CLI tool, which provides access to the entire SDK.

## 3. Creating Your First App

Let's create a simple "Hello, World!" application:

1.  **Initialize a new project:**
    ```bash
    wfo-cli init my-hello-app
    cd my-hello-app
    ```
    This command creates a new directory `my-hello-app` with the basic project structure, including an `app.json` manifest and an `index.js` entry point.

2.  **Explore the project structure:**
    *   `app.json`: Your application's manifest file.
    *   `index.js`: The main JavaScript/Elk file where your application logic resides.

## 4. Code Your App (`index.js`)

Open `my-hello-app/index.js` in your text editor. You'll find a basic example demonstrating how to use the `WenoFitOS` system API and `LVGL` UI bindings:

```javascript
// index.js
WenoFitOS.log("Hello, World! app started.");

let screen = LVGL.createScreen();
let label = LVGL.createLabel(screen);
LVGL.setLabelText(label, "Hello, Weno Fit OS!");
LVGL.alignObject(label, LVGL.ALIGN_CENTER, 0, 0);
```

You can modify this file to add more UI elements, interact with sensors, or send notifications using the APIs described in the [API Reference](api_reference.md).

## 5. Build and Sign Your App

Before your app can run on a Weno Fit OS device or simulator, it needs to be built and digitally signed.

1.  **Build the application package:**
    ```bash
    wfo-cli build
    ```
    This command compiles your JS/Elk code, bundles assets, and creates an unsigned application package (e.g., `my-hello-app.wfo`) in a `dist/` directory.

2.  **Sign the application package:**
    You will need a private key (e.g., `developer_private.pem`) previously generated and approved by Weno Fit OS.
    ```bash
    wfo-cli sign --package dist/my-hello-app.wfo --key developer_private.pem
    ```
    This command adds a digital signature to your application package, making it ready for deployment. Refer to the [Security and App Signing](security_and_signing.md) document for more details on key management.

## 6. Test in the Web Simulator

The Weno Fit OS SDK includes a web-based simulator for quick testing:

```bash
wfo-cli deploy --package dist/my-hello-app.wfo --simulator
```
This will open your default web browser with the Weno Fit OS Simulator running your application. You can interact with your app in the browser.

## 7. Deploy to a Physical Device

To deploy your app to a physical Weno Fit OS device:

1.  **Connect your device:** Connect your Weno Fit OS device to your development machine via USB or another designated interface.
2.  **Deploy the signed package:**
    ```bash
    wfo-cli deploy --package dist/my-hello-app.wfo --device /dev/ttyUSB0
    ```
    (Replace `/dev/ttyUSB0` with the appropriate device path for your system).

## 8. Debugging

The `wfo-cli` provides basic debugging capabilities:

```bash
wfo-cli debug
```
This command might open a remote debugger interface or stream application logs to your console, depending on the current debugging configuration. More advanced debugging features will be detailed in future documentation.

You are now ready to start creating amazing applications for Weno Fit OS!
