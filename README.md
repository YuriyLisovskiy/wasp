## xalwart
[![c++](https://img.shields.io/badge/c%2B%2B-20-6c85cf)](https://isocpp.org/)
[![cmake](https://img.shields.io/badge/cmake-%3E=3.12-success)](https://cmake.org/)
[![alpine](https://img.shields.io/badge/Alpine_Linux-0D597F?style=flat&logo=alpine-linux&logoColor=white)](https://alpinelinux.org/)
[![ubuntu](https://img.shields.io/badge/Ubuntu-E95420?style=flat&logo=ubuntu&logoColor=white)](https://ubuntu.com/)

### Build Status
| @ | Build |
|---|---|
| Dev branch: | [![CI](https://github.com/YuriyLisovskiy/xalwart/actions/workflows/ci.yml/badge.svg?branch=dev)](https://github.com/YuriyLisovskiy/xalwart/actions/workflows/ci.yml?query=branch%3Adev) |
| Master branch: | [![CI](https://github.com/YuriyLisovskiy/xalwart/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/YuriyLisovskiy/xalwart/actions/workflows/ci.yml?query=branch%3Amaster) |

## Requirements
The following compilers are tested with the CI system, and are known to work
on Alpine Linux and Ubuntu.
* g++ 10 or later
* clang++ 10 or later

To build the library from source CMake 3.12 or later is required.

### Dependencies
The following libraries are required:
- [xalwart.base](https://github.com/YuriyLisovskiy/xalwart.base) 0.x.x or later
- [xalwart.orm](https://github.com/YuriyLisovskiy/xalwart.orm) 0.x.x or later
- [xalwart.render](https://github.com/YuriyLisovskiy/xalwart.render) 0.x.x or later

The following library is optional (you can use alternatives):
- [xalwart.server](https://github.com/YuriyLisovskiy/xalwart.server) 0.x.x or later

### Third party
- [yaml parser](https://github.com/jbeder/yaml-cpp) (already included v0.6.3 in [xalwart.base](https://github.com/YuriyLisovskiy/xalwart.base))

## Compile from Source
* `BUILD_SHARED_LIBS` means to build a shared or static library (`ON` by default).
* `XW_USE_DB_DRIVER_NAME` marks the name of a driver that will be used in ORM. `DB_DRIVER_NAME` should be
  replaced by one of the [available drivers](https://github.com/YuriyLisovskiy/xalwart.orm/tree/dev#available-drivers).
```bash
git clone https://github.com/YuriyLisovskiy/xalwart.git
cd xalwart
mkdir build
cmake -D CMAKE_BUILD_TYPE=Release \
      -D XW_USE_DB_DRIVER_NAME=ON \
      ..
make

# for linux:
sudo make install
```

## Testing
```bash
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Debug \
      -D XW_BUILD_TESTS=ON \
      ..
make unittests-all
valgrind --leak-check=full ./tests/unittests-all
```
