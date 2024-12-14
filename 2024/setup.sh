day=$1
folder="Day $day"
if [ -d "$folder" ]; then
	echo "Folder already exists!"
	exit 1
fi

mkdir "$folder"
echo "Created folder"
source ./.env
curl -o "Day $day/input.txt" -H "Cookie: session=$SESSION_ID" "https://adventofcode.com/2024/day/$1/input"
echo "Downloaded input"


cat > "$folder/solution.c" << END
#include "../lib/cutils.h"
#include <stdlib.h>

llu part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	llu sum = 0;

	free(data);
	return sum;
}

llu part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	llu sum = 0;

	free(data);
	return sum;
}
END
codium "$folder/solution.c"
echo "Created and opened solution file"

touch "$folder/ex1.txt"
codium "$folder/ex1.txt"
echo "Created and opened example input file"
