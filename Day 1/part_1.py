
with open("input.txt") as f:
    data = f.readlines()
digit_sum = 0
for line in data:
    line = [chr for chr in line.strip() if chr.isnumeric()]
    digit_sum += int(line[0] + line[-1])
print(digit_sum)
