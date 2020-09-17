# AWS IoT Device Shadow

The AWS IoT Device Shadow library enables you to store and retrieve the current state (the “shadow”) of every registered device. The device’s shadow is a persistent, virtual representation of your device that you can interact with from AWS IoT Core even if the device is offline. The device state is captured as its “shadow” within a [JSON](https://www.json.org/) document. The device can send commands over MQTT or HTTP to update its latest state. Each device’s shadow is uniquely identified by the name of the corresponding “thing”, a representation of a specific device or logical entity on the AWS Cloud. See [Managing Devices with AWS IoT](https://docs.aws.amazon.com/iot/latest/developerguide/iot-thing-management.html) for more information. More details about AWS IoT Device Shadow can be found in [AWS IoT documentation](https://docs.aws.amazon.com/iot/latest/developerguide/iot-device-shadows.html). This library is distributed under the [MIT Open Source License](LICENSE).

This library has gone through code quality checks including verification that no function has a [GNU Complexity](https://www.gnu.org/software/complexity/manual/complexity.html) score over 8, and checks against deviations from mandatory rules in the [MISRA coding standard](https://www.misra.org.uk/MISRAHome/MISRAC2012/tabid/196/Default.aspx). Deviations from the MISRA C:2012 guidelines are documented under [MISRA Deviations](MISRA.md). This library has also undergone both static code analysis from [Coverity static analysis](https://scan.coverity.com/), and validation of memory safety and proof of functional correctness through the [CBMC automated reasoning tool](https://www.cprover.org/cbmc/).  

### AWS IoT Device Shadow Config File
The AWS IoT Device Shadow library exposes configuration macros that are required for building the library.
A list of all the configurations and their default values are defined in [shadow_config_defaults.h](source/include/shadow_config_defaults.h). 
To provide custom values for the configuration macros, a custom config file named `shadow_config.h` can be provided by the user application to the library.

By default, a `shadow_config.h` custom config is required to build the library. To disable this requirement
and build the library with default configuration values, provide `SHADOW_DO_NOT_USE_CUSTOM_CONFIG` as a compile time preprocessor macro.

## Building the Library

The [shadowFilePaths.cmake](shadowFilePaths.cmake) file contains the information of all source files and the header include path required to build the AWS IoT Device Shadow library.

As mentioned in the [previous section](#aws-iot-device-shadow-config-file), either a custom config file (i.e. `shadow_config.h`) OR the `SHADOW_DO_NOT_USE_CUSTOM_CONFIG` macro needs to be provided to build the AWS IoT Device Shadow library.

For a CMake example of building the AWS IoT Device Shadow library with the `shadowFilePaths.cmake` file, refer to the `coverity_analysis` library target in [test/CMakeLists.txt](test/CMakeLists.txt) file.

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

The AWS IoT Embedded C-SDK repository contains demos of using the AWS IoT Device Shadow library [here](https://github.com/aws/aws-iot-device-sdk-embedded-C/tree/master/demos/shadow) on a POSIX platform. These can be used as reference examples for the library API.

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```
