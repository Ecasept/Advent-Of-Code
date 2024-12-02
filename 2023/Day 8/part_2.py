from math import lcm


with open("input.txt") as f:
    data = f.readlines()


turn_list = list(data[0].strip())

class Node:
    def __init__(self, node_id, left, right):
        self.node_id: str = node_id
        self.left: str = left
        self.right: str = right


nodes: dict[str, tuple[str, str]] = {}

def n(node_id: str):
    node = nodes[node_id]
    return Node(node_id = node_id, left=node[0], right=node[1])

for node_text in data[2:]:
    node_id, connections = node_text.split(" = ")
    right, left = connections.strip("()\n").split(", ")
    nodes[node_id]  = (right, left)

def turn_iter(lst):
    i = 0
    length = len(lst)
    while True:
        index = i % length
        yield lst[index], i
        i += 1
def get_starting_nodes():
    return [n(node[0]) for node in nodes.items() if node[0].endswith("A")]
starting_nodes = get_starting_nodes()

cycle_times = []

for node in starting_nodes:
    for turn, iters in turn_iter(turn_list):
        match turn:
            case "R":
                node = n(node.right)
            case "L":
                node = n(node.left)
        if node.node_id.endswith("Z"):
            cycle_times.append(iters+1)
            break

print(lcm(*cycle_times))
