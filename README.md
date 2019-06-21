# m68k-elf-gcc toolchain with C++ support

## Why ?

I'm currently busy developing an Amiga game in C and wanted to give others the possibility to have an easy start into this topic
I believe that C can be used as an alternative to assembler even on a basic 68000.

It turns out there are quite some good tolchains for Amiga development out here.
I want to give credit to those I use.
- [The toolchain68k from haarer](https://github.com/haarer/toolchain68k) is a very good one. I currently use this as NDOS toolchain for developing without any AmigaDOS active.
- [The amiga-gcc from bebbo](https://github.com/bebbo/amiga-gcc) is another very good example. It targets developing for AmigaDOS and I use this one for the harddisk port of my game.

So what is the issue then? I was thinking about if C++ could be used for developing for the Amiga and this project here gives some benchmarks in terms of speed and size about this topic. Also it turns out that the toolchain68k doesn't produce a working libstdc++. While g++ is working in general I'm not satisfied with the results.

## What this toolchain does different?

- Delivered with Bootloader and ADF construction kit for direct building of ADFs
- Working C++ examples
- Downsizing of libstdc++ to save memorey
	- Deactication of Exceptions
	- Usage of streaming (like std::cout) is not possible as it wastes about 600kB of memory.

## Prerequisites

- A linux machine as Windows support won't be tested by me. But feel free to inform me about successes.
- kick31.rom and kick13.rom must be placed in example for execution. I can't supply this for copyright reasons.
- You need to install
	- fs-uae
	- build-essentials

## How to build the toolchain

```bash
cd toolchain
./buildToolchain.sh
```

## How to build the example as NDOS application

```bash
cd example
./build.sh
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

