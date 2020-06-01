## Xalwart

Inspired by [Django](https://github.com/django/django).

### Requirements

C++ compiler minimum version:
* Ubuntu: g++ v8.3.0
* Microsoft C/C++: Visual Studio 2019 v16.6

### Third party
Yaml library: [jbeder/yaml-cpp](https://github.com/jbeder/yaml-cpp) (v0.6.3)

### Testing

Use valgrind to check for memory leaks:
```
$ valgrind --leak-check=full ./your-executable
```
