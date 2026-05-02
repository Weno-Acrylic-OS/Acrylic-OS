# Weno Fit OS

A new RTOS-based operating system for smart health trackers.

## Project Structure

*   `src`: Source code for kernel, drivers, and applications.
*   `include`: Header files.
*   `build`: Build artifacts.
*   `docs`: Documentation.
*   `tests`: Unit and integration tests.

## Running the Weno Fit OS Simulator

To run Weno Fit OS in a web simulator run this command:

```bash
source ../emsdk/emsdk_env.sh && make -f Makefile.web run-web
```

*Note: You must have emscripten installed on your computer to run the simulator.*