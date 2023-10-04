**Summary:**

**CSE462/562: Database Systems Course Project**

In this semester-long course project, you will build a mini database system using the Taco-DB framework. This README contains instructions on how to set up the build environment, compile, and test the system.

**Dependencies and system requirements:**

* git
* cmake >= 3.13
* pkg-config (0.29 recommended; <= 0.25 will make the build extremely slow)
* c11 and c++11 compilers (e.g., gcc >= 7 or clang)
* make
* autoconf (for jemalloc)
* python3

**Recommended platform:**

* Recent Linux distribution on x86_64 platform

**Known non-working platforms:**

* Mac OS
* Windows WSL1
* Cygwin
* MSYS
* Win32
* Non-x86_64 architectures (e.g., Apple laptops with M1 processor, Microsoft Surface Pro X with SQ1 or SQ2)

**How to build:**

To create a debug build in the `build/` directory, run:

```
cmake -B build .
```

On the first build, it will also build the jemalloc library and install it in the `external/install` directory.

To create a release build in the `build.release/` directory, run:

```
cmake -DCMAKE_BUILD_TYPE=Release -B build.release .
```

**How to test your implementation:**

The project uses the GoogleTest framework with its community-supported CMake integration, ctest. For advanced usages, please review the GoogleTest and ctest documentation.

To run all tests, in your build directory, run:

```
ctest
```

To run an individual test, you may either use:

```
ctest -V -R <some_test_name_regex>
```

or directly run:

```
./tests/path-to-some-test
```

To list the project-specific flags, run:

```
./tests/path-to-some-test --help
```

**Additional notes:**

* Please report any bugs to the course Piazza forum using a **private** message to all TAs and the instructor.
* It is highly recommended to use a recent Linux distribution on x86_64 platform. The project might not work as intended or might not work at all on other systems, even if it compiles.
