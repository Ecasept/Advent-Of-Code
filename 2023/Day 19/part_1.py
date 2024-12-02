with open("input.txt") as f:
    data = f.read()

class Workflow:
    class Rule:
        def __init__(self, val, comparison, num, result, has_condition):
            self.val: str = val
            self.comparison: str = comparison
            self.num: int = num
            self.result: str = result
            self.has_condition: bool = has_condition
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

start_workflow = get_workflow("in")

for part in parts:
    is_accepted = test_part(part, start_workflow)
    if is_accepted:
        accepted_parts.append(part)

answer = 0
for part in accepted_parts:
    answer += part["x"] + part["m"] + part["a"] + part["s"]
print(answer)
