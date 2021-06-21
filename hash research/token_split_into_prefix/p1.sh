#!/bin/bash
gcc list_token.c -o list_token -O2 -Wall -Wextra -Wshadow
for((i=1;i<=10000;i++)); do
    ./list_token < ../pure/mail$i
    echo mail$i
done
