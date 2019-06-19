#!/bin/bash

gcc -c -o server.o server.c
gcc -c -o client.o client.c
gcc -o server server.o
gcc -o client client.o
