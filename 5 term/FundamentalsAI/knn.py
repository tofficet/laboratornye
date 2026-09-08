import numpy as np
import matplotlib.pyplot as plt
import random

np.random.seed(42)
random.seed(42)

n_classes = random.randint(2, 5)
k = random.randint(3, 7)

print(f"Количество классов: {n_classes}")
print(f"K (число соседей): {k}")

centers = np.random.uniform(low=-8.0, high=8.0, size=(n_classes, 2))

samples_per_class = np.ones(n_classes, dtype=int)
remaining = 50 - n_classes
extra = np.random.multinomial(remaining, [1.0/n_classes]*n_classes)
samples_per_class += extra

X_train_list = []
y_train_list = []
sigma = 1.2

for class_id in range(n_classes):
    n_points = samples_per_class[class_id]
    center = centers[class_id]
    points = np.random.normal(loc=center, scale=sigma, size=(n_points, 2))
    X_train_list.append(points)
    y_train_list.append(np.full(n_points, class_id))

X_train = np.vstack(X_train_list)
y_train = np.hstack(y_train_list)

X_test = np.random.uniform(low=-10.0, high=10.0, size=(20, 2))

def knn_predict(X_train, y_train, X_test, k):
    y_pred = []
    for test_point in X_test:
        distances = np.linalg.norm(X_train - test_point, axis=1)
        nearest_indices = np.argsort(distances)[:k]
        neighbor_labels = y_train[nearest_indices].astype(int)
        counts = np.bincount(neighbor_labels, minlength=n_classes)
        predicted_class = np.argmax(counts)
        y_pred.append(predicted_class)
    return np.array(y_pred)

y_pred = knn_predict(X_train, y_train, X_test, k)

plt.figure(figsize=(10, 8))
colors = plt.cm.tab10(np.linspace(0, 1, n_classes))

for class_id in range(n_classes):
    mask = (y_train == class_id)
    plt.scatter(X_train[mask, 0], X_train[mask, 1],
                color=colors[class_id], label=f'Класс {class_id} (обуч.)',
                s=60, alpha=0.8, edgecolors='k')

for class_id in range(n_classes):
    mask = (y_pred == class_id)
    plt.scatter(X_test[mask, 0], X_test[mask, 1],
                color=colors[class_id], marker='s', s=120,
                edgecolors='white', linewidth=1.5,
                label=f'Класс {class_id} (тест)')

plt.title(f'Классификация KNN (K={k}, классов={n_classes})')
plt.xlabel('X')
plt.ylabel('Y')
plt.xlim(-12, 12)
plt.ylim(-12, 12)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.show()

print("Предсказанные классы для тестовых точек:", y_pred)