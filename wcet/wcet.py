import numpy as np

# Tasks
no_id       = 0
mission     = 1
navigate    = 2
control     = 3
refine      = 4
report      = 5
communicate = 6
avoid       = 7

# Scenarios
corner = 1
field_others = 2
field_none = 3
field_victim = 4
field_triple_victim = 5

N = 20

class Permutation:
    def __init__(self, id):
        self.id = id
        self.counts = np.zeros((8,))

    # Sets how many times a task occurs in the permutation
    def set_count(self, taskid, count):
        self.counts[taskid] = count

    # Gets all the measured times for a task one of the five scenarios in this permutation
    def get_times(self, scenario, taskid):
        times = np.genfromtxt(f"m_{self.id}_{scenario}.csv", delimiter = ",")
        raw = times[taskid]
        return raw[:int(self.counts[taskid] * N)]

p1 = Permutation(1)
for i in range(1,8):
    p1.set_count(i, 1)

p2 = Permutation(2)
p2.set_count(mission, 1)
p2.set_count(navigate, 3)
p2.set_count(control, 3)
p2.set_count(refine, 1)
p2.set_count(report, 1)
p2.set_count(communicate, 1)
p2.set_count(avoid, 4)

p3 = Permutation(3)
p3.set_count(mission, 1)
p3.set_count(navigate, 3)
p3.set_count(control, 3)
p3.set_count(refine, 2)
p3.set_count(report, 2)
p3.set_count(communicate, 1)
p3.set_count(avoid, 5)

p4 = Permutation(4)
p4.set_count(mission, 1)
p4.set_count(navigate, 2)
p4.set_count(control, 2)
p4.set_count(refine, 1)
p4.set_count(report, 1)
p4.set_count(communicate, 1)
p4.set_count(avoid, 3)

p5 = Permutation(5)
p5.set_count(mission, 1)
p5.set_count(navigate, 2)
p5.set_count(control, 2)
p5.set_count(refine, 1)
p5.set_count(report, 1)
p5.set_count(communicate, 1)
p5.set_count(avoid, 6)

# All the times per task 
times = [list() for _ in range(8)]

# For every permutation 
for i, p in enumerate([p1, p2, p3, p4, p5]):
    # For every scenario
    for s in range(1, 6):
        # We forgot to copy the measurements for permutation 5 scenario 5
        if p == p5 and s == 5:
            continue

        # For every task
        for t in range(0, 8):
            times[t] += p.get_times(s, t).tolist()

f = open("wcet.csv", "x")
f.write("Task,N,mean,std,max,min,max_10_mean,max_10_std,max_10\n")

for t in range(1, 8):
    times_t = times[t]

    mean_t = np.mean(times_t)
    std_t = np.std(times_t)
    max_t = max(times_t)
    min_t = min(times_t)

    max_10 = sorted(times_t, reverse = True)[:10]
    max_10_mean = np.mean(max_10)
    max_10_std = np.std(max_10)

    f.write(f"{t},{len(times_t)},{mean_t},{std_t},{max_t},{min_t},{max_10_mean},{max_10_std},{max_10}\n")

    print(f"Have {len(times_t)} data points for task with id {t}.")
    print(f"Mean: {mean_t}, std: {std_t}, max: {max_t}, min: {min_t}")
    # print(f"Maximum 10: {max_10}, mean: {max_10_mean}, std: {max_10_std}")
