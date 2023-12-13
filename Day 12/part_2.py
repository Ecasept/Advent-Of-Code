import functools
import os


with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.read().split("\n")

def check_is_possible(states: str, non_op_pos: list[dict]):
    global c, succ, fail 
    # debug = ["."]*20
    # for nop in non_op_pos:
        # debug[nop["start"]:nop["start"]+nop["length"]] = ["#"] * nop["length"]
    # debug = "".join(debug)
    
    def is_operational(non_op_pos, pos):
        for nop in non_op_pos:
            if nop["start"] <= pos < nop["start"] + nop["length"]:
                return False
        return True
    last = max(map(
        lambda x:x["start"]+x["length"],
        non_op_pos
    ))

    for i, state in enumerate(states):
        if i >= last:
            break
        if state == ".":
            if not is_operational(non_op_pos, i):
                fail += 1
                return False
        elif state == "#":
            if is_operational(non_op_pos, i):
                fail += 1
                return False
    succ += 1
    return True

c = 0
cc= 0
succ = 0
fail = 0

import time

def get_possibilities(states: str, non_op: list[int], non_op_pos: list[dict], index):
    global c, succ, fail
    c+=1
    if c%1000000 == 0:
        print(c)


    if index == len(non_op):
        # if, after checking all the hot springs, we have reached this point, then this is a valid possibility
        return 1 #ret

    
    start = time.time()

    non_op_pos = non_op_pos.copy()

    if len(non_op_pos) == 0:
        prev_end = 0
    else:
        prev_row = non_op_pos[-1]
        prev_end = prev_row["start"] + prev_row["length"] + 1 # +1 because there is always min 1 empty space between functional entries
    this = non_op[index]

    non_op_pos.append({"start": -1, "length": this})

    poss_count = 0
    for pos in range(prev_end, len(states)):
        if pos + this > len(states): continue
        non_op_pos[-1]["start"] = pos
        if not check_is_possible(states, non_op_pos): continue
        poss_count += get_possibilities(states, non_op, non_op_pos, index+1)
    return poss_count
possibilities = 0
for line in data:
    states, functional = line.split(" ")
    functional = [int(num) for num in functional.split(",")]
    states = "?".join([states]*5)
    functional = functional * 5
    p = get_possibilities(states, functional, [],0)
    print(f"{cc} - found {p}, ratio: {succ}/{fail}")
    possibilities += p
    cc += 1
print(possibilities)
