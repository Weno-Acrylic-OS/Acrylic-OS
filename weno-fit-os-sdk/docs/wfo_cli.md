Weno Fit OS (WFO) CLI SDK Documentation

Welcome to the Weno Fit OS (WFO) developer documentation. This guide provides detailed information on using the wfo-cli to build, sign, and deploy applications for the Weno Fit OS ecosystem.

Overview

The wfo-cli is a Node.js-based toolset designed to automate the developer workflow. It manages the lifecycle of a WFO application from initial project scaffolding to simulator deployment.

Core Commands

init <name>

Initializes a new project directory based on the standard Weno Fit OS template.

Usage: wfo-cli init <project-name>

What it does:

Creates a new directory named <name>.

Copies the basic_app template into the directory.

Automatically updates app.json with your project's name.

build

Packages your source code into a distribution-ready .wfo file.

Usage: wfo-cli build

What it does:

Validates the presence of app.json in the current working directory.

Compresses app.json and index.js into a Gzipped tarball.

Outputs the package to the ./dist/ folder with a slugified filename (e.g., my-app.wfo).

sign <package>

Applies a developer signature to a compiled package.

Usage: wfo-cli sign ./dist/app.wfo

What it does:

Appends a ---SIGNATURE--- marker to the binary file to satisfy OS security requirements.

deploy <package>

Installs the application package into the Weno Fit OS Simulator.

Usage: wfo-cli deploy ./dist/app.wfo

What it does:

Extracts the .wfo package contents into the simulator's third_party_apps directory.

Makes the app immediately available for testing within the simulator environment.

debug

Beta Feature. Starts a debug session for the currently active project to monitor logs and performance.

Technical Specifications

The .wfo Package Format

A Weno Fit OS application package is a compressed archive containing the essential components for the OS runtime:

Component

Description

app.json

Manifest file containing metadata (name, version, permissions).

index.js

The primary entry point for application logic.

Signature

A trailing string appended to the archive for verification.

System Requirements

Runtime: Node.js (v14.0.0 or higher)

Dependencies: yargs, fs-extra, tar

Directory Structure

A standard project generated and built by the CLI follows this structure:

/my-app
├── app.json         # Application Manifest
├── index.js         # Logic & Main Loop
├── /dist            # Build Artifacts
│   └── my-app.wfo   # The compiled package
└── /node_modules    # SDK dependencies


Troubleshooting

Error: app.json not found: Ensure you are running the build command from the root of your project directory, not from within the /dist or /src folders.

Permission Denied: On Unix-based systems, ensure the CLI has execution permissions: chmod +x wfo-cli.