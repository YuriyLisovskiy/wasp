## Xalwart

Inspired by [Django](https://github.com/django/django).

### Requirements

C++ compiler minimum version:
* Ubuntu: g++ v8.3.0
* Microsoft C/C++: Visual Studio 2019 v16.6

### Testing

Use valgrind to check for memory leaks:
```
$ valgrind --leak-check=full ./your-executable
```
