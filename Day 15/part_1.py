with open("input.txt") as f:
    data = f.read().strip()

data = data.split(",")

def get_hash(string):
    cur_val = 0
    for char in string:
        cur_val += ord(char)
        cur_val *= 17
        cur_val %= 256
    return cur_val

answer = 0

for string in data:
    string_hash = get_hash(string)
    answer += string_hash
print(answer)
