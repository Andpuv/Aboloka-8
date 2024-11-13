CC="gcc"
CFLAGS="-std=c99 -Wall -Werror -g -DDEBUG"
LDFLAGS=""

BINARY="-o bin/aboloka-cli"
SOURCES="src/cli/*.c src/core/*.c"
HEADERS="inc/*.h"

$CC $CFLAGS $SOURCES $LDFLAGS $BINARY
