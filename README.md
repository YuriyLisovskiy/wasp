## xalwart

| build | info |
|---|---|
| [![gcc](https://github.com/YuriyLisovskiy/xalwart/actions/workflows/tests-gcc.yml/badge.svg)](https://github.com/YuriyLisovskiy/xalwart/actions/workflows/tests-gcc.yml) [![clang](https://github.com/YuriyLisovskiy/xalwart.server/actions/workflows/tests-clang.yml/badge.svg)](https://github.com/YuriyLisovskiy/xalwart.server/actions/workflows/tests-clang.yml) | [![c++](https://img.shields.io/badge/c%2B%2B-20-6c85cf)](https://isocpp.org/) [![system](https://img.shields.io/badge/Ubuntu-OS-ef4800.svg?style=flat&logo=ubuntu)](https://ubuntu.com/) |

Inspired by [Django](https://github.com/django/django).

### Requirements
- C++ compiler:
    - `gcc`: v10 or later.
    - `clang`: v10 or later.
- CMake: `3.12` or later.
- Additional libraries:
    - [xalwart.core](https://github.com/YuriyLisovskiy/xalwart.core) (required)
    - [xalwart.render](https://github.com/YuriyLisovskiy/xalwart.render) (required)
    - [xalwart.server](https://github.com/YuriyLisovskiy/xalwart.server) (optional)
    - [xalwart.orm](https://github.com/YuriyLisovskiy/xalwart.orm) (optional)

### Third party
- [yaml parser](https://github.com/jbeder/yaml-cpp) (already included v0.6.3)

### Build and Install
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && sudo make install
```

### Testing
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug .. && make
```
Use valgrind to check for memory leaks:
```bash
valgrind --leak-check=full ./tests/unittests-all
```
