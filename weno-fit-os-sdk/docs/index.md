# Weno Fit OS SDK Documentation

Welcome to the Weno Fit OS SDK! This SDK provides you with the tools and APIs to create your own applications for the Weno Fit OS ecosystem.

This documentation will guide you through setting up your development environment, creating your first app, and using the available APIs.

## Getting Started

### 1. Install the SDK

The Weno Fit OS SDK is distributed as an NPM package. To get started, you need to have Node.js and NPM installed. Then, you can install the SDK command-line tool globally:

```bash
npm install -g weno-fit-os-sdk
```
*(Note: In this interactive session, the package name is `wfo-cli`, but for public distribution, you would likely use a name like `weno-fit-os-sdk`)*

### 2. Create a New Project

Once the SDK is installed, you can create a new project using the `init` command:

```bash
wfo-cli init MyFirstApp
```

This will create a new directory `MyFirstApp` with a basic "Hello World" application.

You can also start with a different template using the `--template` option:

```bash
wfo-cli init MyNotificationApp --template notification_app
```

This will create a new app that demonstrates how to use the Notification API.

### 3. Run the Simulator

To test your app, you can use the web-based simulator. First, deploy your app to the simulator's app directory:

```bash
cd MyFirstApp
wfo-cli build
wfo-cli deploy dist/myfirstapp.wfo
```

Then, run the simulator from the root of the Weno Fit OS project:

```bash
wfo-cli run-simulator
```

This will open the simulator in your browser. You can find your app in the "Apps" list.

## CLI Commands

The `wfo-cli` tool provides several commands to help you with your development workflow:

-   `init <name>`: Creates a new project.
-   `build`: Builds your application and creates a `.wfo` package in the `dist/` directory.
-   `deploy <package>`: Deploys your app package to the simulator.
-   `run-simulator`: Starts the web-based simulator.

For more details on each command, you can use the `--help` option, e.g., `wfo-cli init --help`.

## API Reference

The Weno Fit OS SDK provides a Javascript API for interacting with the OS.

-   **[Notifications](./notifications.md)**: Learn how to send notifications from your app.

*(More API documentation will be added here.)*
