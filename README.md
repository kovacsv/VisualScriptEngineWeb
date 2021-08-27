# Visual Script Engine Web

[![Build status](https://ci.appveyor.com/api/projects/status/pyttwg31mi37b4hy?svg=true)](https://ci.appveyor.com/project/kovacsv/visualscriptengineweb)

The [Emscripten](https://emscripten.org) port of [VisualScriptEngine](https://github.com/kovacsv/VisualScriptEngine).

## Live Demo

[You can see the live demo here.](http://kovacsv.github.io/VisualScriptEngineWebDemo)

![Screenshot](Documentation/WebEmbeddingDemo01.png?raw=true "Web Embedding Demo")

## Building on Windows

### 1. Install Prerequisites

You have to install the following software:
- [CMake](https://cmake.org) (3.6 minimum version is needed).
- [Python](https://www.python.org) (version 2.7+ or 3.5+).

### 2. Install Emscripten SDK

You can find detailed instruction on [Emscripten Download and install](https://emscripten.org/docs/getting_started/downloads.html) page. You have to use the following commands to set up the environment for VisualScriptEngineWeb:

```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
emsdk install latest
emsdk activate latest
emsdk install mingw-4.6.2-32bit
emsdk activate mingw-4.6.2-32bit
```

### 3. Generate the Visual Studio Project (Optional)

You can create the project with CMake, and build it with Visual Studio. This step is optional. The Visual Studio project is useful if you would like to debug the functionalities, but it is not necessary for building the web application.

```
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cd ..
```

### 4. Generate the Web Application

The easiest way is to run the `em_build_win.bat` file from the root directory. It needs two command line parameters:
- The Emscripten SDK folder (e.g. "C:\Emscripten\emsdk").
- The build configuration type (Debug or Release).

```
em_build_win.bat.bat C:\Emscripten\emsdk Release
```
