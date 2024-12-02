with open("input.txt") as f:
    data = f.readlines()

def get_copies(winners: list[int], listed: list[int], count):
    win_count = 0
    for winner in winners:
        if winner in listed:
            win_count += 1
    if win_count == 0:
        return []
    return [win_count] * count
def to_num_list(string: str):
    nums = []
    while True:
        if string:
            nums.append(int(string[0:3].strip()))
            string = string[3:]
        else:
            break
    return nums
answer = 0
copies = []
count = 0
for line in data:
    count += 1
    line = line.strip()
    pre, numbers = line.split(": ")
    card_id = pre.split(" ")[1]

    winners, listed = numbers.split(" | ")
    winners = to_num_list(winners)
    listed = to_num_list(listed)

    extra_copies = get_copies(winners, listed, len(copies) + 1)
    count += len(copies)
    copies = [num-1 for num in copies if num > 1] # decrement all copies and remove 0s
    copies.extend(extra_copies)

print(count)
