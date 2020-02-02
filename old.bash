#!/bin/bash
INPUT=`realpath $1`
cd ./build
./Crafter/src/GraphConstruct $INPUT
