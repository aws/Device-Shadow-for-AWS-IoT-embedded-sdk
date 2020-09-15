# Device Shadow Library

This repository contains a Device Shadow client library for embedded platforms to interact with the [AWS IoT Device Shadow service](https://docs.aws.amazon.com/iot/latest/developerguide/iot-device-shadows.html).

### Device Shadow Config File
The Device Shadow library exposes build configuration macros that are required for building the library.
A list of all the configurations and their default values are defined in [shadow_config_defaults.h](source/include/shadow_config_defaults.h). 
To provide custom values for the configuration macros, a custom config file named `shadow_config.h` can be provided by the application to the library.

By default, a `shadow_config.h` custom config is required to build the library. To disable this requirement
and build the library with default configuration values, provide `SHADOW_DO_NOT_USE_CUSTOM_CONFIG` as a compile time preprocessor macro.

## Building the Library

The [shadowFilePaths.cmake](shadowFilePaths.cmake) file contains the information of all source files and the header include path required to build the Shadow library.

As mentioned in the previous section, either a custom config file (i.e. `shadow_config.h`) OR the `SHADOW_DO_NOT_USE_CUSTOM_CONFIG` macro needs to be provided to build the Shadow library.

For a CMake example of building the Shadow library with the `shadowFilePaths.cmake` file, refer to the `coverity_analysis` library target in [test/CMakeLists.txt](test/CMakeLists.txt) file.

## Building Unit Tests

### Platform Prerequisites

- For building the library, **CMake 3.13.0** or later and a **C90 compiler**.
- For running unit tests, **Ruby 2.0.0** or later is additionally required for the CMock test framework (that we use).
- For running the coverage target, **gcov** and **lcov** are additionally required.

### Steps to build unit tests

1. Go to the root directory of this repository.

1. Run the *cmake* command: `cmake -S test -B build -DBUILD_CLONE_SUBMODULES=ON `

1. Run this command to build the library and unit tests: `make -C build all`

1. The generated test executables will be present in `build/bin/tests` folder.

1. Run `cd build && ctest` to execute all tests and view the test run summary.

## Reference examples

The AWS IoT Embedded C-SDK repository contains demos of using the Device Shadow library [here](https://github.com/aws/aws-iot-device-sdk-embedded-C/tree/master/demos/shadow) on a POSIX platform. These can be used as reference examples for the library API.

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```
