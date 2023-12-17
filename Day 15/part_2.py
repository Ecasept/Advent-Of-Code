with open("input.txt") as f:
    data = f.read().strip()

data = data.split(",")

# Apparently for python 3.7+ dictionaries being ordered isn't just an implementation detail so this could be done simpler
boxes: list[list[dict[str, int]]] = [[] for _ in range(256)]

def get_hash(string):
    cur_val = 0
    for char in string:
        cur_val += ord(char)
        cur_val *= 17
        cur_val %= 256
    return cur_val

answer = 0

for string in data:
    if "-" in string:
        op = "remove"
        label = string[:string.index("-")]
    else:
        op = "set"
        label, focal_length = string.split("=")
        focal_length = int(focal_length)
    box = get_hash(label)
    if op == "remove":
        for i, lens in enumerate(boxes[box]):
            if lens["label"] == label:
                boxes[box].pop(i)
                break
    elif op == "set":
        if label not in [dct["label"] for dct in boxes[box]]:
            boxes[box].append({"label": label, "focal_length": focal_length})
        else:
            index = [dct["label"] for dct in boxes[box]].index(label)
            boxes[box][index]["focal_length"] = focal_length
    # for i, box in enumerate(boxes):
    #     if box != []:
    #         print(i, box)  
    # print("\n")  

focusing_power = 0
for box_id, box in enumerate(boxes):
    for slot_number, lense in enumerate(box):
        focusing_power += (box_id+1) * (slot_number+1) * lense["focal_length"]
print(focusing_power)
