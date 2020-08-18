# VisualScriptEngineWeb

[![Build status](https://ci.appveyor.com/api/projects/status/pyttwg31mi37b4hy?svg=true)](https://ci.appveyor.com/project/kovacsv/visualscriptengineweb)

The [Emscripten](https://emscripten.org) port of [VisualScriptEngine](https://github.com/kovacsv/VisualScriptEngine).

## Live Demo

[You can see the live demo here.](http://kovacsv.github.io/VisualScriptEngineWebDemo)

![Screenshot](Documentation/WebEmbeddingDemo01.png?raw=true "Web Embedding Demo")

## Building on Windows

### 1. Install Emscripten SDK

You can find detailed instruction on [Emscripten Downloand and install](https://emscripten.org/docs/getting_started/downloads.html) page. You have to use the following commands to set up the environment for VisualScriptEngineWeb:

```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
emsdk install latest
emsdk activate latest
emsdk install mingw-4.6.2-32bit
emsdk activate mingw-4.6.2-32bit
```

### 2. Generate the Visual Studio Project

You can create the project with CMake, and build it with Visual Studio.

```
mkdir Build
cd Build
cmake -G "Visual Studio 15 2017 Win64" ..
cd ..
```

### 3. Generate the Web Application

The easiest way is to run the `EmscriptenBuild.bat` file from the root directory. It needs two command line parameters:
- The Emscripten SDK folder (e.g. "C:\Emscripten\emsdk").
- The result folder of the web application (e.g. "C:\xampp\htdocs\WebApp").

```
EmscriptenBuild.bat C:\Emscripten\emsdk C:\xampp\htdocs\WebApp
```

To try out offline you have to start a web server and set it's folder as the result folder.
