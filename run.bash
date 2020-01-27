#!/bin/bash
INPUT=`realpath $1`
cd ./build
./CrafterUI/CrafterUI $INPUT
