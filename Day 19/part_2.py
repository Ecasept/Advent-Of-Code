import os
from queue import Queue


with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.read()

class Workflow:
    class Rule:
        def __init__(self, val, comparison, num, result, has_condition):
            self.val: str = val
            self.comparison: str = comparison
            self.num: int = num
            self.result: str = result
            self.has_condition: bool = has_condition
        def __repr__(self):
            return f"{self.val} {self.comparison} {self.num} -> {self.result}"
    def __init__(self, name, rules):
        self.name: str = name
        self.rules: list[Workflow.Rule] = rules
    def run(self, part):
        pass

def to_workflow(workflow):
    name, rules_list = workflow
    rules = []
    for rule in rules_list:
        val, comparison, num, result, has_condition = rule
        rules.append(Workflow.Rule(val, comparison, num, result, has_condition))
    return Workflow(name, rules)

def get_workflow(name):
    return next(filter(lambda x: x[0] == name, workflows))

def test_part(part, workflow):
    def return_workflow(part, rule):
        if rule.result == "A":
            return True
        elif rule.result == "R":
            return False
        else:
            return test_part(part, get_workflow(rule.result))

    workflow = to_workflow(workflow)
    for rule in workflow.rules:
        if not rule.has_condition:
            return return_workflow(part, rule)
        if rule.comparison == "<":
            if part[rule.val] < rule.num:
                return return_workflow(part, rule)
        elif comparison == ">":
            if part[rule.val] > rule.num:
                return return_workflow(part, rule)
        else:
            raise Exception("Invalid input data")

workflows_list, parts_list = data.split("\n\n")
workflows_list: list[str] = workflows_list.split("\n")
parts_list: list[str] = parts_list.split("\n")

workflows = []

for workflow in workflows_list:
    name_end = workflow.find("{")
    name = workflow[:name_end]
    rules = workflow[name_end + 1:-1].split(",")
    rules_list = []
    for rule in rules:
        if rule.find(":") == -1:
            result = rule
            rules_list.append((None, None, None, result, False))
            continue
        condition, result = rule.split(":")
        val = condition[0]
        comparison = condition[1]
        num = int(condition[2:])
        rules_list.append((val, comparison, num, result, True))
    workflows.append((name, rules_list))

accepted_parts = []
parts = []

for part in parts_list:
    part = part.removeprefix("{").removesuffix("}")
    categories = part.split(",")
    categories_list = {}
    for category in categories:
        val = category[0]
        num = int(category[2:])
        categories_list.update({val: num})
    parts.append(categories_list)

def get_ranges_for_rule(range_set: dict[str, range], rule: Workflow.Rule):
    if not rule.has_condition:
        return None, range_set # all of them are changed
    r = range_set[rule.val]
    if rule.comparison == "<":
        if r.stop < rule.num:
            return None, range_set # all of them are changed
        elif r.start >= rule.num:
            return range_set, None # none of them are changed
        else:
            changed = range(r.start, rule.num)
            same = range(rule.num, r.stop)
    else:
        if r.start > rule.num:
            return None, range_set # all of them are changed
        elif r.stop <= rule.num:
            return range_set, None # none of them are changed
        else:
            same = range(r.start, rule.num+1)
            changed = range(rule.num+1, r.stop)
    same_set = range_set.copy()
    same_set[rule.val] = same
    changed_set = range_set.copy()
    changed_set[rule.val] = changed
    return same_set, changed_set
def get_ranges_for_workflow(part: tuple[dict[str, range], str]):
    """
    Takes a part range
    Gets the next workflow
    for each rule in the workflow:
        it checks the ranges of the part that are captured:
        for all parts that are captured: the will not be changed further
        for all parts that are not captured: the will be go through more rules
    
    """
    
    start_range_set, next_workflow = part
    workflow = to_workflow(get_workflow(next_workflow))
    finished = []
    still_running_range_sets = [start_range_set]
    for rule in workflow.rules:
        
        running = still_running_range_sets
        still_running_range_sets = []
        
        for range_set in running:
            same_range_set, changed_range_set = get_ranges_for_rule(range_set, rule)
            if same_range_set:
                still_running_range_sets.append(same_range_set)
            if changed_range_set:
                finished.append((changed_range_set, rule.result))
    if len(still_running_range_sets) > 1:
        raise Exception("still some running range sets")
    return finished

start_workflow = get_workflow("in")

part_ranges: Queue[tuple[dict[str, range], str]] = Queue()

part_ranges.put(({"x": range(1, 4001), "m": range(1, 4001), "a": range(1, 4001), "s": range(1, 4001)}, "in"))

possibilities = 0

while not part_ranges.empty():
    part = part_ranges.get()
    if part[1] == "R":
        continue
    elif part[1] == "A":
        possibilities += \
            (part[0]["x"].stop - part[0]["x"].start) * \
            (part[0]["m"].stop - part[0]["m"].start) * \
            (part[0]["a"].stop - part[0]["a"].start) * \
            (part[0]["s"].stop - part[0]["s"].start)
        continue
    new_ranges = get_ranges_for_workflow(part)
    for nr in new_ranges:
        part_ranges.put(nr)
print(possibilities)
