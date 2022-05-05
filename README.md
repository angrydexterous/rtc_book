# Ray Tracer Challenge Solution
Taking a stab at the problems in the [Ray Tracer Challenge](http://raytracerchallenge.com/) book in C++. Cmake-based project, setup for conan, and using imgui for visualizing problems in the book with gtest for executing your test suites.

## Build
```
mkdir build
cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug ..
make all
./bin/rtc_project
./bin/rtc_project_tests
```

### Dependencies
- Linux, OSX: Due to imgui code as implemented(can be patched for other OS's)
- [conan.io](https://conan.io/)
- [cmake](https://cmake.org/)
- Various System Libraries: Install as instructed for missing libs during cmake setup and repeat until success

## References
- The Book: [Ray Tracer Challenge](http://raytracerchallenge.com/)
- Getting Conan to work with CMake [here](https://blog.jroddev.com/cmake-conan-and-gtest-example/)
- imgui for UI [example](https://github.com/conan-io/examples/tree/master/libraries/dear-imgui/basic)

## FYI: Base Conan Cmake Project
Looking for a conan/cmake base template with gtest support? Grab this repo's first commit.