#!/bin/bash

# dnf install libasan libubsan
cc rap.c -Werror -Wall -Wextra -fsanitize=undefined,address -o rap
