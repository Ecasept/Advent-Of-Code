with open("input.txt") as f:
    data = f.readlines()

def get_points(winners: list[int], listed: list[int]):
    win_count = 0
    for winner in winners:
        if winner in listed:
            win_count += 1
    if win_count == 0:
        return 0
    return 2**(win_count-1)
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
for line in data:
    line = line.strip()
    pre, numbers = line.split(": ")
    card_id = pre.split(" ")[1]

    winners, listed = numbers.split(" | ")
    winners = to_num_list(winners)
    listed = to_num_list(listed)
    answer += get_points(winners, listed)

print(answer)
