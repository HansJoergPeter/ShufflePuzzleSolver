# Sliding Puzzle Solver
This is a naive brute-force solver for [sliding puzzles](https://en.wikipedia.org/wiki/Sliding_puzzle) written in C++17.


## Dependencies
There are no dependencies except for reasonably new versions of [CMake](https://cmake.org/) and a C++17-capable compiler (e.g., gcc 8+ or clang 7+).

Under Ubuntu, you can install these dependencies via:
```bash
sudo  apt-get update  &&  apt-get install build-essentials cmake clang
```

There is an optional dependency to [Google's Abseil library](https://abseil.io/).
If Abseil is available, the usage of `std::unordered_map` is replaced by `absl::flat_hash_map`, which leads to a better performance.


## Compiling
The solver command-line executables can be built using the standard CMake process.
Feel free to add any additional configuration parameters to the first call to cmake.
For example,
```bash
-DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

```bash
# in the root directory of the repo
mkdir build
cd build
cmake  ..
cmake  --build .
```

You should now have the solver executables (e.g., `solve_3x3` and `solve_4x4`) under directory `build/`.
