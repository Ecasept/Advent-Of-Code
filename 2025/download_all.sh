#!/bin/bash
year=2025

if [ ! -f .env ]; then
    echo ".env file not found! Please create one with your SESSION_ID."
    exit 1
fi

source .env

if [ -z "$SESSION_ID" ]; then
    echo "SESSION_ID is not set in .env"
    exit 1
fi

for day in {1..25}; do
    input_dir="inputs/d$day"
    input_file="$input_dir/input.txt"

    # Create directory if it doesn't exist
    mkdir -p "$input_dir"

    if [ -f "$input_file" ]; then
        echo "Day $day: Input already exists, skipping."
    else
        echo "Day $day: Downloading input..."
        status_code=$(curl -s -w "%{http_code}" -o "$input_file" -H "Cookie: session=$SESSION_ID" "https://adventofcode.com/$year/day/$day/input")
        
        if [ "$status_code" -ne 200 ]; then
            echo "  Failed to download (HTTP $status_code). The puzzle might not be unlocked yet."
            rm "$input_file" # Remove empty or error file
        else
            echo "  Success."
        fi
        
        # Be nice to the server
        sleep 0.2
    fi
done
