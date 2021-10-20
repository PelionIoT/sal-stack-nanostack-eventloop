# Unit tests

You can run unit tests with script `run_unit_tests.sh`.

The unit tests are implemented with GoogleTest (gtest) C++ test framework.

## Prerequisites

If you need to generate code coverage report, install `gcovr` tool:

```
pip install gcovr

or...

sudo apt-get install gcovr
```

## Building and running

Prepare unit test build with `cmake ..` and then run the test with `make check` as follows:

```
# clone repository if needed
git clone sal-stack-nanostack-eventloop
cd sal-stack-nanostack-eventloop
mkdir build
cd build
cmake ..

make check
```

### Code coverage

Code coverage is optional feature and it is disabled by default.
To enable code coverage use the following command to build the tests:
```
cmake .. -Denable_coverage_data=ON
```

### Debugging

You can add mode debug information to the build by adding a `-DCMAKE_BUILD_TYPE=Debug` to the build command, like:
```
cmake .. -Denable_coverage_data=ON -DCMAKE_BUILD_TYPE=Debug
```
And then debug the generated executable with a debugger like `Visual code`, `gdb`, `Eclipse`.
