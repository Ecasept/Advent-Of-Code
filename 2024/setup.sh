day=$1
folder="Day $day"
if [ -d "$folder" ]; then
	echo "Folder already exists!"
	exit 1
fi

mkdir "$folder"
echo "Created folder"
./getinput.sh $day
echo "Downloaded input"


cat > "$folder/solution.cpp" <<- END
#include "../utils.h"


void part1() {
	auto file = utils::getExample(1);
	std::string line;

    while (std::getline(file, line)) {
    }
    std::cout << "not implemented" << '\n';
}

void part2() {
    auto file = utils::getExample(1);
	std::string line;

    while (std::getline(file, line)) {
    }
    std::cout << "not implemented" << '\n';
}

int main() {
    part1();
    part2();
    return 0;
}
END
codium "$folder/solution.cpp"
echo "Created and opened solution file"

touch "$folder/ex1.txt"
codium "$folder/ex1.txt"
echo "Created and opened example input file"
