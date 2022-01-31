#!/bin/sh

find .                        \
    -maxdepth 1               \
    ! -name .                 \
    ! -name ..                \
    ! -name .git              \
    ! -name .github           \
    ! -name ds                \
    ! -name include           \
    ! -name src               \
    ! -name .clang-format     \
    ! -name bootstrap.sh      \
    ! -name clean.sh          \
    ! -name compile_flags.txt \
    ! -name configure.ac      \
    ! -name COPYING           \
    ! -name Makefile.am       \
    ! -name README            \
    -exec rm -rf "{}" +

if [ -d include ]; then 
    cd include
    find .            \
        ! -name .     \
        ! -name ..    \
        ! -name '*.h' \
        -exec rm -rf "{}" +
    cd ..
fi

if [ -d src ]; then 
    cd src
    find .                 \
        ! -name .          \
        ! -name ..         \
        ! -name '*.c'      \
        -exec rm -rf "{}" +
    cd ..
fi
