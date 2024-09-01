#!/bin/bash

# Purpose:
# This script generates a `compile_commands.json` file for use with code intelligence tools
# such as clangd. It scans the `src` directory for source files (*.c and *.cpp), creates
# corresponding compilation commands, and writes them to `compile_commands.json` in
# the project root directory. Object files are compiled directly into the `src` directory.
# Anybody who uses a *vi* like editor which is clangd aware can run this script directly
# from the project root directory using `./scripts/generate_compile_commands.sh` or if
# you have clangd installed outside from the editor. 

# Configuration
SRC_DIR="src"
INCLUDE_DIR="include"
COMPILE_COMMANDS_FILE="compile_commands.json"
COMPILER="gcc"
CFLAGS="-I${INCLUDE_DIR}"

# Start JSON array
echo "[" > "$COMPILE_COMMANDS_FILE"

# Function to add a compilation command to the JSON file
add_command() {
    local file="$1"
    local cmd="$2"
    local dir="$3"

    echo "  {" >> "$COMPILE_COMMANDS_FILE"
    echo "    \"directory\": \"$dir\"," >> "$COMPILE_COMMANDS_FILE"
    echo "    \"command\": \"$cmd\"," >> "$COMPILE_COMMANDS_FILE"
    echo "    \"file\": \"$file\"" >> "$COMPILE_COMMANDS_FILE"
    echo "  }," >> "$COMPILE_COMMANDS_FILE"
}

# Iterate over source files and create compile commands
for file in $(find "$SRC_DIR" -type f \( -name '*.c' -o -name '*.cpp' \)); do
    # Get the file name and path relative to src directory
    relative_file=$(realpath --relative-to="$SRC_DIR" "$file")
    
    # Define the compile command
    object_file="${SRC_DIR}/$(basename ${relative_file%.*}.o)"
    compile_command="${COMPILER} ${CFLAGS} -c -o ${object_file} $file"
    
    # Add the command to the JSON file
    add_command "$relative_file" "$compile_command" "$(pwd)"
done

# Remove the trailing comma and close the JSON array
sed -i '$ s/,$//' "$COMPILE_COMMANDS_FILE"
echo "]" >> "$COMPILE_COMMANDS_FILE"

echo "Generated $COMPILE_COMMANDS_FILE in the project root directory"

