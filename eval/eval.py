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
    def __init__(self, minor, major, cycleid):
        self.minor = minor
        self.major = major
        self.cycleid = cycleid
        
    def load_times(self, scenario):
        self.periods = np.genfromtxt(f"p_{self.cycleid}_{scenario}.csv", delimiter = ",")
        self.times = np.genfromtxt(f"e_{self.cycleid}_{scenario}.csv", delimiter = ",")

    def evaluate(self, scenario):

s1 = Schedule(100, 600)
[s1.load_times(i) for i in range(1, 6)]
