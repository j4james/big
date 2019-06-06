BIG: Befunge Integer Generator
==============================

The Befunge Integer Generator attempts to generate the shortest possible
[Befunge] expression that will evaluate to a given number.

There is only a small range of numbers that can be represented with literal
values in Befunge, so in most cases you'll have to calculate the number you
need using an arithmetic expression. Determining the optimal version of that
expression is a non-trivial task, but that's where this app can help.

[Befunge]: https://github.com/catseye/Befunge-93


Usage
-----

Simply pass the number you want to generate as a command-line argument, e.g.:

    $ big 31337
    ";"3*:*8+

For more advanced queries, and detailed documentation of the available options,
see the [MANUAL] file.

[MANUAL]: MANUAL.md


Download
--------

The latest binaries can be downloaded from GitHub at the following url:

https://github.com/j4james/big/releases/latest



Build Instructions
------------------

If you want to build the program yourself, you'll need [CMake] version 3.10
or later and a C++ compiler supporting C++17 or later (tested with [GCC] 7.4,
[Clang] 6.0.0, and [Visual Studio] 2017 v15.9.12).

The process is slightly different depending on whether you're using a single
configuration generator (like a Makefile) or a multi configuration generator
(like Visual Studio or Xcode).

**Single configuration generators**

1. Download or clone the source:  
   `git clone https://github.com/j4james/big.git`

2. Change into the build directory:  
   `cd big/build`

3. Generate the project for your preferred build type:  
   `cmake -D CMAKE_BUILD_TYPE=Release ..`

4. Start the build:  
   `cmake --build .`

**Multi configuration generators**

1. Download or clone the source:  
   `git clone https://github.com/j4james/big.git`

2. Change into the build directory:  
   `cd big/build`

3. Generate the build project (supporting all build types):  
   `cmake ..`

4. Start the build for your preferred build type:  
   `cmake --build . --config Release`

[CMake]: https://cmake.org/
[GCC]: https://gcc.gnu.org/
[Clang]: https://clang.llvm.org/
[Visual Studio]: https://visualstudio.microsoft.com/


License
-------

The BIG source code and binaries are released under the MIT License. See the
[LICENSE] file for full license details.

[LICENSE]: LICENSE.txt
