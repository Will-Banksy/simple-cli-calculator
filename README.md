# Simple CLI Calculator

A work in progress, will just solve simple expressions (no variables, differentiation, anything like that).

### Compile and Run

Only tested on Linux

To compile:

Create build directory in the root directory of the project, and `cd` into it. Then, using Makefile:
```bash
cmake ..
make
```
Or, using ninja:
```bash
cmake -GNinja ..
ninja
```

To run:

The executable will be called Calculator, it will be in the build directory, so just `./Calculator`
