#!/bin/bash

# Directory to iterate through
directory="Examples"

# Output file

# Iterate through the directory
for file in "$directory"/*; do
    output_file="$directory"/"$(basename "$file")".output

    # Check if the file is a regular file
    if [[ -f "$file" ]]; then
        # Run the 'tree_builder' program with the filename as an argument and redirect the output to the output file
        # Get the extension of the file
        extension="${file##*.}"
         # Check if the extension is txt or csv
        if [[ "$extension" == "tb" ]]; then
            # Run the 'tree_builder' program with the filename as an argument and redirect the output to the output file
            ./tree_builder "$file" &> "$output_file"


            new_extension=".out"
            test_output_file="$directory"/"$(basename "$file" .tb)""$new_extension"
            
            # compare original and output files
            diff "$test_output_file" "$output_file" > /dev/null
            if [ $? -eq 0 ]; then
                echo "Test passed for $file"
            else
                echo "Test failed for $file"
            fi
        fi
    fi

    
done