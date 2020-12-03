# Distance Calculator

![Travis CI Build Status](https://travis-ci.com/lnikon/distancecalculator.svg?branch=master)

## Prerequisites

Make sure that you've installed the following packages:

```sh
>= cmake 3.18
>= g++ 10.2 or >= clang 10.0
>= make 4.3
```

## Archlinux

To install mentioned packages on archlinux run:

```sh
for g++:
sudo pacman -Sy cmake gcc make
for clang:
sudo pacman -Sy cmake clang make
```

## Build

Please note that this build is tested with g++ 10.2.0 on linux kernel 5.8.6.1.

Clone the project:

```sh
git clone https://github.com/lnikon/distancecalculator
```

This projects depends on the following framework:

* Catch2
* GoogleBenchmark

Those are included as gitsubmodules, so you need to:

```sh
git submodule init
git submodule update
```

To build run the following instructions:
CUDA based calculation engine support is initially disabled. If you wish to enable then pass -DSUPPORT_CUDA_ENGINE=ON to cmake.

```sh
mkdir build && cd build
cmake .. -G "Unix Makefiles"
make -j4
```
