import matplotlib.pyplot as plt

num_processes = [2, 4, 6, 8, 10, 12, 14, 16]
execution_times = [2.490220, 1.200597, 0.755327, 0.581210, 0.464277, 0.388616, 0.325262, 0.304837]
# num_processes = [8, 10, 12, 14, 16]
# execution_times = [2.497612, 2.009330, 1.697738, 1.439961, 1.232883]

execution_times_filtered = [time for time in execution_times if time is not None]

speedup = [execution_times_filtered[0] / execution_times_filtered[i] for i in range(1, len(execution_times_filtered))]
# linear_speedup = [num_processes[i] for i in range(1, len(execution_times_filtered))]

plt.figure(figsize=(8, 6))
plt.plot(num_processes[1:len(execution_times_filtered)], speedup, marker='o', linestyle='-', color='b', label='Speedup')
# plt.plot(num_processes[1:len(execution_times_filtered)], linear_speedup, linestyle='--', color='r', label='Linear Speedup')

plt.title('Speedup n=16000')
# plt.title('Speedup n=32000')
plt.xlabel('Number of Processes')
plt.ylabel('Speedup')

plt.legend()

plt.savefig('speedup.png')

plt.show()
