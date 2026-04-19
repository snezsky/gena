
# Gena

A C++ project generator that creates a ready-to-use project skeleton with:

- CMake build system
- Static analysis
- Dynamic analysis
- Extended warning set
- Gitlab CI with test coverage
- Other useful but unobtrusive features (caching, optimizations,  etc.)

## Build

You can build the project using your IDE of choice, or:
```bash
cmake --list-presets
cmake --preset <preset-name>
cmake --build build/<preset-name>
```
Tested on Windows 10/11 and Ubuntu 24.04, but it should work on other Unix-like systems too.

> [!NOTE]
> When building on Windows from the command line, use the x64 Native  
  Tools Command Prompt (or run vcvars64.bat) to ensure a 64-bit toolchain.

> [!TIP]
> If your Qt installation is not in the default system directory, set the `Qt6_ROOT`   
  environment variable to your Qt installation and add its binary directory to your **PATH**.

## Dependencies
- [Qt 6.3+](https://www.qt.io/development/download-qt-installer-oss) 
- [ccache](https://ccache.dev/download.html) (recommended)
- [cppcheck](https://cppcheck.sourceforge.io/) (developer mode only)
- [clang-tidy](https://github.com/llvm/llvm-project/releases/) (developer mode only)

> [!IMPORTANT]
> On Windows, you will need to install clang-tidy 22.1.0  
  or newer to use it with sanitizer-enabled builds.

## Usage

After building, you will see something like this:

![GUI image](docs/gui.png)

You can generate a library, an empty executable, or an executable with a Qt GUI.  
The generated project includes all the features listed above and works out of the box.

It provides a set of CMake presets divided into **developer**  
and **user** mode. Developer mode enables additional options
```cmake
option(MYPROJECT_ENABLE_WARNINGS          "more warnings and werror"  ${MYPROJECT_ENABLE_DEVELOPER_MODE})
option(MYPROJECT_ENABLE_TEST_COVERAGE     "test coverage"             ${MYPROJECT_ENABLE_DEVELOPER_MODE})
option(MYPROJECT_ENABLE_STATIC_ANALYSIS   "clang-tidy, cppcheck"      ${MYPROJECT_ENABLE_DEVELOPER_MODE})
option(MYPROJECT_ENABLE_DYNAMIC_ANALYSIS  "available sanitizers"      ${MYPROJECT_ENABLE_DEVELOPER_MODE})
```

> [!IMPORTANT]
> You are expected to call `<project name>_setup_target` for every  
  target you create. This ensures all features are applied to that target.

> [!TIP]
> Don’t turn off `Developer mode` just because **clang-format** or **clang-tidy** is annoying.  
  Fix the issues, tweak *.clang-tidy* / *.clang-format*, use [NOLINT](https://clang.llvm.org/extra/clang-tidy/#suppressing-undesired-diagnostics) / [clang-format off](https://clang.llvm.org/docs/ClangFormatStyleOptions.html#disabling-formatting-on-a-piece-of-code) where it makes sense.  
  Developing in user mode disables most of the tooling and defeats the whole purpose.

## GitLab runner

The project includes a **.gitlab-ci.yml** file with a simple pipeline that  
builds the project, runs tests, and checks coverage. However, it does not  
create a runner automatically, so you will need to set one up yourself.  
See the official [documentation](https://docs.gitlab.com/runner/install/)
