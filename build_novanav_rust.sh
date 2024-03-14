#!/bin/bash

cargo build --release

if [ $? -eq 0 ]; then
    ./target/release/NovaNav
else
    echo "Error: Failed to build the project."
fi
