#!/bin/bash
# Helper script to run deskflow client

export LD_LIBRARY_PATH=/home/grahampheath/Documents/deps-build/install/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=/home/grahampheath/Documents/deps-build/install/lib/x86_64-linux-gnu/pkgconfig:$PKG_CONFIG_PATH

cd /home/grahampheath/Documents/deskflow/build
./bin/deskflow-core client "$@"
