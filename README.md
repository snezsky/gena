
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
```cmake
cmake --list-presets
cmake --preset <preset-name>
cmake --build build/<preset-name> 
```

## Dependencies
- Qt6
- ccache (recommended)
- cppcheck (developer mode only)
- clang-tidy (developer mode only)

## Usage

After building, you will see something like this:

![GUI image](docs/gui.png)

You can generate either a library or a Qt executable with a GUI.  
The generated project includes all features listed above and  
works out of the box.

It provides a set of CMake presets divided into **developer**  
and **user** mode. Developer mode enables additional options
```cmake
option(MYPROJECT_ENABLE_WARNINGS          "more warnings and werror"  ${MYPROJECT_ENABLE_DEVELOPER_MODE})
option(MYPROJECT_ENABLE_TEST_COVERAGE     "test coverage"             ${MYPROJECT_ENABLE_DEVELOPER_MODE})
option(MYPROJECT_ENABLE_STATIC_ANALYSIS   "clang-tidy, cppcheck"      ${MYPROJECT_ENABLE_DEVELOPER_MODE})
option(MYPROJECT_ENABLE_DYNAMIC_ANALYSIS  "available sanitizers"      ${MYPROJECT_ENABLE_DEVELOPER_MODE})
```

> NOTE: You may want to adjust **.clang-tidy** and **.clang-format** to  
better suit your project. The default configuration is quite strict.


> NOTE: You are expected to call `<project name>_setup_target` for every  
target you create. This ensures all features are applied to that target.

## GitLab runner

The project includes a **.gitlab-ci.yml** file with a simple pipeline that  
builds the project, runs tests, and checks coverage. However, it does not  
create a runner automatically, so you will need to set one up yourself.  
See the official [documentation](https://docs.gitlab.com/runner/install/)
