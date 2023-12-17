from dataclasses import dataclass
import os


with open(os.path.join(os.path.dirname(__file__),"test.txt")) as f:
    data = f.read()

city_map = data.split("\n")

import enum

class Dir(enum.Enum):
    N = 0
    E = 1
    S = 2
    W = 3

type Node = tuple(int, int)

def node_in_direction(node, direction):
    x, y = node
    if direction == Dir.N:
        return (x, y-1)
    elif direction == Dir.E:
        return (x+1, y)
    elif direction == Dir.S:
        return (x, y+1)
    elif direction == Dir.W:
        return (x-1, y)

def next_nodes(current_node: Node, last_direction: Dir, direction_count: int):
    # x, y = current_node
    nodes = []
    for dir in [Dir.N, Dir.E, Dir.S, Dir.W]:
        if dir != last_direction:
            nodes.append((node_in_direction(current_node, dir), dir, 1))
        else:
            if direction_count < 3:
                nodes.append((node_in_direction(current_node, dir), dir, direction_count+1))
    return nodes

@dataclass
class Path:
    nodes: tuple[Node] = ()
    heat_loss: int = 0
    current_direction: Dir = Dir.E
    direction_count: int = 0


start_node = (0, 0)
end_node = (len(city_map[0])-1, len(city_map)-1)

paths: list[Path] = [Path(nodes=(start_node,), heat_loss=0)]


winner_path = None
winner_heat_loss = None

while True:
    new_paths: list[Path] = []
    print(len(paths))
    print(max([len(path.nodes) for path in paths]))
    for path in paths:
        end_node = path.nodes[-1]
        heat_loss = path.heat_loss
        nodes = next_nodes(end_node, path.current_direction, path.direction_count)
        for node, cur_dir, dir_count in nodes:
            node_heat_loss = int(city_map[node[1]][node[0]])
            total_heat_loss = heat_loss + node_heat_loss

            new_path = Path(nodes=path.nodes + (node,), heat_loss = total_heat_loss, current_direction=cur_dir, direction_count=dir_count)
            
            if node == end_node:
                if winner_path:
                    if winner_heat_loss > total_heat_loss:
                        winner_path = new_path
                        winner_heat_loss = total_heat_loss
                else:
                    winner_path = new_path
                    winner_heat_loss = total_heat_loss
            else:
                new_paths.append(new_path)

    paths = new_paths
    if paths == []:
        break

print(winner_heat_loss)

