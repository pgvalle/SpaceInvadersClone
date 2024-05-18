#!/bin/bash

# make sure to run script in rootdir
cd $(dirname $0)

deps/NAGE/build.sh
echo ""

# Specify the directory containing your .cpp files
SOURCE_DIR="src"
BUILD_DIR="build"
FLAGS="-Wall -std=c++17 -Ideps/NAGE/include -I$SOURCE_DIR $(pkg-config --cflags sdl2 SDL2_image SDL2_ttf)"
LIBS="-Ldeps/NAGE -lnage $(pkg-config --libs sdl2 SDL2_image SDL2_ttf)"

# If the first argument is "clean", then clean the build directory
if [ "$1" == "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    exit 0
fi

find_src_files() {
    find $SOURCE_DIR -type f -name '*.cpp'
}

# Create a directory to store compiled files (if it doesn't exist)
mkdir -p $BUILD_DIR

src_files=$(find_src_files)
obj_files=""

# Compile each .cpp file only if it has changed
for src in $src_files; do
    # Extract the base filename (without extension)
    base_name=$(basename "$src" .cpp)
    src_file="$BUILD_DIR/$base_name.o"

    # Check if the compiled file already exists and is up-to-date
    if [ -f $src_file ] && [ $src -ot $src_file ]; then
        echo "Skipping $src (already compiled)"
    else
        # Compile using g++ and link with libraries
        g++ -c $src -o $src_file $FLAGS
        echo "Compiled $src"
    fi

    obj_files="$obj_files $src_file"
done

# Create a single executable from all compiled files
g++ $obj_files -o sic.out $LIBS
