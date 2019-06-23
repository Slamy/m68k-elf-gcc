# m68k-elf-gcc toolchain with C++ support

## Why ?

I'm currently busy developing an Amiga game in C and wanted to give others the possibility to have an easy start into this topic.
I believe that C can be used as an alternative to assembler even on the Motorola 68000.

It turns out there are quite some good tolchains for Amiga development out here.
I want to give credit to those I use.
- [The toolchain68k from haarer](https://github.com/haarer/toolchain68k) is a very good one. I currently use this as NDOS toolchain for developing without using AmigaDOS which saves some valuable memory for game development.
- [The amiga-gcc from bebbo](https://github.com/bebbo/amiga-gcc) is another very good example. It targets developing for AmigaDOS and I use this one for the harddisk port of my game.

So what is the issue then? I was thinking about if C++ could be used for developing for the Amiga and this project here gives some benchmarks in terms of speed and size about this topic. While bebbos's toolchain is working with C++ code, it turns out that the toolchain68k of haarer doesn't produce a working libstdc++ and now working global constructors. While g++ is working in general (range-loops are funcitonal) I'm not satisfied with the results as I wanted to use the C++11 potential during development.

So I decided to dive a little bit deeper and try to create my own m68k-elf-gcc toolchain.

## What this toolchain does different?

- Delivered with Bootloader and ADF construction kit for direct building of ADFs
- Working C++ examples with C++11 style code.
- Downsizing of newlib to save memory
	- Removal of Floating point support of scanf and printf (I haven't found a purpose for it during development. Keep in mind that float and double datatypes are still usable and only take up space if used in the project.)
- Downsizing of libstdc++ to save memorey
	- Deactivation of exceptions. (Unwind code takes up much space. Exceptions leads to abort() in this case)
	- Usage of stream type operators (like std::cout) is highly discouraged as it wastes about 300kB of memory. For systems with lots of FastRAM this might not be a big issue. But for A500 development the newlib-nano implementation of printf is much better suited. In The example project those can be enabled optionally to see the difference.

## Parts of this project which were fetched from other projects

Besides the mentionend toolchains above, I'd like to give credit and thanks to those projects

- DDE5-BootLoader.S is a BootLoader and Floppy driver by [Photon](http://coppershade.org/asmskool/SOURCES/Photon-snippets/DDE5-BootLoader.S).
- make-adf.py and sum-bootblock.py is from [this bootloader project](https://github.com/deplinenoise/trackloader).

## Prerequisites

- A linux machine as Windows support won't be tested by me. But feel free to inform me about successes.
- kick31.rom and kick13.rom must be placed in the example folder for execution. I can't supply this for copyright reasons.
- If you like to execute the AmigaDOS version of the example
	- Copy a valid Libs folder to harddrive from an AmigaDOS 3.1 installation. I can't supply this for copyright reasons.
	- You need to install the bebbo toolchain mentioned above as this project only produces an ELF kit.
- You need to install some build-essentials
	- sudo apt-get install  wget bzip2 git make tar flex bison diffutils texinfo gcc g++ libgmp-dev libmpc-dev libmpfr-dev libisl-dev
- For testing you also need an Amiga emulator. fs-uae is my currently favoured solution
    - sudo apt-get install fs-uae


## How to build the toolchain

```bash
cd toolchain
./buildToolchain.sh
```

## How to build the example as NDOS application

```bash
cd example
./build.sh -DAMIGACROSS=1 -DBUILD_FOR_AMIGADOS=0
```

## How to build the example as AmigaDOS application (bebbo Toolchain is needed!)

```bash
cd example
./build.sh -DAMIGACROSS=1 -DBUILD_FOR_AMIGADOS=1
```

## How to build the example as a Linux application

```bash
cd example
./build.sh -DAMIGACROSS=0
```


## How to test the NDOS build

This is a very small example. You need a COM port emulation.
Execute this in one terminal

```bash
scripts/serialout.sh
```

Now execute this in another terminal

```bash
fs-uae a1200.fs-uae
```

## How to test the AmigaDOS build

This is a very small example. You need a COM port emulation.
Execute this in one terminal

```bash
scripts/serialout.sh
```

Now execute this in another terminal

```bash
fs-uae a1200hd.fs-uae
```
AmigaDOS should pop up and then execute "m". I went for this short name as it could be type very fast.


## Example Results of current build

### NDOS on unexpanded emulated Amiga 500

    Allocate 50 int arrays of size 4 and manage them
                c array of malloc'd pointers   5784 us    100%
                   c array of new'd pointers   7045 us    122%
                  c array of unique pointers   7621 us    132%
       reserve c++ vector of unique pointers   9034 us    156%
      presized c++ vector of unique pointers   8577 us    148%
                  c array of shared pointers   7009 us    121%
               c++ vector of unique pointers  10146 us    175%
                 c++ list of unique pointers  15569 us    269%
                c++ array of unique pointers   7669 us    133%
    
    Allocate 50 int arrays of size 64 and manage them
                c array of malloc'd pointers   6442 us    100%
                   c array of new'd pointers   7723 us    120%
                  c array of unique pointers  10361 us    161%
       reserve c++ vector of unique pointers  12083 us    188%
      presized c++ vector of unique pointers  11438 us    178%
                  c array of shared pointers   7329 us    114%
               c++ vector of unique pointers  13262 us    206%
                 c++ list of unique pointers  19122 us    297%
                c++ array of unique pointers  10550 us    164%
    
    Construct strings from 4 integers
                           sprintf   3887 us
            c++ stringstream build   5753 us
    Strings ok
    
    Iterate over 1000 ints and sum them up
                arraysum - for i++   1441 us    100%   results verified
                arraysum - for ++i   1441 us    100%   results verified
             arraysum - range loop   1441 us    100%   results verified
            vectorsum - range loop   1442 us    100%   results verified
         vectorsum - std::for_each   1441 us    100%   results verified
              vectorsum - iterator   1441 us    100%   results verified
       vectorsum - std::accumulate   1442 us    100%   results verified
              listsum - range loop   2000 us    139%   results verified
                listsum - iterator   1716 us    119%   results verified

### NDOS on unexpanded emulated Amiga 1200

    Allocate 50 int arrays of size 4 and manage them
                c array of malloc'd pointers   5784 us    100%
                   c array of new'd pointers   7045 us    122%
                  c array of unique pointers   7621 us    132%
       reserve c++ vector of unique pointers   9034 us    156%
      presized c++ vector of unique pointers   8577 us    148%
                  c array of shared pointers   7009 us    121%
               c++ vector of unique pointers  10146 us    175%
                 c++ list of unique pointers  15569 us    269%
                c++ array of unique pointers   7669 us    133%
    
    Allocate 50 int arrays of size 64 and manage them
                c array of malloc'd pointers   6442 us    100%
                   c array of new'd pointers   7723 us    120%
                  c array of unique pointers  10361 us    161%
       reserve c++ vector of unique pointers  12083 us    188%
      presized c++ vector of unique pointers  11438 us    178%
                  c array of shared pointers   7329 us    114%
               c++ vector of unique pointers  13262 us    206%
                 c++ list of unique pointers  19122 us    297%
                c++ array of unique pointers  10550 us    164%
    
    Construct strings from 4 integers
                           sprintf   3887 us
            c++ stringstream build   5753 us
    Strings ok
    
    Iterate over 1000 ints and sum them up
                arraysum - for i++   1441 us    100%   results verified
                arraysum - for ++i   1441 us    100%   results verified
             arraysum - range loop   1441 us    100%   results verified
            vectorsum - range loop   1442 us    100%   results verified
         vectorsum - std::for_each   1441 us    100%   results verified
              vectorsum - iterator   1441 us    100%   results verified
       vectorsum - std::accumulate   1442 us    100%   results verified
              listsum - range loop   2000 us    139%   results verified
                listsum - iterator   1716 us    119%   results verified


### AmigaDOS on unexpanded emulated Amiga 1200

TODO File output is currently missing.

### AmigaDOS on emulated Amiga 1200 with FastRAM

TODO File output is currently missing.


