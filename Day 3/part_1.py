


with open("input.txt") as f:
    data = f.readlines()

class Number:
    def __init__(self, line: int, start: int, end: int, string_value: str):
        self.line = line
        self.start = start
        self.end = end
        self.string_value = string_value
    def get_cells_above(self, schematic_width: int, schematic_height: int) -> list[int]:
        start = self.start-1
        end = self.end
        if self.line == 0: # if first line
            return []
        if self.start == 0:
            start += 1 # remove first cell
        if self.end == schematic_width:
            end -= 1 # remove last cell
        return list(range(start, end+1))

    def get_cells_below(self, schematic_width: int, schematic_height: int) -> list[int]:
        start = self.start-1
        end = self.end
        if self.line == schematic_height - 1: # if last line
            return []
        if self.start == 0:
            start += 1 # remove first cell
        if self.end == schematic_width:
            end -= 1 # remove last cell
        return list(range(start, end+1))
    
    def get_cells_beside(self, schematic_width: int, schematic_height: int) -> list[int]:
        ret = []
        if self.start != 0:
            ret.append(self.start - 1)
        if self.end != schematic_width:
            ret.append(self.end)
        return ret

    def __repr__(self):
        return f"{self.line}/{self.start}-{self.end}: {self.string_value}"

numbers = []
l = 0
sw = len(data[0].strip())
sh = len(data)
for line in data:
    line = line.strip()
    
    start = -1
    currently_in_number = False
    current_number = ""
    i = 0
    for chr in line:
        if chr.isnumeric():
            if currently_in_number:
                current_number += chr
            else:
                start = i
                currently_in_number = True
                current_number = chr
        else:
            if currently_in_number:
                numbers.append(Number(l, start, i, current_number))
                currently_in_number = False
                start = -1
                current_number = ""
        i += 1
    if currently_in_number:
        numbers.append(Number(l, start, i, current_number))
    l += 1

def is_symbol(cell: str):
    if cell.isnumeric() or cell == ".":
        return False
    return True

def check_cells(line: int, cell_pos: int):
    for pos in cell_pos:
        if is_symbol(data[line][pos]):
            return True
    return False
def is_engine_number(number: Number):
    cells_above_pos = number.get_cells_above(sw, sh)
    if check_cells(number.line - 1, cells_above_pos):
        return True
    cells_below_pos = number.get_cells_below(sw, sh)
    if check_cells(number.line + 1, cells_below_pos):
        return True
    cells_beside_pos = number.get_cells_beside(sw, sh)
    if check_cells(number.line, cells_beside_pos):
        return True
    return False

answer = 0
for number in numbers:
    if is_engine_number(number):
        answer += int(number.string_value)
print(answer)
