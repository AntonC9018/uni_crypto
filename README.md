# Criptografie

This repository contains assignments, implemented as part of the course **Criptografie** (**Cryptography**).

It features a GUI app built with GTK. See explanatory markdown documents for each of the assignments in the `doc` folder. Assignments are all in *Romanian*.

> The markdown files use [tex math formulas, which won't show on github](https://stackoverflow.com/questions/11256433/how-to-show-math-equations-in-general-githubs-markdownnot-githubs-blog). To see formulas, you will either have to convert markdown to html or pdf, with [`MathJax`](https://www.mathjax.org/) enabled, or find the compiled pdf's [on my google drive](https://drive.google.com/drive/folders/1Rs0-qy6ivSDuHh5JadrP4Ta4YDhuVRiC).

PR's with grammar corrections, bug fixes, improvement suggestions or translations are very welcome.

Leave a star as a way to say "Thank you". Enjoy!


## Building

Clone the submodules along with the repository:

```sh
git clone --recurse-submodules https://github.com/AntonC9018/uni_crypto.git
cd uni_crypto
```

The application requires CMake 3.21 or newer, a C++17 compiler, `pkg-config`,
and the GTKmm 3 development package. Configure and build it with:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Run the application with:

```sh
cmake --build build --target run
```

Build output is placed in `build/`. Remove that directory to start with a
clean build.

### Windows

Use the **MSYS2 UCRT64** shell. Install CMake, the compiler, and GTKmm:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain \
  mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-gtkmm3
```

Then build the application:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

To create a redistributable Windows archive, run:

```sh
cmake --build build --target package
```

This creates `build/crypto-windows.zip`. CPack uses CMake's runtime dependency
scanner to include `crypto.exe`, the license, and all required non-system
DLLs. It can be extracted and run on another compatible 64-bit Windows
machine without an MSYS2 installation.
