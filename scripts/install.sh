#!/bin/bash

echo "-- Generating headers: /usr/local/include/$XW_LIB_NAME"
cd "$XW_ROOT_DIR" || exit 1
find . -type f -name \*.h -exec install -D {} /usr/local/include/"$XW_LIB_NAME"/{} \;
