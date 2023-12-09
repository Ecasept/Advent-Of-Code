with open("input.txt") as f:
    data = f.readlines()

def add_difference(difs: list[list[int]]):
     # fun thing, lists are references so no need to actually return anything
    last_difs = difs[-1]
    if last_difs.count(0) == len(last_difs):
        # last line contains only zeroes
        return
    
    new_difs = []
    for i in range(len(last_difs)-1): # -1 because i+1 later 
        new_dif = last_difs[i+1] - last_difs[i]
        new_difs.append(new_dif)
    difs.append(new_difs)
    add_difference(difs)

def extrapolate(difs: list[list[int]], i: int):
    length = len(difs)
    if i == length-1:
        return 0
    new_val = difs[i][-1] + extrapolate(difs, i+1)
    return new_val

answer = 0
for line in data:
    line = [ [int(num) for num in line.strip().split(" ")] ]
    add_difference(line)
    new_val = extrapolate(line, 0)
    answer += new_val
print(answer)
