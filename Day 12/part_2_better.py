import functools
import os

# I read a comment talking about this solution so I decided to implement it on my own

with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.read().split("\n")



def is_possible(leftover_states: str, leftover_springs: list[int], current_spring_pos: int, current_spring: int):
    broken_spring = leftover_springs[0]
    end = current_spring_pos + current_spring
    if end > len(leftover_states):
        return False
    for i, chr in enumerate(leftover_states[:end+1]): # +1 to account for next plot that will be between two springs
        if chr == "#":
            if not (current_spring_pos <= i < current_spring_pos + broken_spring): # if not broken
                return False
        if chr == ".":
            if current_spring_pos <= i < current_spring_pos + broken_spring: # if broken
                return False
    return True

@functools.cache
def get_possibilities(leftover_states: str, leftover_springs: tuple[int]):
    if len(leftover_springs) == 0:
        if "#" not in leftover_states:
            return 1
        return 0
    spring_length = leftover_springs[0]
    check_count = leftover_states.find("#") + 1
    if check_count <= 0:
        check_count = len(leftover_states) - spring_length + 1
    # check_end = check_end + spring_length -1 if check_end >= 0 else len(leftover_states)
    
    # if this is states
    # springs: ###
    # ???#???
    #|------|
    # ###....  -
    # .###...  | 4 = check_count
    # ..###..  |
    # ...###   -


    possibilities = 0
    for i in range(check_count):
        debug = ["."]*20
        debug[i:i+spring_length] = "#"*spring_length
        debug = "".join(debug)
        if is_possible(leftover_states, leftover_springs, i, spring_length):
            possibilities += get_possibilities(leftover_states[i+spring_length+1:], leftover_springs[1:])
    return possibilities

possibilities = 0
for line in data:
    states, functional = line.split(" ")
    functional = [int(num) for num in functional.split(",")]
    states = "?".join([states]*5)
    functional = functional * 5
    p = get_possibilities(states, tuple(functional))
    # print(p)
    # print(f"{cc} - found {p}, ratio: {succ}/{fail}")
    possibilities += p
    # cc += 1
print(possibilities)
