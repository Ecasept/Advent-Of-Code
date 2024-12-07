#!/bin/bash

# Initialize debug flag
debug_mode=false

# Parse command-line options
while getopts ":d" opt; do
    case ${opt} in
        d )
            debug_mode=true
            ;;
        \? )
            echo "Invalid option: $OPTARG" 1>&2
            exit 1
            ;;
    esac
done
shift $((OPTIND -1))

# Check if two arguments are provided
if [ $# -lt 2 ]; then
    echo "Usage: $0  [-d] \"Day <num>\" <output>"
    exit 1
fi

# Extract the day number from the first argument
day_number=$(echo "$1" | grep -oP 'Day \K\d+')

# Construct the make target using lowercase 'day' + number
make_target="day${day_number}"

# Run the make command with the specified output and optional debug flag
if [ "$debug_mode" = true ]; then
	echo "Running make $make_target OUTPUT=$2 DEBUG=true"
    make "$make_target" OUTPUT="$2" DEBUG=true
else
	echo "Running make $make_target OUTPUT=$2"
    make "$make_target" OUTPUT="$2"
fi
