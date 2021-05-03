
cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

project(mfat)

add_library(mfat
    src/mbr.c
    src/mbytesin.c
    src/mext2.c
    src/mfat.c)
