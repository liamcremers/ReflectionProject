#!/bin/bash
set -e

[[ ! -f /etc/os-release ]] && { echo "Error: Linux/WSL required"; exit 1; }

echo "Installing system packages..."
sudo apt-get update
sudo apt-get install -y cmake ninja-build unzip wget build-essential inotify-tools jq

if command -v code &> /dev/null; then
    echo "Installing VS Code extensions..."
    cat .vscode/extensions.json | jq -r '.recommendations[]' | xargs -I {} code --install-extension {} 2>/dev/null || true
else
    echo "Warning: VS Code not found"
fi

echo "==================================="
echo "Setup complete!"
echo "==================================="

cmake -S . -B build -G Ninja   -DCMAKE_TOOLCHAIN_FILE=clang-toolchain.cmake   -DCMAKE_BUILD_TYPE=Debug   -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 
cmake --build build

echo "Build finished!"
