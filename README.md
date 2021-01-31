# Sliding Puzzle Solver
This is a naive [brute-force](https://en.wikipedia.org/wiki/Brute-force_search) solver for [sliding puzzles](https://en.wikipedia.org/wiki/Sliding_puzzle) written in [C++17](https://en.cppreference.com/).


## Dependencies
There are no dependencies except for reasonably new versions of [CMake](https://cmake.org/) and a C++17-capable compiler (e.g., gcc 8+ or clang 7+).

Under Ubuntu, you can install these dependencies via:
```bash
sudo  apt-get update
sudo  apt-get install build-essential cmake
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
cmake  ..  -DCMAKE_BUILD_TYPE=Release
cmake  --build .
```

You should now have the solver executables (e.g., `solve_3x3` and `solve_4x4`) under directory `build/`.


## Running
Consider the following 3x3 sliding puzzle:
```
+---+---+---+
| 8 | 4 |   |
+---+---+---+
| 1 | 2 | 6 |
+---+---+---+
| 7 | 5 | 3 |
+---+---+---+
```

Such a 3x3 puzzle is called solved iff it is in the following configuration:
```
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 7 | 8 |   |
+---+---+---+
```

We can let `solve_3x3` calculate a shortest winning strategy that transforms the given random puzzle incrementally into a solved one (note that 0 represents the hole):
```bash
$ ./solve_3x3  8 4 0  1 2 6  7 3 5
Winning strategy:
Move 6 up
Move 2 right
Move 4 down
Move 8 right
Move 1 up
Move 4 left
Move 8 down
Move 6 left
Move 2 up
Move 8 right
Move 3 up
Move 5 left
Move 8 down
Move 3 right
Move 6 down
Move 2 left
Move 3 up
Move 6 right
Move 5 up
Move 8 left
```
