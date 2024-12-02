from enum import Enum
import os
from queue import Queue


with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.read()


class Pulse(Enum):
    HIGH = 1
    LOW = 0
    NONE = -1

# forward declaration
class Module:pass

class Module:
    def __init__(self, name: str, next_: list[str | Module]):
        self.name = name
        self.next = next_
    def get_signal(self, pulse, sender):
        pass

class FlipFlop(Module):
    def __init__(self, name: str, next_: list[str | Module]):
        super().__init__(name, next_)
        self.on = False
    
    def get_signal(self, pulse, _):
        if pulse == Pulse.LOW:
            if self.on:
                self.on = False
                return Pulse.LOW
            else:
                self.on = True
                return Pulse.HIGH
        return Pulse.NONE
    
    def __repr__(self): return f"Flip-flop {self.name}: {'on' if self.on else 'off'}"

class Conjunction(Module):
    def init_memory(self, inputs):
        self.last_pulses = {inp: Pulse.LOW for inp in inputs}
        print(self.name, self.last_pulses)

    def get_signal(self, pulse: Pulse, sender: Module):
        self.last_pulses[sender] = pulse
        if all(pulse == Pulse.HIGH for pulse in self.last_pulses.values()):
            return Pulse.LOW
        return Pulse.HIGH
    def __repr__(self): return f"Conjunction {self.name}"


class Broadcaster(Module):
    def get_signal(self, pulse, _):
        return pulse
    def __repr__(self): return f"Broadcaster {self.name}"

class Void(Module):
    def get_signal(self, pulse, _):
        return Pulse.NONE
    def __repr__(self): return f"Void {self.name}"

def get_module(name: str):
    return next(filter(lambda x: x.name == name, modules))

modules: list[Module] = []

for line in data.split("\n"):
    p1, p2 = line.split(" -> ")
    next_ = p2.split(", ")
    if p1 == "broadcaster":
        module = Broadcaster(p1, next_)
    else:
        type_ = p1[0]
        name = p1[1:]
        if type_ == "%": # flipflop
            module = FlipFlop(name, next_)
        elif type_ == "&":
            module = Conjunction(name, next_)
    modules.append(module)
for module in modules:
    print(module, module.next)
    for i, next_ in enumerate(module.next):
        try:
            module.next[i] = get_module(next_)
        except StopIteration:
            module.next[i] = Void(next_, [])

class Signal:
    def __init__(self, sender: Module, pulse: Pulse, receiver: Module):
        self.sender = sender
        self.pulse = pulse
        self.receiver = receiver
    def __repr__(self): return f"{self.sender} -{self.pulse}> {self.receiver}"

print()

cjs = [module for module in modules if isinstance(module, Conjunction)]
for cj in cjs:
    inputs = []
    for module in modules:
        if cj in module.next:
            inputs.append(module)
    cj.init_memory(inputs)




signals: Queue[Signal] = Queue()

low_pulses = 0
high_pulses = 0

cm = "" # connected module
for module in modules:
    if "rx" in [next_.name for next_ in module.next]:
        cm = module
        break
else: raise Exception("no module connected to rx")
print(cm)
rx_in = {}
for module in modules:
    if cm in module.next:
        rx_in[module.name] = []


print(rx_in)

presses = 0
while True:
    presses += 1
    signals.put(Signal("button", Pulse.LOW, get_module("broadcaster")))

    while not signals.empty():
        signal = signals.get()
        # print(signal)
        if signal.pulse == Pulse.HIGH:
            high_pulses += 1
        elif signal.pulse == Pulse.LOW:
            low_pulses += 1
        next_signal = signal.receiver.get_signal(signal.pulse, signal.sender)
        if next_signal == Pulse.NONE:
            continue
        for next_ in signal.receiver.next:
            signals.put(Signal(signal.receiver, next_signal, next_))
        
        if signal.receiver.name in rx_in:
            rx_in[signal.receiver.name].append(presses)
    print(rx_in)

print(low_pulses * high_pulses)
