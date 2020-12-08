#!/bin/bash

mkdir ../build_server

cmake -S . -B ../build_server
make -C ../build_server