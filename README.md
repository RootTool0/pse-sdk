# Portal Solver Editor SDK

**[EN]** | **[[RU]](README_RU.md)**

**Portal Solver Editor (PSE) SDK** - The official toolkit _(SDK)_ for programmatic work with the in-game editor of **Portal: Solver**

The library provides a programming interface _(C API)_ for bidirectional real-time data exchange, allowing you to build **external level editors**, **automation tools**, **plugins**, **unusual custom levels**, and **utilities** that run in sync with the running game in real time

## Bindings

The core architecture _(C API)_ allows **PSE SDK** to be ported to any platform and language

Below is the list of existing solutions

- **Python:** **[pse-sdk-python](https://github.com/RootTool0/pse-sdk-python)** by [RootTool](https://github.com/RootTool0)
- **Lua:** **[pse-lua](https://github.com/ntcd-lol/pse-lua)** by [ntcd-lol](https://github.com/ntcd-lol)

## Project structure

- `include/` - Interface header files _(pse.h, enums.h, structures.h, pse.hpp)_
- `source/` - **PSE SDK** implementation source code
- `examples/` - Compilable usage examples of various **PSE SDK** modes
- `docs/` - Technical and conceptual documentation of the project

## Documentation

A detailed description of the architecture, element specifications and usage guides can be found in the **[Documentation](https://roottool0.github.io/pse-sdk/index.html?lang=en)** section

## Code example

A basic scenario of initializing a session and sending a command to create a dynamic mesh, blocking the thread until the game responds

```cpp
#include "pse/pse.h"
#include "pse/pse.hpp"

int main() {
    // Allocating and initializing the buffers
    if (pseInitializeBuffers() != 0) {
        return -1;
    }
    
    // Creating the command
    PseData data{};
    data.header = PSE_COMMAND_DYNAMIC_MESH_CREATE; // Command - create a dynamic mesh
    
    // Packing the parameters at fixed byte offsets
    pse::Data::set< 0, PseTransform>(data, PseTransform{PseQuat{}, PseVector3{0, 0, 0}, PseVector3{1, 1, 1}});
    pse::Data::set<40, PseMesh>(data, PSE_MESH_FACE);
    pse::Data::set<42, PseMaterial>(data, PSE_MATERIAL_FLOOR_WHITE);
    pse::Data::set<44, int8_t>(data, 1); // Visibility
    
    // Sending the packet to the ring buffer and waiting for the execution result
    pseCommandBufferSinglePushAndWait(&data);
    
    // Releasing the shared memory resources
    pseDeinitializeBuffers();
    return 0;
}
```

## Building from source _(Obtaining the DLL / Library)_

The internal part of the **SDK** is implemented in **C++17**, while a pure **C API interface** is exposed outward to ensure **maximum compatibility**

To build the dynamic library _(`.dll` / `.so`)_ and header files into a single distribution folder `dist/`, run the following commands in the terminal from the repository root:
```bash
# 1. Generate the build files and set the output folder to dist
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="dist"

# 2. Compile the project
cmake --build build --config Release

# 3. Assemble the finished distribution (the library and includes will go into the dist folder)
cmake --install build --config Release
```

After running these commands, a `dist/` folder will appear in the project root, ready for integration into **your projects** or **bindings**

## Links and feedback

- **Portal: Solver** page on **GameJolt**: [GameJolt](https://gamejolt.com/games/PortalSolver/898591)
- Download ready releases: [Releases](https://github.com/RootTool0/pse-sdk/releases)
- Report a bug _(Bug Report)_: [Issues](https://github.com/RootTool0/pse-sdk/issues)
- Contact **RootTool**:
    - **Telegram (channel/blog)**: [RootTool Blog](https://t.me/RootToolBlog)
    - **Telegram (DM)**: [@RootTool0](https://t.me/RootTool0)
    - **Discord**: `.roottool` (or `RootTool#5776`)
    - **YouTube**: [RootTool](https://www.youtube.com/@RTEdits0)
