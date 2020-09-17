# Distance Calculator

README is coming...

# Prerequisites
Make sure that you've installed the following packages:
```
>= cmake 3.18
>= g++ 10.2 or >= clang 10.0
>= make 4.3
```

## Archlinux
To install mentioned packages on archlinux run:
```
for g++:
sudo pacman -Sy cmake gcc make
for clang:
sudo pacman -Sy cmake clang make
```

# Build

Please note that this build is tested with g++ 10.2.0 on linux kernel 5.8.6.1.

Clone the project:
```
git clone https://github.com/lnikon/distancecalculator
```

This projects depends on the following framework:
* Catch2
* GoogleBenchmark

Those are included as gitsubmodules, so you need to:
```
git submodule init
git submodule update
```

To build run the following instructions:
```
$ mkdir build && cd build
$ cmake .. -G "Unix Makefiles"
$ make -j4
```
