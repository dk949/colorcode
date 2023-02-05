# colorcode

Colourise input based on a regular expression. Like grep, but more colourful.

<div align="center">
  <img src="https://github.com/dk949/colorcode/blob/trunk/screenshot_0.png" width=75% alt="Screenshot"/>
</div>

## Build

### Setup VCPKG

```sh
git clone "https://github.com/Microsoft/vcpkg.git"
./vcpkg/bootstrap-vcpkg.sh -disableMetrics
./vcpkg/vcpkg install
```

## Setup Cmake

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

## Build and run

```sh
cmake --build build
./build/bin/colorcode
```
