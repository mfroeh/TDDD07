import numpy as np

packets = np.zeros((5, 2))
go_aheads = 0
seconds = 0
# For every scenario
f = open("packets.csv", "x")
for i in range(1, 9):
    csv = np.genfromtxt(f"./{i}/packets.csv", delimiter = ",")
    # For all the message types
    for m in range(5):
        packets[m][0] += csv[m][0]
        packets[m][1] += csv[m][1]

        print(csv[m])
    
    go_aheads += csv[5][0]
    seconds += csv[5][1]

f.write("Message ID, $N$, \\sum bytes, $N$ / second, bytes / second\n")
for m in range(5):
    N = packets[m][0]
    cum_bytes = packets[m][1] 
    f.write(f"{m},{N},{cum_bytes},{N / seconds},{cum_bytes / seconds}\n")
f.close()

f = open("go_aheads.csv", "x")
f.write("Go aheads received, Seconds ran, Go aheads / second\n")
f.write(f"{go_aheads},{seconds},{go_aheads/seconds}\n")
f.close()
