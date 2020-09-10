# Device Shadow Library

This repository contains a Device Shadow client library for embedded platforms to interact with the [AWS IoT Device Shadow service](https://docs.aws.amazon.com/iot/latest/developerguide/iot-device-shadows.html).

## Building Unit Tests

### Device Shadow Config File
The Device Shadow library exposes build configuration macros that are required for building the library.
A list of all the configurations and their default values are defined in [shadow_config_defaults.h](https://github.com/aws/device-shadow-for-aws-iot-embedded-sdk/blob/master/source/include/shadow_config_defaults.h). 
To provide custom values for the configuration macros, a custom config file named `shadow_config_defaults.h` can be provided by the application to the library.

By default, a `shadow_config.h` custom config is required to build the library. To disable this requirement
and build the library with default configuration values, provide `SHADOW_DO_NOT_USE_CUSTOM_CONFIG` as a compile time preprocessor macro.

**Thus, the Device Shadow library can be built by** defining the `SHADOW_DO_NOT_USE_CUSTOM_CONFIG` preprocessor macro for the library build.

### Platform Prerequisites

- For building the library, **CMake 3.13.0** or later and a **C90 compiler**.
- For running unit tests, Ruby 2.0.0 or later is additionally required for the CMock test framework (that we use).
- For running the coverage target, gcov is additionally required.

### Steps to build Unit Tests

1. Go to the root directory of this repository.

1. Create a build directory: `mkdir build && cd build`

1. Run *cmake* while inside the build directory: `cmake ../test -DBUILD_CLONE_SUBMODULES=ON`

1. Run this command to build the library and unit tests: `make all`

1. The test executables will be present in the `build/bin/tests` folder.

1. Run `ctest` to execute all tests and view the test run summary.

## Reference examples

The AWS IoT Embedded C-SDK repository contains demos of using the Device Shadow library [here](https://github.com/aws/aws-iot-device-sdk-embedded-C/tree/development/demos/shadow) on a POSIX platform. These can be used as reference examples for the library API.

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```


