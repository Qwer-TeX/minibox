#!/bin/bash

SRC_DIR="src"
CONFIG_PREFIX="CONFIG_"

# Function to add #ifdef guards and #include "config.h"
add_ifdef_guards() {
    for file in "$SRC_DIR"/*.c; do
        filename=$(basename "$file" .c)
        # Never touch the core dispatcher; it has special handling for
        # CONFIG_MINIBOX and unconditional new commands + sentinel.
        if [ "$filename" = "minibox" ]; then
            continue
        fi
        macro="${CONFIG_PREFIX}$(echo $filename | tr '[:lower:]' '[:upper:]')"

        # Check if the file already has the guard
        if grep -q "^#ifdef $macro" "$file"; then
            echo "Guard already exists in $file"
            continue
        fi

        # Create a temporary file to hold the new content
        tmp_file=$(mktemp)

        # Add #include "config.h" and #ifdef guard
        {
            # Add #include "config.h" if it's not already present
            if ! grep -q '#include "config.h"' "$file"; then
                echo "#include \"config.h\""
            fi

            # Add #ifdef guard and existing content
            echo "#ifdef $macro"
            cat "$file"
            echo "#endif /* $macro */"
        } > "$tmp_file"

        # Replace the original file with the updated file
        mv "$tmp_file" "$file"
    done
}

# Function to remove #ifdef guards and #include "config.h"
remove_ifdef_guards() {
    for file in "$SRC_DIR"/*.c; do
        filename=$(basename "$file" .c)
        # Protect core + the added commands (not in base git history) so that
        # distclean + "make clean && make" still yields a full working build
        # with clean gcc lines (no need for the old -DCONFIG spam).
        case "$filename" in
            minibox|help|uname|mkfifo|ln|chmod|chown|printf|pidof|lsmod|rmmod|insmod|modprobe|mount|umount|df|su|login|poweroff)
                continue ;;
        esac
        macro="${CONFIG_PREFIX}$(echo $filename | tr '[:lower:]' '[:upper:]')"

        # Remove #ifdef guard
        if grep -q "^#ifdef $macro" "$file"; then
            echo "Removing #ifdef guard from $file"
            tmp_file=$(mktemp)
            sed -e "/^#ifdef $macro/,/^#endif \/\* $macro \*\//{//!b};d" "$file" > "$tmp_file"
            mv "$tmp_file" "$file"
        else
            echo "No guard found in $file"
        fi

        # Remove #include "config.h" if it's at the top
        if grep -q '#include "config.h"' "$file"; then
            echo "Removing #include \"config.h\" from $file"
            tmp_file=$(mktemp)
            sed '/^#include "config.h"/d' "$file" > "$tmp_file"
            mv "$tmp_file" "$file"
        fi
    done
}

# Check for the -D flag
if [[ "$1" == "-D" ]]; then
    remove_ifdef_guards
else
    add_ifdef_guards
fi

