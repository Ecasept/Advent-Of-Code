with open("input.txt") as f:
    data = f.read()

data = data.split("\n\n")

def to_num_list(string: str):
    return [int(num) for num in string.split(" ")]

seeds = to_num_list(data[0].split(": ")[1])
data = data[1:]

current_numbers = [{"val":seed, "mapped":False} for seed in seeds]

for map in data:
    map = map.strip()
    print(map.split("\n")[0], current_numbers)
    # print(current_numbers)
    map = [to_num_list(lst) for lst in map.split("\n")[1:]]
    for mapping in map:
        dst = mapping[0]
        src = mapping[1]
        length = mapping[2]
        for cn in current_numbers:
            if cn["mapped"] == True:continue
            if src <= cn["val"] <= src+length-1:
                dif = cn["val"] - src
                cn["val"] = dst + dif
                print(f"{cn['val']}")
                cn["mapped"] = True
    for cn in current_numbers:
        cn["mapped"] = False
min_loc = min([num["val"] for num in current_numbers])
print(min_loc)
