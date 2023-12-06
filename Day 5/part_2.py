def to_num_list(string: str):
    return [int(num) for num in string.split(" ")]

with open("input.txt") as f:
    data = f.read()

data = data.split("\n\n")
seed_numbers = to_num_list(data[0].split(": ")[1])
data = data[1:]

seeds = []
for i in range(0, len(seed_numbers), 2):
    start = seed_numbers[i]
    length = seed_numbers[i+1]
    seeds.append(range(start, start+length))

maps = []
for map in data:
    map = map.strip()
    map = [to_num_list(lst) for lst in map.split("\n")[1:]]
    maps.append(map)

def calc(num):
    for map in maps:
        for mapping in map:
            dst = mapping[0]
            src = mapping[1]
            length = mapping[2]
            if src <= num <= src+length-1:
                dif = num - src
                num = dst + dif
                break
    return num
import time
seed_for_min_loc = -1
min_loc = 10**15
time_sum = 0
i = 0
for seed_range in seeds:
    for seed in seed_range:
        loc = calc(seed)
        time_sum += end-start
        i += 1
        if loc<min_loc:
            min_loc = loc
            seed_for_min_loc = seed
            print(min_loc, seed_for_min_loc)
        if i%10**6 == 0:
            if i != 0:
                print(time_sum/i)
