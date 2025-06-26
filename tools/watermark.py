# tools/remove_headers.py
#!/usr/bin/env python3
"""
A script to recursively find and remove Godot-style license headers
from .cpp and .h files.
"""
import os
import re
import argparse

def process_file(file_path, dry_run=False):
    """
    Processes a single file to remove a matching license header.

    The script looks for a multi-line C-style comment block (/* ... */)
    that contains the phrase "This file is part of:" followed by either
    "GODOT ENGINE" or "KUDOU ENGINE". This ensures it targets the correct
    headers while ignoring other comment blocks.
    """
    # Regex to find the header. It's designed to be flexible.
    header_regex = re.compile(
        r'\s*'
        r'/\*+'  # Start of a C-style comment block.
        r'[\s\S]*?'  # Non-greedy match for any characters (including newlines).
        r'This file is part of:'
        r'[\s\S]*?'  # Match everything until the project name.
        r'(GODOT ENGINE|KUDOU ENGINE)'  # Match either project name.
        r'[\s\S]*?'  # Match the rest of the comment block.
        r'\*+/\s*\n?',  # End of the comment block.
        re.MULTILINE
    )

    try:
        # Read with UTF-8, which is standard for Godot source files.
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()

        # Replace the found header with an empty string.
        new_content, count = header_regex.subn('', content)

        if count > 0:
            print(f"Header found and removed in: {file_path}")
            if not dry_run:
                # Write the modified content back to the file.
                with open(file_path, 'w', encoding='utf-8', newline='\n') as f:
                    # lstrip() removes leading whitespace/newlines left after removal.
                    f.write(new_content.lstrip())
            return True
        else:
            # No header found, no changes needed.
            return False

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