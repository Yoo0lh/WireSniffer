#!/bin/bash 

if [[ $1 == "run" ]]
then 
  gcc -Wall -Wextra -Werror main.c -o wiresniffer
  ./wiresniffer  
  rm -f wiresniffer
elif [[ $1 == "clean" ]]
then
  rm -f wiresniffer; 
fi

:
