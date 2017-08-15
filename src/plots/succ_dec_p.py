import matplotlib.pyplot as plt
import numpy as np

eta= np.linspace(1, 2.5, num=16)
ps10010 = [0.045, 0.095, 0.2, 0.3, 0.42, 0.55, 0.66, 0.725, 0.765, 0.82, 0.865, 0.895, 0.925, 0.965, 0.975, 0.99]
ps10020 = [0, 0.005, 0.01, 0.015, 0.03, 0.05, 0.065, 0.115, 0.16, 0.18, 0.21, 0.265, 0.28, 0.31, 0.365, 0.39]
plt.plot(eta, ps10010, 'bs-', label='N=100 K=10')
plt.plot(eta, ps10020, 'r^-', label='N=100 K=20')
plt.legend()
plt.axis([1, 2.5, 0, 1])
plt.xlabel(r'Decoding Ratio $\eta$')
plt.ylabel(r'Successful Decoding Probability $P_s$')
plt.show()
