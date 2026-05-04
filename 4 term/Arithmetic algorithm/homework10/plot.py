import pandas as pd
import matplotlib.pyplot as plt

e_df = pd.read_csv("data/e_series.csv")
pi_df = pd.read_csv("data/pi_series.csv")

plt.figure()
plt.plot(e_df["precision"], e_df["operations"], marker='o', label="e^r")
plt.plot(pi_df["precision"], pi_df["operations"], marker='o', label="pi")
plt.xlabel("Точность")
plt.ylabel("Количество операций")
plt.legend()
plt.grid()

plt.show()