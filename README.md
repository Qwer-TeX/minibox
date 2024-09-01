
# MiniBox
Ultra-small and simple micro-embedded Linux command set

![MiniBox](https://github.com/user-attachments/assets/5e0caa54-876e-40a6-8fe3-186027ac4fa3)

MiniBox is not a fork of BusyBox or ToyBox; it is entirely written from scratch. Inspired by the "small and simple is beautiful" philosophy of Thompson and Ritchie, MiniBox embodies a minimalistic approach that aligns with its name.

## What is this project?
MiniBox is a collection of ultra-miniature C utilities comparable to the full-sized GNU Coreutils. The key difference is that MiniBox utilities omit uncommon options and prioritize size over features. Each utility can be compiled standalone, separate from the MiniBox suite.

## Why MiniBox?
Why choose MiniBox? Here are some reasons:

1. **Extremely Small**: MiniBox comes in under 60KB.
2. **Blazingly Fast**: It is 7.3x faster than BusyBox and 3.1x faster than ToyBox.
3. **Simple Source Code**: The code is easy to understand with zero bloat.
4. **Universal libc Support**: Compatible with dietlibc, uclibc, musl, glibc, etc.
5. **No Dependencies**: Except for libc, MiniBox requires no external dependencies.
6. **Standalone Utilities**: Each utility can be compiled independently (see Standalone Compilation).

## How to Build
Follow these simple steps to build MiniBox:

1. **Configure**:
   - Run `./configure` (no options) to configure the build.
   - To compile all commands, you can use `yes | ./configure`.

2. **Compile**:
   - Run `make`. (if you select yes to continue, `./configure` would do it for you)

3. **Install**:
   - Run `make install` to install MiniBox to `install_dir`.

### Cleaning Up
- **Remove compiled objects**: Run `make distclean`.
- **Uninstall**: Since files are copied to `install_dir`, you can't uninstall traditionally, but you can clean up with `make distclean`.

### Important Notes
If you need to edit any code after running `./configure`, you must first run:

1. `make distclean`
   - **OR**
2. `./scripts/toggle_ifdef.sh -D` (recommended)

This will remove the `#ifdef` statements from the source files so that everything compiles without running `./configure`.

## Standalone Compilation
To compile a utility standalone:

1. **Clean Up**:
   - If you've already run `./configure` or `make`, run `make distclean` or `./scripts/toggle_ifdef.sh -D`.

2. **Edit the Source**:
   - Remove `#include "minibox.h"` and replace it with the necessary headers and function definitions (or copy the source to your home directory before modifying).

3. **Add a Main Function**:
   - Your main function should look like this:

```c
int main(int argc, char *argv[]) {
    return example(argc, argv);
}
```

4. **Handle the VERSION Macro**:
   - Either add `#define VERSION <some_random_chars>` or remove `VERSION` and the corresponding `%s` in the printf statement.

5. **Handle Reusable Functions**:
   - If your program uses functions from `libmb`, either copy the code from `libmb/<function_name>.c` into your program or compile `libmb` and link it with `-Llibmb -lmb`.

6. **Compile**:
   - Use `gcc -flto -o <utility> <utility>.c`. You can add additional compilation flags as needed.

7. **Optional: Static Compilation**:
   - To compile statically, use `gcc -static -flto -o <utility> <utility>.c -lc`. If compilation fails, ensure that all necessary libraries are included.

## Adding Your Own Utilities
If you want to contribute or add your own utilities:

1. **Create the Source File**:
   - Create the source file (e.g., `src/example.c`).

2. **Include Headers**:
   - Include `#include "minibox.h"` in your source file.
   - If your program has multiple functions, also include `#include "utils.h"`.

3. **Define Functions**:
   - Add the function definitions to `minibox.h`:

```c
int example(int argc, char *argv[]);
```

4. **Implement the Program**:
   - Implement the program in `src/example.c`. Manual option parsing is required (do not use `getopt`).

5. **Update the Commands Table**:
   - Add your program to the commands table in `src/minibox.c`:

```c
Command commands[] = {
    #ifdef CONFIG_EXAMPLE
        {"example", example},
    #endif
};
```

6. **Edit the Makefile**:
   - Add your program to the `PROGS` variable in the Makefile.

7. **Test**:
   - Run `./configure` and select your program for compilation.
