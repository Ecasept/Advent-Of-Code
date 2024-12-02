with open("input.txt") as f:
    data = f.read()

data = data.split("\n\n")

def to_num_list(string: str):
    return [int(num) for num in string.split(" ")]

seeds = to_num_list(data[0].split(": ")[1])
# data = data[1:]

# seeds_expanded = []
# for i in range(0, len(seeds), 2):
    # print("start")
    # start = seeds[i]
    # length = seeds[i+1]
    # seeds_expanded.append(range(start, start+length))



current_numbers = [{"val":seed, "mapped":False} for seed in seeds]

maps = []
for map in data:
    map = map.strip()
    map = [to_num_list(lst) for lst in map.split("\n")[1:]]
    maps.append(map)
data = data[::-1]

import time
def get_seed_number(loc):
    cn = loc
    for map in maps:
        for mapping in map:
            src = mapping[0]
            dst = mapping[1]
            length = mapping[2]
            if src <= cn <= src+length-1:
                # print(f"{cn} -> ", end="")
                dif = cn - src
                cn = dst + dif
                break
    return cn

def non_multithread():
    min = -1
    i = 0
    time_sum = 0
    while True:
        start = time.time()
        seed_num = get_seed_number(i)
        end = time.time()
        time_sum += end-start
        if seed_num in seeds:
            print(f"seed: {seed_num} -> loc: {i}")
            break
        if i%100000 == 0:
            if i != 0:
                print(f"{i} - {time_sum/i}")
                
        i += 1

from multiprocessing import Process, Queue

def multithread():
    
    def batch(start_seed: int, batch_size: int, q: Queue) -> int:
        global_min_val = 10**20
        for i in range(start_seed, start_seed + batch_size):
            if not q.empty():
                global_min_val = q.get_nowait()
                if global_min_val < start_seed:
                    # can't get something lower than start_seed
                    print("better min")
                    return
            seed_num = get_seed_number(i)
            if seed_num in seeds:
                if i < global_min_val:
                    q.put()
                    print("found min")
                    return
            if i%10**5==0:
                print(i)

    batch_size = 10**6

    q = Queue()
    processes = []
    for i in range(100):
        p = Process(target=batch, args=(i*batch_size, batch_size, q))
        processes.append(p)
        p.start()
    

    for p in processes:
        p.join()
    while not q.empty():
        print(q.get())

multithread()
