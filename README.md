# c-template

`c-template` is a template repository intended to serve as a base layer for easier, and safer C development by encouraging best practices and provided well-tested components. It is intended to be used when starting a new C project by cloning the repository, and starting to work in the cloned repo. It provides build management through CMake, unit testing with CMocka, dynamic analysis with Valgrind, and Doxygen for parsing code comments and generating documentation with them, as well as a number of different utility functions. 

The goal of `c-template` is to provide as close to a developer build experience as you would get from writing Golang code.

# using in your own projects

The first thing you'll want to do is change the project name to yours. To do so run the following sed statements which will replace the template name with yours

```shell
$> sed -i 's/c-template-REPLACEME/yourProjectNameHere/g' CMakeLists.txt
$> sed -i 's/c-template-REPLACEME/yourProjectNameHere/g' cmake/*.cmake
```

After that you'll want to decide what macros to include. The default `CMakeLists.txt` file is setup to build all included libraries, and their associated tests. This may not be needed for your environment.

# documentation

## doxygen

Doxygen documentation is supported, [click here for an example generated off the current code](https://bonedaddy.github.io/c-template/html/index.html).

To build doxygen docs run:

```shell
$> make # build project
$> make doxygen-docs
```

## sphinx

Commands to build docs are one of the following supported builders:

```shell
$> make # build project
$> make sphinx-docs
```
# utility functions/programs

* thread safe logger
  * color coded messages sent to stdout
  * optionally writes to a log file as well as sends to stdout
* array length and size checking
  * note: doesn't work properly on `char*` data types
* ansi color coddes
  * Enables writing to a file, or printing to stdout strings using ANSI color escape codes
* random number and string generation
* CLI builder template

# dependencies

* linux/unix (OS)
* sphinx (docs)
* doxygen (docs)
* cmocka (testing)
* valgrind (testing)
* argtable3 (command_line & cli)
* pthreads (logger)
* gcc (compilation)
  * must support C17 standard
* all other dependencies are the libraries in this repo

# examples

Eventually some examples will be included, however your best bet is to look at the `*_test.c` files for now.

# build

## release

```shell
$> make
```

## debug

```shell
$> make build-all-debug
```

# test

## unit tests

Two make targets exist to run unit tests against release, and debug builds

```shell
$> make # or make build-all-debug
$> cd build
$> ctest
```

Additionally you can run memory testing through ctest with (requires using `cmake/setup_valgrind.cmake` macro):

```shell
$> ctest -T memcheck
```


## dynamic analysis

Using valgrind you can conduct dynamic analysis, typically memory check and helgrind.  You can also run valgrind against all code built in release mode with:

```shell
$> make valgrind-all # runs valgrind against release build
```

Or if you want to run against debug builds:

```shell
$> make valgrind-all-debug # runs valgrind against debug build
```