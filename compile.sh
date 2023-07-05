#!/bin/bash

if [[ $1 == "run" ]]
then
  gcc -Wall -Wextra main.c -o wiresniffer -fsanitize=address -g
  ./wiresniffer
  rm -f wiresniffer
elif [[ $1 == "clean" ]]
then
  rm -f wiresniffer;
  rm -f log_file.txt;
fi

