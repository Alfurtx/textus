#!/bin/bash

CC=clang
CWD=$(pwd)
CFLAGS="-std=c11 -g -Wall -Wpedantic -Wno-unused-parameter -Wno-switch -Wno-unused-function -I${CWD}/dependencies/glad/include -I${CWD}/dependencies/glfw/include"
LDFLAGS="-lm -lglfw3 -ldl -lpthread -lGL -lX11 -lXrandr -lXi"
LDFLAGS="$LDFLAGS -L${CWD}/dependencies/glfw/build/src/"
CFILES="$(echo ${CWD}/code/*.c)"
CFILES="$CFILES ${CWD}/dependencies/glad/src/glad.c"

# GLFW_CFLAGS="$(pkg-config --cflags glfw3)"
# GLFW_LDFLAGS="$(pkg-config --static --libs glfw3)"

# CFLAGS="$CFLAGS $GLFW_CFLAGS"
# LDFLAGS="$LDFLAGS $GLFW_LDFLAGS"

mkdir -p bin
pushd ./bin > /dev/null
${CC} ${CFILES} -o textus.exe ${LDFLAGS} ${CFLAGS}
popd > /dev/null
