import matplotlib.pyplot as plt

# processes = [4, 8, 16]
# accelerations = [3.250283]

processes = [4, 8, 16]
accelerations = [0.029170, 6.250283, 9.250283]

plt.plot(processes, accelerations, marker='o', label='Speedup')

plt.title('Speedup n=200')
plt.xlabel('Number of Processes')
plt.ylabel('Speedup')

plt.legend()

plt.savefig('speedup.png')

plt.show()
