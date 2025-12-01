day=$1
year=2025
folder="src/d$day"
inputs_folder="inputs/d$day"
if [ -d "$folder" ]; then
	echo "Solution folder already exists!"
	exit 1
fi
if [ -d "$inputs_folder" ]; then
	echo "Inputs folder already exists!"
	exit 1
fi

mkdir "$folder"
mkdir "$inputs_folder"
echo "Created folders"
source ./.env
curl -o "$inputs_folder/input.txt" -H "Cookie: session=$SESSION_ID" "https://adventofcode.com/$year/day/$1/input"
echo "Downloaded input"


cat > "$folder/solution.rs" << END
END
codium "$folder/solution.rs"
echo "Created and opened solution file"

touch "$inputs_folder/ex1.txt"
codium "$inputs_folder/ex1.txt"
echo "Created and opened example input file"
