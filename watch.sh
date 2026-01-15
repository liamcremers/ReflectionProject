#!/bin/bash
set -e

echo "Watching input.json for changes..."
echo "Press Ctrl+C to stop"

while true; do
    inotifywait -e modify "input.json" 2>/dev/null
    echo "Change detected! Rebuilding..."
    ./build/Reflection
done
