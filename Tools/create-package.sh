#!/bin/bash

# Resolve the script's directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define relative paths based on the script location
TARGET_DIR="$SCRIPT_DIR/../Source/UnrealMLAgents"  # Where .cpp, .h, .cs files are located
THIRDPARTY_DIR="$SCRIPT_DIR/../Source/ThirdParty"  # Where bin directories are located
BINARIES_DIR="$SCRIPT_DIR/../Binaries"             # Binaries directory
INTERMEDIATE_DIR="$SCRIPT_DIR/../Intermediate"     # Intermediate directory
GRPC_DIR="$SCRIPT_DIR/../Source/UnrealMLAgents/Grpc"  # Where .pb.cc files are located

# Define the copyright notice
COPYRIGHT="Copyright © 2025 Stephane Capponi and individual contributors. All Rights Reserved."

# Define the pragma warning directive
PRAGMA_WARNING="#pragma warning(disable : 4100 4127 4267 4324)"

echo "----------------------------------"
echo "Removing bin directories in ThirdParty..."
echo "----------------------------------"

# Find and remove all "bin" directories under Source/ThirdParty
echo "Remove all bin directories under Source/ThirdParty"
find "$THIRDPARTY_DIR" -type d -name "bin" -print -exec rm -rf {} \;

echo "----------------------------------"
echo "Removing Binaries and Intermediate directories..."
echo "----------------------------------"

# Remove Binaries and Intermediate directories
if [ -d "$BINARIES_DIR" ]; then
    rm -rf "$BINARIES_DIR"
    echo "Deleted: $BINARIES_DIR"
else
    echo "Skipped: $BINARIES_DIR (Not Found)"
fi

if [ -d "$INTERMEDIATE_DIR" ]; then
    rm -rf "$INTERMEDIATE_DIR"
    echo "Deleted: $INTERMEDIATE_DIR"
else
    echo "Skipped: $INTERMEDIATE_DIR (Not Found)"
fi

echo "----------------------------------"
echo "Adding pragma warning disable to .pb.cc files..."
echo "----------------------------------"

# Loop through all .pb.cc files in the Grpc directory
find "$GRPC_DIR" -type f -name "*.pb.cc" | while read -r file; do
    # Check if the file already contains the pragma directive (to avoid duplication)
    if ! grep -q "$PRAGMA_WARNING" "$file"; then
        # Create a temp file with the pragma warning at the top
        (echo "$PRAGMA_WARNING"; echo ""; cat "$file") > temp_file && mv temp_file "$file"
        echo "Updated: $file"
    else
        echo "Skipped (already has pragma warning): $file"
    fi
done

echo "----------------------------------"
echo "Adding copyright notice to source files..."
echo "----------------------------------"

# Loop through all .cpp, .h, and .cs files in the UnrealMLAgents directory
find "$TARGET_DIR" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.cs" -o -name "*.cc" \) | while read -r file; do
    # Check if the file already contains the copyright (to avoid duplication)
    if ! grep -q "$COPYRIGHT" "$file"; then
        # Create a temp file with the copyright notice at the top
        (echo "// $COPYRIGHT"; echo ""; cat "$file") > temp_file && mv temp_file "$file"
        echo "Updated: $file"
    else
        echo "Skipped (already has copyright): $file"
    fi
done

echo "----------------------------------"
echo "Cleanup and copyright update completed!"
echo "----------------------------------"
