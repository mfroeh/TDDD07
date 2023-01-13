import numpy as np

individual_found = np.zeros((5, 29 * 10))
individual_sent = np.zeros((5, 29 * 10))
individual_sent_bytes = np.zeros((5, 29 * 10))
individual_aheads = np.zeros((29*10,))
for i in range(1, 11):
    found = np.genfromtxt(f"./new{i}/packet_found.csv", delimiter = ",")
    sent = np.genfromtxt(f"./new{i}/packet_sent.csv", delimiter = ",")
    sent_bytes = np.genfromtxt(f"./new{i}/packet_sent_bytes.csv", delimiter = ",")
    aheads = np.genfromtxt(f"./new{i}/packet_aheads.csv", delimiter = ",")

    individual_found[0:5, (i-1)*29:i*29] = found[:5, 1:30]
    individual_sent[0:5, (i-1)*29:i*29] = sent[:5, 1:30]
    individual_sent_bytes[0:5, (i-1)*29:i*29] = sent_bytes[:5, 1:30]
    individual_aheads[(i-1)*29:i*29] = aheads[1:30]
    
np.savetxt("individual_found.csv", individual_found, fmt="%s", delimiter = ",")
np.savetxt("individual_sent.csv", individual_sent, fmt="%s", delimiter = ",")
np.savetxt("individual_sent_bytes.csv", individual_sent_bytes, fmt="%s", delimiter = ",")
np.savetxt("individual_aheads.csv", individual_aheads, fmt="%s", delimiter = ",")

didnt_find_pos = []
didnt_send_pos = []
didnt_get_ahead = []

for i in range(29 * 10):
    if individual_found[0, i] == 0:
        didnt_find_pos.append(i)
    if individual_sent[0, i] == 0:
        didnt_send_pos.append(i)
    if individual_aheads[i] == 0 and i % 29 != 0:
        didnt_get_ahead.append(i)

np.savetxt("didnt_find_pos.csv", didnt_find_pos, fmt="%s", delimiter = ",")
np.savetxt("didnt_send_pos.csv", didnt_send_pos, fmt="%s", delimiter = ",")
np.savetxt("didnt_get_ahead.csv", didnt_get_ahead, fmt="%s", delimiter = ",")

var_sent = [np.var(individual_sent[i,]) for i in range(5)]
var_sent_bytes = [np.var(individual_sent_bytes[i,]) for i in range(5)]

mean_sent = [np.mean(individual_sent[i,]) for i in range(5)]
mean_sent_bytes = [np.mean(individual_sent_bytes[i,]) for i in range(5)]
mean_sent_bytes_cum = sum(mean_sent_bytes)

sum_sent = [sum(individual_sent[i,]) for i in range(5)]
sum_sent_bytes = [sum(individual_sent_bytes[i,]) for i in range(5)]
sum_sent_bytes_cum = sum(sum_sent_bytes)

min_sent = [min(individual_sent[i,]) for i in range(5)]
min_sent_bytes = [min(individual_sent_bytes[i,]) for i in range(5)]
max_sent = [max(individual_sent[i,]) for i in range(5)]
max_sent_bytes = [max(individual_sent_bytes[i,]) for i in range(5)]

stats = np.array([sum_sent, sum_sent_bytes, mean_sent, mean_sent_bytes, var_sent, var_sent_bytes, min_sent, min_sent_bytes, max_sent, max_sent_bytes]).round(decimals=2)
np.savetxt("stats.csv", stats, fmt="%.2f", delimiter = ",")
np.savetxt("cumulated.csv", [mean_sent_bytes_cum, sum_sent_bytes_cum], delimiter = ",")

print(stats)
