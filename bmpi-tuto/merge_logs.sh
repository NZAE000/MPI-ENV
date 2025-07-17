#!/bin/bash

# Verification of arguments.
if [ $# -ne 1 ]; then
    echo "Uso: $0 <directorio_logs>"
    exit 1
fi

log_dir="$1"

# Verify that the directory exists.
if [ ! -d "$log_dir" ]; then
    echo "Error: El directorio '$log_dir' no existe."
    exit 1
fi

# Output file.
output_file="$log_dir/merged_log.txt"

# Awk compatible version of macOS.
awk '
{
    # Buscar la parte entre corchetes
    if (match($0, /\[[0-9:]*\]/)) {
        timestamp = substr($0, RSTART+1, RLENGTH-2)  # Remove brackets.
        gsub(":", "", timestamp)                     # Remove the colon.
        print timestamp "|" $0
    }
}
' "$log_dir"/log_*.txt \
| sort \
| cut -d"|" -f2- > "$output_file"

echo "Combined file generated in: $output_file"

