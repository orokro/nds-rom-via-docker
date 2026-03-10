# Cube UI Demo (Nintendo DS)

A simple Nintendo DS homebrew demonstration featuring a rotating 3D cube on the top screen and an interactive touch-based UI on the bottom screen.

## Features
*   **3D Rendering:** Uses the DS hardware to render a textured, rotating cube with fixed-point math.
*   **Touch Interactivity:** Toggle the cube's rotation direction by touching the designated button on the bottom screen.
*   **Real-time Feedback:** Displays live stylus coordinates and rotation status on the sub-screen console.
*   **Dockerized Build:** Includes a pre-configured environment to build the project without manual toolchain installation.

## Prerequisites
Before you begin, ensure you have the following installed on your system:
*   [Docker](https://www.docker.com/get-started) (Ensure the Docker daemon is running)
*   A Bash-compatible shell (Linux, macOS, or Windows with Git Bash/Cygwin/WSL)

**Note:** This project was developed and tested in a **Cygwin** environment on Windows. While the `ndsutilities.sh` script includes logic to handle different path formats, you may need to modify it if you encounter mounting issues in other environments (like native Linux or macOS).

## Getting Started

### 1. Load the Utilities
The project includes a helper script (`ndsutilities.sh`) that wraps Docker commands for the `devkitARM` toolchain. To make these commands available in your terminal, source the script:

```bash
source ./ndsutilities.sh
```

### 2. Install the Build Environment
Pull the latest `devkitpro/devkitarm` Docker image. This contains all the compilers and libraries (like `libnds`) required for DS development.

```bash
ndsinstall
```

### 3. Build the ROM
Compile the source code into a Nintendo DS ROM (`.nds` file):

```bash
ndsbuild
```

Once the build is complete, you will find `cubeui.nds` in the root directory.

## Available Commands
After sourcing `ndsutilities.sh`, the following commands are available:
*   `ndsinstall`: Pull/update the devkitPro Docker image.
*   `ndsbuild` / `ndsmake`: Build the `.nds` ROM.
*   `ndsclean`: Remove build artifacts and the ROM.
*   `ndsrebuild`: Perform a clean and then a full build.
*   `ndsshell`: Open an interactive shell inside the build container.
*   `ndsrom`: Print the path to the generated ROM.

## Running the Demo
You can run the resulting `cubeui.nds` file using:
*   **Hardware:** Copy it to a flashcard (like an R4) and run it on an actual Nintendo DS/3DS.
*   **Emulator:** Use an emulator like [DeSmuME](https://desmume.org/) or [melonDS](https://melonds.kuribo64.net/).

## Project Structure
*   `source/main.c`: Core application logic, 3D setup, and UI code.
*   `include/`: Directory for header files.
*   `Makefile`: Standard devkitPro Makefile for NDS projects.
*   `ndsutilities.sh`: Docker helper scripts for building the project.
