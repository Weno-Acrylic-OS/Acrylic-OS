# Weno Fit OS

A new RTOS-based operating system for smart health trackers.

## Project Structure

- `src`: Source code for kernel, drivers, and applications.
- `include`: Header files.
- `build`: Build artifacts.
- `docs`: Documentation.
- `tests`: Unit and integration tests.

## Running the Weno Fit OS Simulator

To run Weno Fit OS in a web simulator run this command:

```bash
source ../emsdk/emsdk_env.sh && make -f Makefile.web run-web
```

*Note: You must have emscripten installed on your computer to run the simulator.*

## How to use the [run.sh](http://run.sh) script for running Acrylic OS on QEMU.

You can run and emulate an Acrylic OS system image by downloading the build artifact from the latest successful workflow run. After you have downloaded the .zip archive, extract it on your computer and follow these steps:

1. Rename the buildroot-2023.11.1 directory to final.
2. Place the final directory towards the x86_os directory.
3. Copy and Paste the acrylic_os.img file to build_arm directory.
4. Run the emulator with `./run.sh` command.

### How to use emulator personalities with [run.sh](http://run.sh) .

You can run different interfaces also known as personalities with [run.sh](http://run.sh) by running these commands for these specfic interfaces:

- `./run.sh phone` To emulate the phone personality.
- `./run.sh desktop` To emulate the desktop personality.
- `./run.sh smarthome` To emulate the Smart Home personality (Smart Home Displays and TV Sticks).



## Running the Weno Store

To run the Weno Store server to install apps run this command: 

```sh
cd weno-store-backend && npm run start
```

## Running the Alyssa AI Backend

To run the Alyssa AI backend server (required for the Alyssa assistant feature):

1.  Navigate to the backend directory:
    ```bash
    cd alyssa-ai-backend
    ```
2.  Set up a Python virtual environment and activate it (first time setup):
    ```bash
    python3 -m venv venv
    source venv/bin/activate
    ```
3.  Install the required Python packages (first time setup, downloads AI model):
    ```bash
    pip install -r requirements.txt
    ```
4.  Run the AI server:
    ```bash
    python3 server.py
    ```

*Note: The AI model download (`pip install`) can take some time and consume a significant amount of data and disk space.*



