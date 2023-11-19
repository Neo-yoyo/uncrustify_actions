#!/bin/bash

result=$(find . -type f \( -name "*.cpp" -o -name "*.hpp" \) -not -path "./uncrustify-uncrustify-0.77.1/*" -exec uncrustify --check -c uncrustify.cfg {} \; 2>&1)
# echo "$result"
echo "$result" | grep -q "FAIL"
if [ $? -eq 0 ]; then
  echo "Uncrustify check failed for the following files:"
  echo "$result" | grep "FAIL"
  exit 1
else
  echo "Uncrustify check passed for all files."
  exit 0
fi



# Uncrustify с параметром --check возвращает:
# FAIL: ./json_builder.cpp (File size changed from 19907 to 19503)
# do_source_file: Parsing: ./json_builder.cpp as language CPP
# FAIL: ./json_builder.hpp (File size changed from 4909 to 4818)
# do_source_file: Parsing: ./json_builder.hpp as language CPP
# FAIL: ./main.cpp (File size changed from 9184 to 9373)
# do_source_file: Parsing: ./main.cpp as language CPP
# do_source_file: Parsing: ./json.hpp as language CPP
# PASS: ./json.hpp (3386 bytes)
# do_source_file: Parsing: ./json.cpp as language CPP
# PASS: ./json.cpp (12135 bytes)
#
# FAIL - если файл не прошел проверку, и PASS - если файл прошел проверку
#
# Нужно изменить этот workflow, чтобы при наличии результата FAIL workflow для GitHub Actions заканчивалось неудачей и выводился список всех файлов, которые не прошли проверку


