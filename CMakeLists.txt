cmake_minimum_required(VERSION 3.5.0)
project(rsjson VERSION 0.1.0 LANGUAGES C)

add_subdirectory(tests)

add_library(rsjson
    header.h
    rsmalloc.h rsmalloc.c
    rsvtable.h rsvtable.c
    rsjson.h rsjson.c)

