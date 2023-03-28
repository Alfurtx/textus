#!/bin/bash

CC=clang
CWD=$(pwd)
CFLAGS="-std=c11 -g -Wall -Wpedantic -Wno-unused-parameter -Wno-switch -Wno-unused-function"
CFLAGS="$CFLAGS -Wno-incompatible-pointer-types-discards-qualifiers"
CFLAGS="$CFLAGS -MJ compile_commands.json" # for compilation database (LSP/compile_commands.json) file creation
CFLAGS="$CFLAGS -I${CWD}/dependencies/glfw/include"
CFLAGS="$CFLAGS -I${CWD}/dependencies/glad/include"
CFLAGS="$CFLAGS -I${CWD}/dependencies/freetype/release/include/freetype2"

LDFLAGS="-lm -lglfw3 -ldl -lpthread -lGL -lX11 -lXrandr -lXi"
LDFLAGS="$LDFLAGS -lfreetype"
LDFLAGS="$LDFLAGS -L${CWD}/dependencies/glfw/build/src/"
LDFLAGS="$LDFLAGS -L${CWD}/dependencies/freetype/release/lib/"

CFILES="$(echo ${CWD}/code/*.c)"
CFILES="$CFILES ${CWD}/dependencies/glad/src/glad.c"

mkdir -p bin
pushd ./bin > /dev/null
${CC} ${CFILES} -o textus.exe ${LDFLAGS} ${CFLAGS}
echo -e "[$(cat compile_commands.json)]" > compile_commands.json
mv compile_commands.json ..
popd > /dev/null

${CWD}/bin/textus.exe
