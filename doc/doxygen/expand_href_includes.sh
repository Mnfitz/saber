#!/bin/bash

# Check if the input file is provided
if [ -z "$1" ]; then
    echo "Usage: $0 input.html"
    exit 1
fi

input_file="$1"
output_file="$2"

# Function to inline expand href-include directives
expand_includes() {
    local file="$1"
    while IFS= read -r line; do
        if [[ "$line" =~ href-include=\"([^\"]+)\" ]]; then
            include_file="${BASH_REMATCH[1]}"
            if [ -f "$include_file" ]; then
                cat "$include_file"
            else
                echo "<!-- File $include_file not found -->"
            fi
        else
            echo "$line"
        fi
    done < "$file"
}

# Process the input file and write to the output file
expand_includes "$input_file" > "$output_file"

# echo "Output written to $output_file"
