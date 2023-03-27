# Big Number Library

This project creates a static library that makes it possible to work with giant numbers with thousands of bits.

The purpose of creating this library is to enable the study of algorithms focused on this context.

The reason for its creation is due to the need to manipulate the source that was not possible in commercial libs, with the necessary freedom.

## This Repository

There is a small example of use, in `c`, in the `test/` directory.

This example was also used as a unit and functional test of the library created. `Cmake` is used for its build and `doxygen` for its documentation. Build can be performed by running the scripty `build.py` which is in the `build/` folder.

## Directory structure

### .vscode
Vscode configuration files.
### bin
Program binaries generated using lib de bingnum. Specifically, the binary of the quality tests.

### build
Contains the scripts to build the lib and peripheral programs.

### doc
Lib documentation.

### ide
Workstace for IDEs.

### include
Lib headers.
### lib
Lib delivery.

### src
Source code.

### test
Test Application.

