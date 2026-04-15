# myproject

## Build

You can build the project using your IDE of choice, or:
```bash
cmake --list-presets
cmake --preset <preset-name>
cmake --build build/<preset-name> 
```

## Dependencies
- [ccache](https://ccache.dev/download.html) (recommended)
- [cppcheck](https://cppcheck.sourceforge.io/) (developer mode only)
- [clang-tidy](https://github.com/llvm/llvm-project/releases/) (developer mode only)

> [!IMPORTANT]
> On Windows, you will need to install clang-tidy 22.1.0  
  or newer to use it with sanitizer-enabled builds.
