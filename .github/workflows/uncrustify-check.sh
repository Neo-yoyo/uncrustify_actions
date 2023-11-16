#!/bin/bash

check_format() {
  local file=$1
  local tmpfile="${file}.tmp"

  # Run Uncrustify check
  uncrustify -c uncrustify.cfg --check -f "$file" -l CPP > /dev/null 2>&1
  local exit_code=$?

  # Print the exit code for debugging
  echo "Exit code for $file: $exit_code"

  # If check fails, return 1 (failure), else return 0 (success)
  return $exit_code
}

format_file() {
  local file=$1
  local tmpfile="${file}.tmp"

  # Format the file using Uncrustify
  uncrustify -c uncrustify.cfg -f "$file" -o "$tmpfile"
  git diff --no-index "$file" "$tmpfile" || true
  mv "$tmpfile" "$file"
}

# Find and check/format CPP, C, H, HPP files
find . -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" -o -name "*.hpp" \) | while read -r file; do
  echo "Checking $file"
  check_format "$file"

  # If check fails, format the file
  if [ $? -ne 0 ]; then
    echo "File $file does not conform to code formatting rules. Formatting..."
    format_file "$file"
  else
    echo "File $file passed style checks."
  fi
done
