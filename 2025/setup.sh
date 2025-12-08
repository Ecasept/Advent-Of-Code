day=$1
year=2025
file="src/days/d$day.rs"
inputs_folder="inputs/d$day"
if [ -f "$file" ]; then
	echo "Solution file already exists!"
	exit 1
fi
if [ -d "$inputs_folder" ]; then
	echo "Inputs folder already exists!"
	exit 1
fi

mkdir "$inputs_folder"
echo "Created inputs folder"
source ./.env
curl -o "$inputs_folder/input.txt" -H "Cookie: session=$SESSION_ID" "https://adventofcode.com/$year/day/$1/input"
echo "Downloaded input"


cat > "$file" << END
use crate::utils;
use aoc_macros::aoc;

#[aoc(8, 1)]
fn p1() -> Result<u64, String> {
    let input = utils::get_e(8)?;

    let mut result = 0;

    Ok(result)
}
END
codium "$file"
echo "Created and opened solution file"

touch "$inputs_folder/ex1.txt"
codium "$inputs_folder/ex1.txt"
echo "Created and opened example input file"
