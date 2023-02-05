# colorcode

<div align="center">
  <img src="https://github.com/dk949/colorcode/blob/trunk/screenshot.png" width=75% alt="Screenshot"/>
</div>

Colourise input based on a regular expression. Like grep, but more colourful.

## Build

```sh
# Setup VCPKG
git clone "https://github.com/Microsoft/vcpkg.git"
./vcpkg/bootstrap-vcpkg.sh -disableMetrics
./vcpkg/vcpkg install

# Setup Cmake
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build and install
cmake --build build
cmake --install --prefix /preferred/install/location
```
