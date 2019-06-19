# !/bin/bash

./remove.sh
./compile.sh

./client -h 127.0.0.1 -p 3333
