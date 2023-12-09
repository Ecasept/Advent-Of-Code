with open("input.txt") as f:
    data = f.readlines()


turn_list = list(data[0].strip())

class Node:
    def __init__(self, node_id, left=None, right=None):
        self.node_id = node_id
        self.left = left
        self.right = right
    def set_right(self, right):
        self.right = right
    def set_left(self, left):
        self.left = left


nodes = {}
node_classes: dict[str, Node] = {}

for node_text in data[2:]:
    node_id, connections = node_text.split(" = ")
    right, left = connections.strip("()\n").split(", ")
    nodes[node_id]  = (right, left)

def initialize_node(node_id):
    node_data = nodes[node_id]
    left_id = node_data[0]
    right_id = node_data[1]

    node = Node(node_id)
    node_classes[node_id] = node


    if left_id not in node_classes:
        initialize_node(left_id)
    node.set_left(node_classes[left_id])
    
    if right_id not in node_classes:
        initialize_node(right_id)
    node.set_right(node_classes[right_id])

initialize_node("AAA")

def turn_iter(lst):
    i = 0
    length = len(lst)
    while True:
        index = i % length
        yield lst[index], i
        i += 1

current_node = node_classes["AAA"]
for turn, iters in turn_iter(turn_list):
    match turn:
        case "R":
            current_node = current_node.right
        case "L":
            current_node = current_node.left
    if current_node.node_id == "ZZZ":
        print(iters + 1)
        break
