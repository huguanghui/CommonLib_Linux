#!/bin/bash
dir=`pwd | awk -F\/ '{print $NF}'`
TARGET=lib$dir.so
echo $TARGET