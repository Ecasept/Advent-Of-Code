with open("input.txt") as f:
    data = f.readlines()

string_numbers = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]

def extract(line: str):
    numbers = []
    last_ascii_chars = ""
    for chr in line:
        if chr.isnumeric():
            numbers.append(int(chr))
            last_ascii_chars = ""
        else:
            last_ascii_chars += chr
            print(last_ascii_chars)
            for num in string_numbers:
                if num in last_ascii_chars:
                    numbers.append(string_numbers.index(num)+1)
                    last_ascii_chars = chr # oneight is 18, not 11
    return numbers


digit_sum = 0
for line in data:
    print(line.strip())
    numbers = extract(line.strip())
    print(numbers)
    print()
    digit_sum += int(str(numbers[0]) + str(numbers[-1]))
print(digit_sum)
