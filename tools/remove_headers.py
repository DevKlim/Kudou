#!/usr/bin/env python3
"""
A definitive script to recursively find and remove all variations of 
Godot-style license headers from .cpp and .h files.
It correctly handles headers composed of multiple single-line comment blocks.
"""
import os
import re
import argparse

def process_file(file_path, dry_run=False):
    """
    Processes a single file to remove a matching license header block.

    This function reads a file and looks for a contiguous block of C-style
    comment lines. If that block contains key phrases from the Godot license,
    the entire block is removed.
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()

        # Step 1: Define a regex to find a contiguous block of single-line or multi-line comments.
        # This matches one or more `/* ... */` blocks that are only separated by whitespace.
        block_regex = re.compile(r"((?:\s*/\*[\s\S]*?\*/\s*)+)")
        
        # Step 2: Define the unique phrases that identify the header.
        # If a block of comments contains any of these, it's the one we want to remove.
        identifying_phrases = [
            "This file is part of:",
            "GODOT ENGINE",
            "https://godotengine.org",
            "Copyright (c) 2014-present Godot Engine contributors",
            "Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur",
            "Permission is hereby granted, free of charge",
            "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY",
            "IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE"
        ]

        # Use finditer to check each potential block in the file.
        # We only expect one header at the top, so we can break after the first match.
        match = block_regex.search(content)
        if match:
            block_text = match.group(1)
            # Check if any of the key phrases are in the found block.
            if any(phrase in block_text for phrase in identifying_phrases):
                print(f"Header found and removed in: {file_path}")
                if not dry_run:
                    # Replace the entire matched block with an empty string
                    # and remove any leading newlines/whitespace.
                    new_content = content.replace(block_text, "").lstrip()
                    with open(file_path, 'w', encoding='utf-8', newline='\n') as f:
                        f.write(new_content)
                return True

        return False # No matching header found

    except Exception as e:
        print(f"Error processing file {file_path}: {e}")
        return False


def main():
    """
    Main function to parse arguments and scan files.
    """
    parser = argparse.ArgumentParser(
        description="Removes Godot-style license headers from .cpp and .h files."
    )
    parser.add_argument(
        "directory",
        nargs="?",
        default=".",
        help="The root directory to scan for files (default: current directory)."
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print which files would be changed without modifying them."
    )
    args = parser.parse_args()

    root_dir = os.path.abspath(args.directory)
    print(f"Scanning directory: {root_dir}")

    changed_files = 0
    # Walk through the directory tree.
    for subdir, _, files in os.walk(root_dir):
        for file in files:
            if file.endswith((".cpp", ".h")):
                file_path = os.path.join(subdir, file)
                if process_file(file_path, dry_run=args.dry_run):
                    changed_files += 1

    print("\nScan complete.")
    if changed_files > 0:
        action = "would be changed" if args.dry_run else "were changed"
        print(f"{changed_files} file(s) {action}.")
    else:
        print("No files with matching headers were found.")

if __name__ == "__main__":
    main()