#!/bin/bash

mkdir ../build_client

cmake -S . -B ../build_client
make -C ../build_client