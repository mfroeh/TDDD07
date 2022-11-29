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

class Schedule:
    def __init__(self, minor, major, schedule):
        self.minor = minor
        self.major = major
        self.schedule = schedule
        
    def load_times(self, scenario):
        self.periods = np.genfromtxt(f"period_{self.schedule}_{scenario}.csv", delimiter = ",")
        self.times = np.genfromtxt(f"exec_{self.schedule}_{scenario}.csv", delimiter = ",")
        self.acc = np.genfromtxt(f"acc_{self.schedule}_{scenario}.csv", delimiter = ",")

    def evaluate_deadline(self):
        evaluation = list()
        # For all tasks
        for t in range(8):
            overran = 0
            by = 0
            N = sum(1 for x in self.times[t] if x)
            # For all executions of task
            for i in range(N):
                if self.times[t][i] == 0:
                    break
                deadline = (i+1) * self.periods[t]
                measured = self.times[t][i]
                if measured > deadline:
                    overran += 1
                    by += measured - deadline
            print((N, overran, by))
            evaluation.append((N, overran, by))

        return evaluation

    def evaluate_acc(self):
        count = sum(1 for x in self.acc if x[0] != 0)
        total = sum(x[2] for x in self.acc if x[0] != 0)
        mean = total / count

        return count, total, mean

# TODO: How do we evaluate distance
def eval_schedule(schedule):
    ev = np.zeros((8, 6))
    # For every scenario
    for i in range(1, 6):
        s1.load_times(i)
        # For all the tasks
        for t in range(8):
            n, o, b = s1.evaluate_deadline()[t]
            print(n)
            ev[t][0] += n
            ev[t][1] += o
            ev[t][2] += b

            c, te, m = s1.evaluate_acc()
            ev[t][3] += c
            ev[t][4] += te
            ev[t][5] += m / 5

    f = open(f"eval_{schedule.schedule}.csv", "x")
    f.write("Task,N,overran,cum_time_overran,victims_found,distance_total,distance_mean\n")
    for i in range(1, 8):
        n, o, b, c, t, m = ev[i]
        f.write(f"{i},{n},{o},{b},{c},{t},{m}\n")


s1 = Schedule(100, 600, 1)
eval_schedule(s1)

s2 = Schedule(125,3000, 2)
eval_schedule(s2)
