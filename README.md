# colorcode

Colourise input based on a regular expression. Like grep, but more colourful.


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

See [Configuration options](Configuration-options) to change executable name.
