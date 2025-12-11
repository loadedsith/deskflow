#!/bin/bash
# Helper script to set up environment and build deskflow

export PKG_CONFIG_PATH=/home/grahampheath/Documents/deps-build/install/lib/x86_64-linux-gnu/pkgconfig:$PKG_CONFIG_PATH
export PATH=/home/grahampheath/Documents/cmake-build/install/bin:$PATH

cd /home/grahampheath/Documents/deskflow/build

echo "Environment configured:"
echo "  - CMake: $(cmake --version | head -1)"
echo "  - libei: $(pkg-config --modversion libei-1.0)"
echo "  - libportal: $(pkg-config --modversion libportal)"
echo ""
echo "To build: cmake --build ."
echo "To configure: cmake -S.. -B. -DQt6_DIR=/usr/lib/libqt-jami/lib/cmake/Qt6"
